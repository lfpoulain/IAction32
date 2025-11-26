/*
 * Implémentation MQTT avec Auto-Discovery Home Assistant
 */

#include "mqtt_manager.h"
#include "config.h"
#include "wifi_manager.h"
#include <PubSubClient.h>
#include <WiFi.h>

static WiFiClient wifiClient;
static PubSubClient mqttClient(wifiClient);
static unsigned long lastReconnectAttempt = 0;
static unsigned long lastDiscoveryPublish = 0;
static String deviceId;

// Callback pour les messages MQTT entrants
static void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  String commandTopic = cfg.mqtt_topic + "/" + deviceId + "/capture/set";
  if (String(topic) == commandTopic) {
    cfg.capture_enabled = (message == "ON");
    Serial.printf("📡 MQTT: Capture %s\n", cfg.capture_enabled ? "activée" : "désactivée");
    
    // Publier l'état actuel
    String stateTopic = cfg.mqtt_topic + "/" + deviceId + "/capture/state";
    mqttClient.publish(stateTopic.c_str(), cfg.capture_enabled ? "ON" : "OFF", true);
  }
}

// ========== INIT ==========
void MQTTManager::init() {
  if (!cfg.mqtt_enabled) {
    Serial.println("📡 MQTT désactivé");
    return;
  }

  Serial.println("📡 Initialisation MQTT...");

  // Générer un ID unique basé sur l'adresse MAC
  uint8_t mac[6];
  WiFi.macAddress(mac);
  deviceId = "iaction32_" + String(mac[3], HEX) + String(mac[4], HEX) + String(mac[5], HEX);

  mqttClient.setServer(cfg.mqtt_server.c_str(), cfg.mqtt_port);
  mqttClient.setBufferSize(2048); // Buffer plus grand pour les messages discovery
  mqttClient.setCallback(mqttCallback);

  Serial.printf("  Device ID: %s\n", deviceId.c_str());
  Serial.println("✓ MQTT initialisé");
}

// ========== CONNECT ==========
void MQTTManager::connect() {
  if (!cfg.mqtt_enabled || !WiFiManager::isConnected()) return;

  if (mqttClient.connected()) return;

  Serial.println("📡 Connexion au broker MQTT...");
  Serial.printf("  Serveur: %s:%d\n", cfg.mqtt_server.c_str(), cfg.mqtt_port);

  // Will message (LWT - Last Will Testament)
  String availTopic = cfg.mqtt_topic + "/" + deviceId + "/availability";

  bool connected = false;
  if (cfg.mqtt_user.length() > 0) {
    connected = mqttClient.connect(
      deviceId.c_str(),
      cfg.mqtt_user.c_str(),
      cfg.mqtt_password.c_str(),
      availTopic.c_str(),
      0,
      true,
      "offline"
    );
  } else {
    connected = mqttClient.connect(
      deviceId.c_str(),
      availTopic.c_str(),
      0,
      true,
      "offline"
    );
  }

  if (connected) {
    Serial.println("✓ MQTT connecté");

    // Publier availability
    mqttClient.publish(availTopic.c_str(), "online", true);

    // S'abonner au topic de commande pour le switch
    String commandTopic = cfg.mqtt_topic + "/" + deviceId + "/capture/set";
    mqttClient.subscribe(commandTopic.c_str());
    Serial.printf("  Abonné à: %s\n", commandTopic.c_str());

    // Publier discovery config pour Home Assistant
    publishDiscovery();
    lastDiscoveryPublish = millis();
    
    // Publier l'état initial du switch
    String stateTopic = cfg.mqtt_topic + "/" + deviceId + "/capture/state";
    mqttClient.publish(stateTopic.c_str(), cfg.capture_enabled ? "ON" : "OFF", true);

  } else {
    Serial.printf("✗ Échec connexion MQTT (code: %d)\n", mqttClient.state());
  }
}

// ========== LOOP ==========
void MQTTManager::loop() {
  if (!cfg.mqtt_enabled) return;

  unsigned long now = millis();
  
  if (!mqttClient.connected()) {
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      connect();
    }
  } else {
    mqttClient.loop();
    
    // Republier discovery toutes les 5 minutes pour garantir les sensors
    if (now - lastDiscoveryPublish > 300000) {
      lastDiscoveryPublish = now;
      publishDiscovery();
    }
  }
}

// ========== IS CONNECTED ==========
bool MQTTManager::isConnected() {
  return cfg.mqtt_enabled && mqttClient.connected();
}

// ========== PUBLISH RESULTS ==========
void MQTTManager::publishResults(DynamicJsonDocument& results) {
  if (!isConnected()) return;

  // Publier chaque résultat dans son propre topic
  for (JsonPair kv : results.as<JsonObject>()) {
    String topic = cfg.mqtt_topic + "/" + deviceId + "/" + String(kv.key().c_str());
    String value;

    if (kv.value().is<bool>()) {
      value = kv.value().as<bool>() ? "ON" : "OFF";
    } else if (kv.value().is<int>()) {
      value = String(kv.value().as<int>());
    } else if (kv.value().is<float>()) {
      value = String(kv.value().as<float>(), 2);
    } else {
      value = kv.value().as<String>();
    }

    mqttClient.publish(topic.c_str(), value.c_str(), true);
    Serial.printf("📤 MQTT: %s = %s\n", topic.c_str(), value.c_str());
  }

  // Publier aussi un état global en JSON
  String stateTopic = cfg.mqtt_topic + "/" + deviceId + "/state";
  String payload;
  serializeJson(results, payload);
  mqttClient.publish(stateTopic.c_str(), payload.c_str(), true);
}

// ========== PUBLISH DISCOVERY ==========
void MQTTManager::publishDiscovery() {
  if (!isConnected()) return;

  Serial.println("📡 Publication de la configuration Auto-Discovery...");

  // Device info commun
  DynamicJsonDocument deviceDoc(512);
  JsonObject device = deviceDoc.createNestedObject("device");
  device["identifiers"][0] = deviceId;
  device["name"] = cfg.device_name;
  device["model"] = "ESP32-CAM";
  device["manufacturer"] = "Les Frères Poulain";
  device["sw_version"] = "2.2";

  // Pour chaque question, créer un sensor/binary_sensor
  for (int i = 0; i < cfg.questionsCount; i++) {
    if (!cfg.questions[i].enabled) continue;

    String objectId = deviceId + "_" + cfg.questions[i].jsonKey;
    String component = (cfg.questions[i].type == QUESTION_BOOL) ? "binary_sensor" : "sensor";
    String discoveryTopic = "homeassistant/" + component + "/" + objectId + "/config";

    DynamicJsonDocument doc(1024);

    // Configuration de base
    doc["name"] = cfg.questions[i].jsonKey;
    doc["unique_id"] = objectId;
    doc["state_topic"] = cfg.mqtt_topic + "/" + deviceId + "/" + cfg.questions[i].jsonKey;
    doc["availability_topic"] = cfg.mqtt_topic + "/" + deviceId + "/availability";
    doc["json_attributes_topic"] = cfg.mqtt_topic + "/" + deviceId + "/state";

    // Icon selon le type
    if (cfg.questions[i].jsonKey.indexOf("person") >= 0) {
      doc["icon"] = "mdi:account-alert";
    } else if (cfg.questions[i].jsonKey.indexOf("car") >= 0) {
      doc["icon"] = "mdi:car";
    } else if (cfg.questions[i].jsonKey.indexOf("package") >= 0) {
      doc["icon"] = "mdi:package-variant";
    } else {
      doc["icon"] = "mdi:eye";
    }

    // Config spécifique au type
    if (cfg.questions[i].type == QUESTION_BOOL) {
      doc["payload_on"] = "ON";
      doc["payload_off"] = "OFF";
      doc["device_class"] = "motion";
    } else {
      doc["unit_of_measurement"] = "";
      doc["state_class"] = "measurement";
    }

    // Ajouter device info
    doc["device"] = device;

    String payload;
    serializeJson(doc, payload);

    mqttClient.publish(discoveryTopic.c_str(), payload.c_str(), true);
    Serial.printf("  ✓ Discovery: %s\n", cfg.questions[i].jsonKey.c_str());

    delay(100); // Petit délai entre chaque publication
  }

  // Créer un sensor diagnostic pour l'adresse IP
  String ipSensorId = deviceId + "_ip";
  String ipDiscoveryTopic = "homeassistant/sensor/" + ipSensorId + "/config";

  DynamicJsonDocument ipDoc(1024);
  ipDoc["name"] = "IP Address";
  ipDoc["unique_id"] = ipSensorId;
  ipDoc["state_topic"] = cfg.mqtt_topic + "/" + deviceId + "/ip";
  ipDoc["availability_topic"] = cfg.mqtt_topic + "/" + deviceId + "/availability";
  ipDoc["icon"] = "mdi:ip-network";
  ipDoc["entity_category"] = "diagnostic";
  ipDoc["device"] = device;

  String ipPayload;
  serializeJson(ipDoc, ipPayload);
  mqttClient.publish(ipDiscoveryTopic.c_str(), ipPayload.c_str(), true);
  
  // Publier l'adresse IP
  String ipTopic = cfg.mqtt_topic + "/" + deviceId + "/ip";
  mqttClient.publish(ipTopic.c_str(), WiFi.localIP().toString().c_str(), true);

  // Créer un switch pour activer/désactiver la capture
  String switchId = deviceId + "_capture";
  String switchDiscoveryTopic = "homeassistant/switch/" + switchId + "/config";

  DynamicJsonDocument switchDoc(1024);
  switchDoc["name"] = "Capture";
  switchDoc["unique_id"] = switchId;
  switchDoc["state_topic"] = cfg.mqtt_topic + "/" + deviceId + "/capture/state";
  switchDoc["command_topic"] = cfg.mqtt_topic + "/" + deviceId + "/capture/set";
  switchDoc["availability_topic"] = cfg.mqtt_topic + "/" + deviceId + "/availability";
  switchDoc["icon"] = "mdi:camera";
  switchDoc["payload_on"] = "ON";
  switchDoc["payload_off"] = "OFF";
  switchDoc["device"] = device;

  String switchPayload;
  serializeJson(switchDoc, switchPayload);
  mqttClient.publish(switchDiscoveryTopic.c_str(), switchPayload.c_str(), true);

  Serial.println("✓ Configuration Auto-Discovery publiée");
}

// ========== PUBLISH CAPTURE STATE ==========
void MQTTManager::publishCaptureState() {
  if (!isConnected()) return;
  
  String stateTopic = cfg.mqtt_topic + "/" + deviceId + "/capture/state";
  mqttClient.publish(stateTopic.c_str(), cfg.capture_enabled ? "ON" : "OFF", true);
}

// ========== REMOVE DISCOVERY ==========
void MQTTManager::removeDiscovery(const String& jsonKey, int questionType) {
  if (!isConnected()) return;

  String objectId = deviceId + "_" + jsonKey;
  String component = (questionType == QUESTION_BOOL) ? "binary_sensor" : "sensor";
  String discoveryTopic = "homeassistant/" + component + "/" + objectId + "/config";

  // Publier un payload vide pour supprimer l'entité de Home Assistant
  mqttClient.publish(discoveryTopic.c_str(), "", true);
  
  // Supprimer aussi la valeur du topic
  String valueTopic = cfg.mqtt_topic + "/" + deviceId + "/" + jsonKey;
  mqttClient.publish(valueTopic.c_str(), "", true);

  Serial.printf("🗑️ Discovery supprimé: %s\n", jsonKey.c_str());
}

// ========== DISCONNECT ==========
void MQTTManager::disconnect() {
  if (mqttClient.connected()) {
    String availTopic = cfg.mqtt_topic + "/" + deviceId + "/availability";
    mqttClient.publish(availTopic.c_str(), "offline", true);
    mqttClient.disconnect();
    Serial.println("✓ MQTT déconnecté");
  }
}

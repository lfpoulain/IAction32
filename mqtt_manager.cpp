/*
 * Implémentation MQTT avec Auto-Discovery Home Assistant
 */

#include "mqtt_manager.h"
#include "config.h"
#include "wifi_manager.h"
#include "logger.h"
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
    Logger::printf("MQTT: Capture %s", cfg.capture_enabled ? "ON" : "OFF");
    
    // Publier l'état actuel
    String stateTopic = cfg.mqtt_topic + "/" + deviceId + "/capture/state";
    mqttClient.publish(stateTopic.c_str(), cfg.capture_enabled ? "ON" : "OFF", true);
  }
}

// ========== INIT ==========
void MQTTManager::init() {
  if (!cfg.mqtt_enabled) {
    Logger::log("MQTT disabled");
    return;
  }

  Logger::log("MQTT init...");

  // Générer un ID unique basé sur l'adresse MAC
  uint8_t mac[6];
  WiFi.macAddress(mac);
  deviceId = "iaction32_" + String(mac[3], HEX) + String(mac[4], HEX) + String(mac[5], HEX);

  mqttClient.setServer(cfg.mqtt_server.c_str(), cfg.mqtt_port);
  mqttClient.setBufferSize(2048); // Buffer plus grand pour les messages discovery
  mqttClient.setCallback(mqttCallback);

  Logger::printf("MQTT ready (ID: %s)", deviceId.c_str());
}

// ========== CONNECT ==========
void MQTTManager::connect() {
  if (!cfg.mqtt_enabled || !WiFiManager::isConnected()) return;

  if (mqttClient.connected()) return;

  Logger::printf("MQTT connecting to %s:%d", cfg.mqtt_server.c_str(), cfg.mqtt_port);

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
    Logger::log("MQTT connected");

    // Publier availability
    mqttClient.publish(availTopic.c_str(), "online", true);

    // S'abonner au topic de commande pour le switch
    String commandTopic = cfg.mqtt_topic + "/" + deviceId + "/capture/set";
    mqttClient.subscribe(commandTopic.c_str());
    Logger::printf("Subscribed: %s", commandTopic.c_str());

    // Publier discovery config pour Home Assistant
    publishDiscovery();
    lastDiscoveryPublish = millis();
    
    // Publier l'état initial du switch
    String stateTopic = cfg.mqtt_topic + "/" + deviceId + "/capture/state";
    mqttClient.publish(stateTopic.c_str(), cfg.capture_enabled ? "ON" : "OFF", true);

  } else {
    Logger::printf("MQTT failed (code: %d)", mqttClient.state());
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
    Logger::printf("MQTT: %s=%s", topic.c_str(), value.c_str());
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

  Logger::log("Publishing HA discovery...");

  // Device info commun (réutilisé pour chaque sensor)
  StaticJsonDocument<256> deviceDoc;
  JsonObject device = deviceDoc.createNestedObject("device");
  device["identifiers"][0] = deviceId;
  device["name"] = cfg.device_name;
  device["model"] = F("ESP32-CAM IAction32");
  device["manufacturer"] = F("Les Frères Poulain");
  device["sw_version"] = F("2.3");

  // Pour chaque question, créer un sensor/binary_sensor
  for (int i = 0; i < cfg.questionsCount; i++) {
    if (!cfg.questions[i].enabled) continue;

    String objectId = deviceId + "_" + cfg.questions[i].jsonKey;
    String component = (cfg.questions[i].type == QUESTION_BOOL) ? "binary_sensor" : "sensor";
    String discoveryTopic = "homeassistant/" + component + "/" + objectId + "/config";

    StaticJsonDocument<512> doc;

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
    Logger::printf("Discovery: %s", cfg.questions[i].jsonKey.c_str());

    delay(100); // Petit délai entre chaque publication
  }

  // Créer un sensor diagnostic pour l'adresse IP
  String ipSensorId = deviceId + "_ip";
  String ipDiscoveryTopic = "homeassistant/sensor/" + ipSensorId + "/config";

  StaticJsonDocument<384> ipDoc;
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

  StaticJsonDocument<384> switchDoc;
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

  Logger::log("HA discovery published");
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

  Logger::printf("Discovery removed: %s", jsonKey.c_str());
}

// ========== DISCONNECT ==========
void MQTTManager::disconnect() {
  if (mqttClient.connected()) {
    String availTopic = cfg.mqtt_topic + "/" + deviceId + "/availability";
    mqttClient.publish(availTopic.c_str(), "offline", true);
    mqttClient.disconnect();
    Logger::log("MQTT disconnected");
  }
}

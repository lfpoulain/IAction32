/*
 * Implémentation du serveur web et des handlers
 */

#include "web_server.h"
#include "web_html.h"
#include "web_pages.h"
#include "config.h"
#include "storage.h"
#include "camera_setup.h"
#include "wifi_manager.h"
#include "ai_provider.h"
#include "mqtt_manager.h"
#include <WebServer.h>
#include <ArduinoJson.h>
#include "esp_camera.h"

static WebServer server(80);

// ========== HANDLER: PAGE PRINCIPALE ==========
void handleRoot() {
  String html = FPSTR(HTML_HEAD);
  html += buildDashboardPage();
  html += buildConfigPage();
  html += buildQuestionsPage();
  html += buildCameraPage();
  html += FPSTR(HTML_FOOT);
  server.send(200, "text/html", html);
}

// ========== HANDLER: CAPTURE IMAGE ==========
void handleCapture() {
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    server.send(500, "text/plain", "Erreur capture");
    return;
  }

  server.sendHeader("Content-Disposition", "inline; filename=capture.jpg");
  server.send_P(200, "image/jpeg", (const char*)fb->buf, fb->len);
  esp_camera_fb_return(fb);
}

// ========== HANDLER: SAVE WIFI ==========
void handleSaveWifi() {
  if (server.hasArg("wifi_ssid")) cfg.wifi_ssid = server.arg("wifi_ssid");
  if (server.hasArg("wifi_pass")) cfg.wifi_password = server.arg("wifi_pass");
  Storage::save();
  server.sendHeader("Location", "/#config");
  server.send(302, "text/plain", "WiFi saved");
}

// ========== HANDLER: SAVE AI ==========
void handleSaveAI() {
  if (server.hasArg("lm_host")) cfg.lm_host = server.arg("lm_host");
  if (server.hasArg("lm_model")) cfg.lm_model = server.arg("lm_model");
  if (server.hasArg("openai_key")) cfg.openai_key = server.arg("openai_key");
  if (server.hasArg("provider")) cfg.provider = (AIProviderType)server.arg("provider").toInt();
  Storage::save();
  server.sendHeader("Location", "/#config");
  server.send(302, "text/plain", "AI saved");
}

// ========== HANDLER: SAVE MQTT ==========
void handleSaveMQTT() {
  if (server.hasArg("mqtt_server")) cfg.mqtt_server = server.arg("mqtt_server");
  if (server.hasArg("mqtt_port")) cfg.mqtt_port = server.arg("mqtt_port").toInt();
  if (server.hasArg("mqtt_user")) cfg.mqtt_user = server.arg("mqtt_user");
  if (server.hasArg("mqtt_pass")) cfg.mqtt_password = server.arg("mqtt_pass");
  if (server.hasArg("mqtt_topic")) cfg.mqtt_topic = server.arg("mqtt_topic");
  if (server.hasArg("device_name")) cfg.device_name = server.arg("device_name");
  cfg.mqtt_enabled = server.hasArg("mqtt_enabled");
  
  Storage::save();

  // Reconnecter MQTT si nécessaire
  if (cfg.mqtt_enabled) {
    MQTTManager::disconnect();
    MQTTManager::init();
    MQTTManager::connect();
  }

  server.sendHeader("Location", "/#config");
  server.send(302, "text/plain", "MQTT saved");
}

// ========== HANDLER: SAVE CAMERA ==========
void handleSaveCamera() {
  if (server.hasArg("framesize")) cfg.camera.framesize = server.arg("framesize").toInt();
  if (server.hasArg("quality")) cfg.camera.quality = server.arg("quality").toInt();
  if (server.hasArg("brightness")) cfg.camera.brightness = server.arg("brightness").toInt();
  if (server.hasArg("contrast")) cfg.camera.contrast = server.arg("contrast").toInt();
  if (server.hasArg("saturation")) cfg.camera.saturation = server.arg("saturation").toInt();

  cfg.camera.hmirror = server.hasArg("hmirror");
  cfg.camera.vflip = server.hasArg("vflip");
  cfg.camera.awb = server.hasArg("awb");

  Storage::save();
  Camera::applySettings();

  server.sendHeader("Location", "/?camera_saved=1");
  server.send(302, "text/plain", "Camera settings saved");
}

// ========== HANDLER: SAVE QUESTION ==========
void handleSaveQuestion() {
  if (!server.hasArg("id")) {
    server.send(400, "text/plain", "Missing id");
    return;
  }

  int id = server.arg("id").toInt();
  if (id < 0 || id >= cfg.questionsCount) {
    server.send(400, "text/plain", "Invalid id");
    return;
  }

  // Vérifier si des changements critiques (type ou clé) nécessitent un nettoyage MQTT
  bool needsCleanup = false;
  String oldKey = cfg.questions[id].jsonKey;
  QuestionType oldType = cfg.questions[id].type;
  
  QuestionType newType = cfg.questions[id].type;
  String newKey = cfg.questions[id].jsonKey;

  if (server.hasArg("type")) newType = (QuestionType)server.arg("type").toInt();
  if (server.hasArg("key")) newKey = server.arg("key");

  if (newType != oldType || newKey != oldKey) {
    needsCleanup = true;
  }

  // Si changement, supprimer l'ancien sensor de HA
  if (needsCleanup) {
    MQTTManager::removeDiscovery(oldKey, oldType);
  }

  if (server.hasArg("prompt")) cfg.questions[id].prompt = server.arg("prompt");
  cfg.questions[id].type = newType;
  cfg.questions[id].jsonKey = newKey;

  Storage::save();
  
  // Si changement, republier le discovery pour créer le nouveau sensor
  if (needsCleanup) {
    MQTTManager::publishDiscovery();
  }

  server.sendHeader("Location", "/#questions");
  server.send(302, "text/plain", "Saved");
}

// ========== HANDLER: ADD QUESTION ==========
void handleAddQuestion() {
  if (cfg.questionsCount >= MAX_QUESTIONS) {
    server.send(400, "application/json", "{\"success\":false,\"error\":\"Max questions reached\"}");
    return;
  }

  cfg.questions[cfg.questionsCount].prompt = "Describe what you see briefly.";
  cfg.questions[cfg.questionsCount].type = QUESTION_BOOL;
  cfg.questions[cfg.questionsCount].jsonKey = "question_" + String(cfg.questionsCount + 1);
  cfg.questions[cfg.questionsCount].enabled = true;
  cfg.questionsCount++;

  Storage::save();
  
  // Publier la nouvelle configuration MQTT
  MQTTManager::publishDiscovery();

  // Si c'est une requête AJAX, retourner JSON
  if (server.hasHeader("X-Requested-With")) {
    server.send(200, "application/json", "{\"success\":true,\"count\":" + String(cfg.questionsCount) + "}");
  } else {
    server.sendHeader("Location", "/#questions");
    server.send(302, "text/plain", "Added");
  }
}

// ========== HANDLER: DELETE QUESTION ==========
void handleDeleteQuestion() {
  if (!server.hasArg("id")) {
    server.send(400, "text/plain", "Missing id");
    return;
  }

  int id = server.arg("id").toInt();
  if (id < 0 || id >= cfg.questionsCount) {
    server.send(400, "text/plain", "Invalid id");
    return;
  }

  // Supprimer le sensor de Home Assistant avant de supprimer la question
  MQTTManager::removeDiscovery(cfg.questions[id].jsonKey, cfg.questions[id].type);

  // Décaler les questions
  for (int i = id; i < cfg.questionsCount - 1; i++) {
    cfg.questions[i] = cfg.questions[i + 1];
  }
  cfg.questionsCount--;

  Storage::save();

  server.sendHeader("Location", "/#questions");
  server.send(302, "text/plain", "Deleted");
}

// ========== API: STATUS ==========
void handleApiStatus() {
  DynamicJsonDocument doc(512);
  doc["captures"] = stats.captureCount;
  doc["errors"] = stats.errorCount;
  doc["lastResult"] = stats.lastResult;
  doc["wifiConnected"] = WiFiManager::isConnected();
  doc["ip"] = WiFiManager::getIP();
  doc["captureEnabled"] = cfg.capture_enabled;
  doc["captureModeLive"] = cfg.capture_mode_live;
  doc["intervalSeconds"] = cfg.interval_seconds;

  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

// ========== RUN CAPTURE CYCLE ==========
String runCaptureCycle() {
  if (stats.captureInProgress) {
    return "{\"error\":\"Capture in progress\"}";
  }

  stats.captureInProgress = true;

  String base64Image = Camera::captureBase64();
  DynamicJsonDocument doc(2048);
  DynamicJsonDocument results(1024);

  if (base64Image.length() == 0) {
    doc["success"] = false;
    doc["error"] = "Erreur capture image";
    stats.errorCount++;
  } else {
    bool allSuccess = false;
    
    if (cfg.questionsCount > 0) {
      DynamicJsonDocument batchResults(4096);
      if (AIProvider::sendBatchToAI(base64Image, batchResults)) {
        // Copier les résultats du batch dans l'objet results final
        // et vérifier si toutes les questions attendues sont là
        allSuccess = true;
        for (int i = 0; i < cfg.questionsCount; i++) {
          if (!cfg.questions[i].enabled) continue;
          
          String key = cfg.questions[i].jsonKey;
          if (batchResults.containsKey(key)) {
            results[key] = batchResults[key];
          } else {
            results[key] = "Missing in AI response";
            allSuccess = false;
          }
        }
      } else {
        // Erreur globale (HTTP ou parsing)
        for (int i = 0; i < cfg.questionsCount; i++) {
          if (cfg.questions[i].enabled) {
            results[cfg.questions[i].jsonKey] = "AI Error";
          }
        }
      }
    } else {
      allSuccess = true; // Pas de questions, donc succès technique
    }

    stats.captureCount++;
    stats.lastCaptureTimestamp = millis();

    if (allSuccess) {
      doc["success"] = true;
      stats.lastResult = "OK";
      MQTTManager::publishResults(results);
    } else {
      stats.errorCount++;
      doc["success"] = false;
      doc["error"] = "Erreur IA ou réponse incomplète";
      stats.lastResult = "Erreur";
    }
    
    doc["results"] = results;
  }
  
  stats.captureInProgress = false;
  
  String json;
  serializeJson(doc, json);
  return json;
}

// ========== API: CAPTURE ==========
void handleApiCapture() {
  if (stats.captureInProgress) {
    server.send(503, "application/json", "{\"error\":\"Capture in progress\"}");
    return;
  }
  String json = runCaptureCycle();
  server.send(200, "application/json", json);
}

// ========== PROCESS AUTO CAPTURE ==========
unsigned long lastAutoCaptureTime = 0;

void WebServerManager::processAutoCapture() {
  // Ne rien faire si capture désactivée
  if (!cfg.capture_enabled) return;
  
  unsigned long now = millis();
  bool shouldCapture = false;

  if (cfg.capture_mode_live) {
    // Mode Live: capturer dès que possible (avec petit délai 100ms)
    if (now - lastAutoCaptureTime > 100) shouldCapture = true;
  } else {
    // Mode Interval
    if (now - lastAutoCaptureTime > ((unsigned long)cfg.interval_seconds * 1000)) shouldCapture = true;
  }

  if (shouldCapture && !stats.captureInProgress) {
    runCaptureCycle();
    lastAutoCaptureTime = millis();
  }
}

// ========== API: TOGGLE CAPTURE ==========
void handleApiToggleCapture() {
  if (server.hasArg("enabled")) {
    cfg.capture_enabled = server.arg("enabled") == "1";
    Storage::save();
    MQTTManager::publishCaptureState();
  }
  server.send(200, "application/json", "{\"captureEnabled\":" + String(cfg.capture_enabled ? "true" : "false") + "}");
}

// ========== API: SET MODE ==========
void handleApiSetMode() {
  if (server.hasArg("live")) {
    cfg.capture_mode_live = server.arg("live") == "1";
    Storage::save();
  }
  server.send(200, "application/json", "{\"captureModeLive\":" + String(cfg.capture_mode_live ? "true" : "false") + "}");
}

// ========== API: SET INTERVAL ==========
void handleApiSetInterval() {
  if (server.hasArg("seconds")) {
    cfg.interval_seconds = server.arg("seconds").toInt();
    if (cfg.interval_seconds < 5) cfg.interval_seconds = 5;  // Minimum 5 secondes
    Storage::save();
  }
  server.send(200, "application/json", "{\"interval\":" + String(cfg.interval_seconds) + "}");
}

// ========== API: REFRESH MODELS ==========
void handleApiRefreshModels() {
  int provider = -1;
  String host = "";
  
  if (server.hasArg("provider")) provider = server.arg("provider").toInt();
  if (server.hasArg("host")) host = server.arg("host");
  
  String models = AIProvider::fetchModels(provider, host);
  server.send(200, "application/json", "{\"models\":" + models + "}");
}

// ========== API: SCAN WIFI ==========
void handleApiScanWiFi() {
  String networks = WiFiManager::scanNetworks();
  server.send(200, "application/json", "{\"networks\":" + networks + "}");
}

// ========== HANDLER: REBOOT ==========
void handleReboot() {
  server.send(200, "text/html",
    "<html><body style='background:#0a0e27;color:#fff;font-family:sans-serif;text-align:center;padding:50px;'>"
    "<h1>🔄 Redémarrage...</h1>"
    "<p>Reconnexion dans 5 secondes...</p>"
    "<script>setTimeout(function(){location.href='/';},5000);</script>"
    "</body></html>");
  delay(500);
  ESP.restart();
}

// ========== INIT ==========
void WebServerManager::init() {
  Serial.println("🌐 Démarrage du serveur web...");

  server.on("/", handleRoot);
  server.on("/capture", handleCapture);
  server.on("/save_wifi", HTTP_POST, handleSaveWifi);
  server.on("/save_ai", HTTP_POST, handleSaveAI);
  server.on("/save_mqtt", HTTP_POST, handleSaveMQTT);
  server.on("/save_camera", HTTP_POST, handleSaveCamera);
  server.on("/save_question", HTTP_POST, handleSaveQuestion);
  server.on("/add_question", handleAddQuestion);
  server.on("/delete_question", handleDeleteQuestion);
  server.on("/api/status", handleApiStatus);
  server.on("/api/capture", handleApiCapture);
  server.on("/api/toggle_capture", handleApiToggleCapture);
  server.on("/api/set_mode", handleApiSetMode);
  server.on("/api/set_interval", handleApiSetInterval);
  server.on("/api/refresh_models", handleApiRefreshModels);
  server.on("/api/scan_wifi", handleApiScanWiFi);
  server.on("/reboot", handleReboot);

  server.begin();
  Serial.println("✓ Serveur web démarré");
}

void WebServerManager::handle() {
  server.handleClient();
}

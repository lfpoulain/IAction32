/*
 * IAction32 - Main File
 *
 * Multi-provider AI vision system with advanced UI
 * Supports: LM Studio, Ollama, OpenAI
 *
 * Par Artus Poulain - Les Frères Poulain
 */

#include "config.h"
#include "camera_setup.h"
#include "wifi_manager.h"
#include "ai_provider.h"
#include "mqtt_manager.h"
#include "web_server.h"
#include "storage.h"
#include "logger.h"

void setup() {
  Serial.begin(115200);
  Logger::init();
  
  Logger::log("================================");
  Logger::log("  IAction32 v2.5");
  Logger::log("  Multi-Provider AI + MQTT/HA");
  Logger::log("================================");

  // Initialisation
  Storage::init();
  Storage::load();

  Camera::init();
  Camera::applySettings();

  WiFiManager::connect();

  // MQTT
  MQTTManager::init();
  MQTTManager::connect();

  WebServerManager::init();

  Logger::printf("\n✓ Interface web: http://%s/\n", WiFiManager::getIP().c_str());
  Logger::log("✓ Système prêt\n");
}

void loop() {
  WebServerManager::handle();
  yield(); // Laisser le WiFi/systeme respirer
  WebServerManager::processAutoCapture();
  yield();
  WiFiManager::checkConnection();
  MQTTManager::loop();
  delay(1); // Minimal delay
}

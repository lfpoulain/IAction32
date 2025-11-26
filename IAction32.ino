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

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n╔════════════════════════════════════════╗");
  Serial.println("║   IAction32 v2.3                      ║");
  Serial.println("║   Multi-Provider AI + MQTT/HA         ║");
  Serial.println("╚════════════════════════════════════════╝\n");

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

  Serial.printf("\n✓ Interface web: http://%s/\n", WiFiManager::getIP().c_str());
  Serial.println("✓ Système prêt\n");
}

void loop() {
  WebServerManager::handle();
  WebServerManager::processAutoCapture();
  WiFiManager::checkConnection();
  MQTTManager::loop();
  delay(10);
}

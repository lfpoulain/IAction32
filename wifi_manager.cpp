/*
 * Implémentation de la gestion WiFi
 */

#include "wifi_manager.h"
#include "config.h"
#include "logger.h"
#include <WiFi.h>
#include <ArduinoJson.h>

static bool connected = false;

void WiFiManager::connect() {
  Logger::printf("WiFi connecting to %s...", cfg.wifi_ssid.c_str());

  WiFi.mode(WIFI_STA);
  WiFi.begin(cfg.wifi_ssid.c_str(), cfg.wifi_password.c_str());

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    connected = true;
    Logger::printf("WiFi connected! IP: %s", WiFi.localIP().toString().c_str());
  } else {
    connected = false;
    Logger::log("WiFi failed, starting AP...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    Logger::printf("AP started: %s (IP: %s)", AP_SSID, WiFi.softAPIP().toString().c_str());
  }
}

void WiFiManager::checkConnection() {
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck < 30000) return; // Vérifier toutes les 30s
  lastCheck = millis();

  if (WiFi.status() != WL_CONNECTED) {
    connected = false;
    Logger::log("WiFi lost, reconnecting...");
    connect();
  }
}

bool WiFiManager::isConnected() {
  return connected && (WiFi.status() == WL_CONNECTED);
}

String WiFiManager::getIP() {
  return connected ? WiFi.localIP().toString() : WiFi.softAPIP().toString();
}

String WiFiManager::scanNetworks() {
  DynamicJsonDocument doc(2048);
  JsonArray arr = doc.to<JsonArray>();

  Logger::log("Scanning WiFi networks...");
  int n = WiFi.scanNetworks();

  Logger::printf("%d networks found", n);
  for (int i = 0; i < n; i++) {
    arr.add(WiFi.SSID(i));
  }

  String result;
  serializeJson(doc, result);
  return result;
}

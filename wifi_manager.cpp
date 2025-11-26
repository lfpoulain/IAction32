/*
 * Implémentation de la gestion WiFi
 */

#include "wifi_manager.h"
#include "config.h"
#include <WiFi.h>
#include <ArduinoJson.h>

static bool connected = false;

void WiFiManager::connect() {
  Serial.printf("📡 Connexion à %s...\n", cfg.wifi_ssid.c_str());

  WiFi.mode(WIFI_STA);
  WiFi.begin(cfg.wifi_ssid.c_str(), cfg.wifi_password.c_str());

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    connected = true;
    Serial.printf("\n✓ WiFi connecté! IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\n⚠ Échec WiFi, démarrage du point d'accès...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    Serial.printf("✓ AP démarré: %s (IP: %s)\n", AP_SSID, WiFi.softAPIP().toString().c_str());
  }
}

void WiFiManager::checkConnection() {
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck < 30000) return; // Vérifier toutes les 30s
  lastCheck = millis();

  if (connected && WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠ WiFi perdu, reconnexion...");
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

  Serial.println("📡 Scan des réseaux WiFi...");
  int n = WiFi.scanNetworks();

  Serial.printf("  ✓ %d réseaux trouvés\n", n);
  for (int i = 0; i < n; i++) {
    arr.add(WiFi.SSID(i));
  }

  String result;
  serializeJson(doc, result);
  return result;
}

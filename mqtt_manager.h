/*
 * Gestion MQTT avec Auto-Discovery Home Assistant
 */

#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>

namespace MQTTManager {
  void init();
  void connect();
  void loop();
  bool isConnected();
  void publishResults(DynamicJsonDocument& results);
  void publishDiscovery();
  void publishCaptureState();
  void removeDiscovery(const String& jsonKey, int questionType);
  void disconnect();
}

#endif

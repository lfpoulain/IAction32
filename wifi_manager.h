/*
 * Gestion WiFi
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>

namespace WiFiManager {
  void connect();
  void checkConnection();
  bool isConnected();
  String getIP();
  String scanNetworks();
}

#endif

/*
 * Serveur Web
 */

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>

namespace WebServerManager {
  void init();
  void handle();
  void processAutoCapture();
}

#endif

/*
 * Gestion des providers IA
 */

#ifndef AI_PROVIDER_H
#define AI_PROVIDER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "config.h"

namespace AIProvider {
  bool sendToAI(const String& base64Image, const Question& question, String& response);
  String fetchModels();
}

#endif

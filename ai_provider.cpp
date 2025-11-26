/*
 * Implémentation des providers IA (LM Studio, Ollama, OpenAI)
 */

#include "ai_provider.h"
#include "wifi_manager.h"
#include <HTTPClient.h>

// ========== HELPER: ESCAPE JSON STRING ==========
static String escapeJson(const String& input) {
  String output;
  output.reserve(input.length() + 20);
  for (unsigned int i = 0; i < input.length(); i++) {
    char c = input[i];
    switch (c) {
      case '"':  output += "\\\""; break;
      case '\\': output += "\\\\"; break;
      case '\n': output += "\\n"; break;
      case '\r': output += "\\r"; break;
      case '\t': output += "\\t"; break;
      default:   output += c; break;
    }
  }
  return output;
}

// ========== BATCH AI REQUEST ==========
bool AIProvider::sendBatchToAI(const String& base64Image, DynamicJsonDocument& results) {
  if (!WiFiManager::isConnected()) return false;

  // 1. Construire le prompt global
  String prompt = "Analyze this image and answer the following questions in strict JSON format. ";
  prompt += "Do NOT use markdown code blocks (like ```json). Just return the raw JSON object.\n\n";
  prompt += "Questions:\n";
  
  bool hasQuestions = false;
  for (int i = 0; i < cfg.questionsCount; i++) {
    if (!cfg.questions[i].enabled) continue;
    hasQuestions = true;
    String typeStr = (cfg.questions[i].type == QUESTION_BOOL) ? "boolean (true/false)" : "integer number";
    prompt += "- key \"" + cfg.questions[i].jsonKey + "\" (" + typeStr + "): " + cfg.questions[i].prompt + "\n";
  }

  if (!hasQuestions) return false;
  
  prompt += "\nExpected JSON format:\n{\n";
  bool first = true;
  for (int i = 0; i < cfg.questionsCount; i++) {
    if (!cfg.questions[i].enabled) continue;
    if (!first) prompt += ",\n";
    prompt += "  \"" + cfg.questions[i].jsonKey + "\": " + (cfg.questions[i].type == QUESTION_BOOL ? "true" : "5");
    first = false;
  }
  prompt += "\n}";

  String responseContent;
  HTTPClient http;
  http.setTimeout(120000); // 2 minutes timeout

  // 2. Envoyer la requête selon le provider
  if (cfg.provider == PROVIDER_OPENAI || cfg.provider == PROVIDER_LMSTUDIO) {
    String url = (cfg.provider == PROVIDER_OPENAI) 
      ? "https://api.openai.com/v1/chat/completions" 
      : cfg.lm_host + "/v1/chat/completions";
      
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    if (cfg.provider == PROVIDER_OPENAI) {
      http.addHeader("Authorization", "Bearer " + cfg.openai_key);
    }

    // Construire le JSON manuellement pour éviter les limites de buffer ArduinoJson
    // L'image base64 peut faire 50-100KB, trop gros pour DynamicJsonDocument
    String payload = "{";
    payload += "\"model\":\"" + cfg.lm_model + "\",";
    payload += "\"max_tokens\":300,";
    payload += "\"temperature\":0.1,";
    payload += "\"messages\":[{";
    payload += "\"role\":\"user\",";
    payload += "\"content\":[";
    payload += "{\"type\":\"text\",\"text\":\"" + escapeJson(prompt) + "\"},";
    payload += "{\"type\":\"image_url\",\"image_url\":{\"url\":\"data:image/jpeg;base64," + base64Image + "\"}}";
    payload += "]}]}";

    int httpCode = http.POST(payload);
    if (httpCode == HTTP_CODE_OK) {
      String httpResponse = http.getString();
      DynamicJsonDocument respDoc(4096);
      deserializeJson(respDoc, httpResponse);
      responseContent = respDoc["choices"][0]["message"]["content"].as<String>();
    } else {
      Serial.printf("HTTP Error: %d\n", httpCode);
      http.end();
      return false;
    }
    http.end();
  } 
  else if (cfg.provider == PROVIDER_OLLAMA) {
    String url = cfg.lm_host + "/api/generate";
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    // Pour Ollama, on construit le JSON manuellement pour éviter la duplication mémoire de l'image
    String payload = "{";
    payload += "\"model\":\"" + cfg.lm_model + "\",";
    payload += "\"prompt\":\"" + escapeJson(prompt) + "\",";
    payload += "\"images\":[\"" + base64Image + "\"],";
    payload += "\"stream\":false,";
    payload += "\"format\":\"json\""; // Ollama supporte le mode JSON natif
    payload += "}";

    int httpCode = http.POST(payload);
    if (httpCode == HTTP_CODE_OK) {
      String httpResponse = http.getString();
      DynamicJsonDocument respDoc(4096);
      deserializeJson(respDoc, httpResponse);
      responseContent = respDoc["response"].as<String>();
    } else {
      Serial.printf("HTTP Error: %d\n", httpCode);
      http.end();
      return false;
    }
    http.end();
  }

  // 3. Nettoyer la réponse (enlever les markdown ```json si présents)
  responseContent.trim();
  if (responseContent.startsWith("```json")) {
    responseContent = responseContent.substring(7);
    if (responseContent.endsWith("```")) {
      responseContent = responseContent.substring(0, responseContent.length() - 3);
    }
  } else if (responseContent.startsWith("```")) {
    responseContent = responseContent.substring(3);
    if (responseContent.endsWith("```")) {
      responseContent = responseContent.substring(0, responseContent.length() - 3);
    }
  }
  responseContent.trim();

  // 4. Parser le JSON final
  DeserializationError error = deserializeJson(results, responseContent);
  if (error) {
    Serial.print("JSON Batch Parse Error: ");
    Serial.println(error.c_str());
    Serial.println("Received: " + responseContent);
    return false;
  }

  return true;
}

String AIProvider::fetchModels(int providerOverride, const String& hostOverride) {
  if (!WiFiManager::isConnected()) return "[]";

  HTTPClient http;
  String url;
  String result = "[]";
  
  int provider = (providerOverride != -1) ? providerOverride : (int)cfg.provider;
  String host = (hostOverride.length() > 0) ? hostOverride : cfg.lm_host;

  if (provider == PROVIDER_LMSTUDIO) {
    // LM Studio
    url = host + "/v1/models";
    http.begin(url);
    http.setTimeout(10000);

    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();
      DynamicJsonDocument doc(8192);
      if (deserializeJson(doc, response) == DeserializationError::Ok) {
        DynamicJsonDocument resultDoc(4096);
        JsonArray arr = resultDoc.to<JsonArray>();
        JsonArray data = doc["data"];
        for (JsonObject model : data) {
          arr.add(model["id"].as<String>());
        }
        serializeJson(resultDoc, result);
      }
    }
    http.end();

  } else if (provider == PROVIDER_OLLAMA) {
    // Ollama
    url = host + "/api/tags";
    http.begin(url);
    http.setTimeout(10000);

    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();
      DynamicJsonDocument doc(8192);
      if (deserializeJson(doc, response) == DeserializationError::Ok) {
        DynamicJsonDocument resultDoc(4096);
        JsonArray arr = resultDoc.to<JsonArray>();
        JsonArray models = doc["models"];
        for (JsonObject model : models) {
          arr.add(model["name"].as<String>());
        }
        serializeJson(resultDoc, result);
      }
    }
    http.end();

  } else if (provider == PROVIDER_OPENAI) {
    // OpenAI - Liste statique des modèles vision
    result = "[\"gpt-4.1-nano\",\"gpt-4.1-mini\",\"gpt-4.1\"]";
  }

  return result;
}

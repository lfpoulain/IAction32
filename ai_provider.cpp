/*
 * Implémentation des providers IA (LM Studio, Ollama, OpenAI)
 */

#include "ai_provider.h"
#include "wifi_manager.h"
#include "logger.h"
#include <HTTPClient.h>

// ========== HELPER: ESCAPE JSON STRING ==========
static void escapeJsonTo(String& out, const String& in) {
  out.reserve(out.length() + in.length() + 20);
  for (unsigned int i = 0; i < in.length(); i++) {
    char c = in[i];
    if (c == '"') out += F("\\\"");
    else if (c == '\\') out += F("\\\\");
    else if (c == '\n') out += F("\\n");
    else if (c == '\r') out += F("\\r");
    else if (c == '\t') out += F("\\t");
    else if (c < 32) continue; // Skip control chars
    else out += c;
  }
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

  // 2. Construire l'URL selon le provider (tous utilisent l'API OpenAI-compatible)
  String url;
  if (cfg.provider == PROVIDER_OPENAI) {
    url = "https://api.openai.com/v1/chat/completions";
  } else {
    url = cfg.lm_host + "/v1/chat/completions";
  }

  // 3. Configurer HTTP
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  http.setTimeout(30000); // 30s pour tous les providers
  
  if (cfg.provider == PROVIDER_OPENAI) {
    http.addHeader("Authorization", "Bearer " + cfg.openai_key);
  }

  // 4. Construire le payload JSON (format OpenAI pour tous)
  String payload;
  payload.reserve(base64Image.length() + prompt.length() + 300);
  payload = F("{\"model\":\"");
  payload += cfg.lm_model;
  payload += F("\",\"max_tokens\":300,\"temperature\":0.1,\"messages\":[{\"role\":\"user\",\"content\":[{\"type\":\"text\",\"text\":\"");
  escapeJsonTo(payload, prompt);
  payload += F("\"},{\"type\":\"image_url\",\"image_url\":{\"url\":\"data:image/jpeg;base64,");
  payload += base64Image;
  payload += F("\"}}]}]}");

  // 5. Envoyer la requete
  Logger::printf("AI request to %s...", url.c_str());
  int httpCode = http.POST(payload);
  
  String responseContent;
  if (httpCode == HTTP_CODE_OK) {
    String httpResponse = http.getString();
    
    StaticJsonDocument<200> filter;
    filter["choices"][0]["message"]["content"] = true;
    filter["error"] = true;
    
    DynamicJsonDocument respDoc(4096);
    DeserializationError error = deserializeJson(respDoc, httpResponse, DeserializationOption::Filter(filter));
    
    if (error) {
      Logger::printf("JSON parse error: %s", error.c_str());
      http.end();
      return false;
    }
    
    // Verifier erreur API
    if (respDoc.containsKey("error")) {
      const char* errMsg = respDoc["error"]["message"] | "Unknown error";
      Logger::printf("API error: %s", errMsg);
      http.end();
      return false;
    }
    
    responseContent = respDoc["choices"][0]["message"]["content"].as<String>();
  } else {
    Logger::printf("HTTP error: %d", httpCode);
    http.end();
    return false;
  }
  http.end();

  // 6. Nettoyer la reponse (enlever les markdown ```json si presents)
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

  // 7. Parser le JSON final
  DeserializationError error = deserializeJson(results, responseContent);
  if (error) {
    Logger::printf("JSON Batch Parse Error: %s", error.c_str());
    Logger::log("Received: " + responseContent.substring(0, 100) + "...");
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
      
      // Filter for LM Studio models
      StaticJsonDocument<200> filter;
      filter["data"][0]["id"] = true;
      
      DynamicJsonDocument doc(8192);
      DeserializationError error = deserializeJson(doc, response, DeserializationOption::Filter(filter));
      
      if (error == DeserializationError::Ok) {
        DynamicJsonDocument resultDoc(4096);
        JsonArray arr = resultDoc.to<JsonArray>();
        JsonArray data = doc["data"];
        for (JsonObject model : data) {
          arr.add(model["id"].as<String>());
        }
        serializeJson(resultDoc, result);
      } else {
        Logger::printf("LMStudio Models Parse Error: %s", error.c_str());
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
      
      // Filter for Ollama tags
      StaticJsonDocument<200> filter;
      filter["models"][0]["name"] = true;
      
      DynamicJsonDocument doc(8192);
      DeserializationError error = deserializeJson(doc, response, DeserializationOption::Filter(filter));

      if (error == DeserializationError::Ok) {
        DynamicJsonDocument resultDoc(4096);
        JsonArray arr = resultDoc.to<JsonArray>();
        JsonArray models = doc["models"];
        for (JsonObject model : models) {
          arr.add(model["name"].as<String>());
        }
        serializeJson(resultDoc, result);
      } else {
        Logger::printf("Ollama Models Parse Error: %s", error.c_str());
      }
    }
    http.end();

  } else if (provider == PROVIDER_OPENAI) {
    // OpenAI - Liste statique des modèles vision
    result = "[\"gpt-4.1-nano\",\"gpt-4.1-mini\",\"gpt-4.1\"]";
  }

  return result;
}

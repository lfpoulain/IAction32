/*
 * Implémentation du stockage persistant
 */

#include "storage.h"
#include "esp_camera.h"

static Preferences prefs;

void Storage::init() {
  prefs.begin("iaction32", false);
}

void Storage::load() {
  Serial.println("📂 Chargement de la configuration...");

  // WiFi
  cfg.wifi_ssid = prefs.getString("wifi_ssid", DEFAULT_WIFI_SSID);
  cfg.wifi_password = prefs.getString("wifi_pass", DEFAULT_WIFI_PASSWORD);

  // AI Provider
  cfg.provider = (AIProviderType)prefs.getInt("provider", PROVIDER_LMSTUDIO);
  cfg.lm_host = prefs.getString("lm_host", DEFAULT_LM_HOST);
  cfg.lm_model = prefs.getString("lm_model", DEFAULT_LM_MODEL);
  cfg.openai_key = prefs.getString("openai_key", "");

  // MQTT
  cfg.mqtt_server = prefs.getString("mqtt_srv", DEFAULT_MQTT_SERVER);
  cfg.mqtt_port = prefs.getInt("mqtt_port", DEFAULT_MQTT_PORT);
  cfg.mqtt_user = prefs.getString("mqtt_user", DEFAULT_MQTT_USER);
  cfg.mqtt_password = prefs.getString("mqtt_pass", DEFAULT_MQTT_PASSWORD);
  cfg.mqtt_topic = prefs.getString("mqtt_topic", DEFAULT_MQTT_TOPIC);
  cfg.device_name = prefs.getString("dev_name", DEFAULT_DEVICE_NAME);
  cfg.mqtt_enabled = prefs.getBool("mqtt_en", true);

  // Mode
  cfg.capture_enabled = prefs.getBool("cap_enabled", false);
  cfg.capture_mode_live = prefs.getBool("cap_live", true);  // Par défaut: Live
  cfg.interval_seconds = prefs.getInt("interval", 30);      // Par défaut: 30s

  // Camera - Résolutions 16:9
  cfg.camera.framesize = prefs.getInt("cam_res", FRAMESIZE_HD);
  cfg.camera.quality = prefs.getInt("cam_qual", 12);
  cfg.camera.brightness = prefs.getInt("cam_bright", 0);
  cfg.camera.contrast = prefs.getInt("cam_contr", 0);
  cfg.camera.saturation = prefs.getInt("cam_sat", 0);
  cfg.camera.sharpness = prefs.getInt("cam_sharp", 0);
  cfg.camera.hmirror = prefs.getBool("cam_hmirr", false);
  cfg.camera.vflip = prefs.getBool("cam_vflip", false);
  cfg.camera.awb = prefs.getBool("cam_awb", true);
  cfg.camera.aec = prefs.getBool("cam_aec", true);
  cfg.camera.aec_value = prefs.getInt("cam_aecv", 300);
  cfg.camera.agc = prefs.getBool("cam_agc", true);
  cfg.camera.agc_gain = prefs.getInt("cam_agcg", 0);

  // Questions
  cfg.questionsCount = prefs.getInt("q_count", 0);
  for (int i = 0; i < cfg.questionsCount && i < MAX_QUESTIONS; i++) {
    String prefix = "q" + String(i) + "_";
    cfg.questions[i].prompt = prefs.getString((prefix + "prompt").c_str(), "");
    cfg.questions[i].type = (QuestionType)prefs.getInt((prefix + "type").c_str(), QUESTION_BOOL);
    cfg.questions[i].jsonKey = prefs.getString((prefix + "key").c_str(), "q" + String(i));
    cfg.questions[i].enabled = prefs.getBool((prefix + "enabled").c_str(), true);
  }

  // Créer question par défaut si aucune
  if (cfg.questionsCount == 0) {
    cfg.questions[0].prompt = DEFAULT_PROMPT;
    cfg.questions[0].type = QUESTION_BOOL;
    cfg.questions[0].jsonKey = "person_detected";
    cfg.questions[0].enabled = true;
    cfg.questionsCount = 1;
  }

  Serial.println("✓ Configuration chargée");
}

void Storage::save() {
  Serial.println("💾 Sauvegarde de la configuration...");

  // WiFi
  prefs.putString("wifi_ssid", cfg.wifi_ssid);
  prefs.putString("wifi_pass", cfg.wifi_password);

  // AI Provider
  prefs.putInt("provider", (int)cfg.provider);
  prefs.putString("lm_host", cfg.lm_host);
  prefs.putString("lm_model", cfg.lm_model);
  prefs.putString("openai_key", cfg.openai_key);

  // MQTT
  prefs.putString("mqtt_srv", cfg.mqtt_server);
  prefs.putInt("mqtt_port", cfg.mqtt_port);
  prefs.putString("mqtt_user", cfg.mqtt_user);
  prefs.putString("mqtt_pass", cfg.mqtt_password);
  prefs.putString("mqtt_topic", cfg.mqtt_topic);
  prefs.putString("dev_name", cfg.device_name);
  prefs.putBool("mqtt_en", cfg.mqtt_enabled);

  // Mode
  prefs.putBool("cap_enabled", cfg.capture_enabled);
  prefs.putBool("cap_live", cfg.capture_mode_live);
  prefs.putInt("interval", cfg.interval_seconds);

  // Camera
  prefs.putInt("cam_res", cfg.camera.framesize);
  prefs.putInt("cam_qual", cfg.camera.quality);
  prefs.putInt("cam_bright", cfg.camera.brightness);
  prefs.putInt("cam_contr", cfg.camera.contrast);
  prefs.putInt("cam_sat", cfg.camera.saturation);
  prefs.putInt("cam_sharp", cfg.camera.sharpness);
  prefs.putBool("cam_hmirr", cfg.camera.hmirror);
  prefs.putBool("cam_vflip", cfg.camera.vflip);
  prefs.putBool("cam_awb", cfg.camera.awb);
  prefs.putBool("cam_aec", cfg.camera.aec);
  prefs.putInt("cam_aecv", cfg.camera.aec_value);
  prefs.putBool("cam_agc", cfg.camera.agc);
  prefs.putInt("cam_agcg", cfg.camera.agc_gain);

  // Questions
  prefs.putInt("q_count", cfg.questionsCount);
  for (int i = 0; i < cfg.questionsCount; i++) {
    String prefix = "q" + String(i) + "_";
    prefs.putString((prefix + "prompt").c_str(), cfg.questions[i].prompt);
    prefs.putInt((prefix + "type").c_str(), (int)cfg.questions[i].type);
    prefs.putString((prefix + "key").c_str(), cfg.questions[i].jsonKey);
    prefs.putBool((prefix + "enabled").c_str(), cfg.questions[i].enabled);
  }

  Serial.println("✓ Configuration sauvegardée");
}

void Storage::reset() {
  prefs.clear();
  Serial.println("✓ Configuration réinitialisée");
}

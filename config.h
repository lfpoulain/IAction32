/*
 * Configuration globale et structures de données
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ========== SÉLECTION DU BOARD ==========
#define CAMERA_MODEL_WROVER_KIT
// #define CAMERA_MODEL_AI_THINKER

// ========== PINS CONFIGURATION ==========
#if defined(CAMERA_MODEL_WROVER_KIT)
  #define PWDN_GPIO_NUM    -1
  #define RESET_GPIO_NUM   -1
  #define XCLK_GPIO_NUM    21
  #define SIOD_GPIO_NUM    26
  #define SIOC_GPIO_NUM    27
  #define Y9_GPIO_NUM      35
  #define Y8_GPIO_NUM      34
  #define Y7_GPIO_NUM      39
  #define Y6_GPIO_NUM      36
  #define Y5_GPIO_NUM      19
  #define Y4_GPIO_NUM      18
  #define Y3_GPIO_NUM       5
  #define Y2_GPIO_NUM       4
  #define VSYNC_GPIO_NUM   25
  #define HREF_GPIO_NUM    23
  #define PCLK_GPIO_NUM    22
  #define FLASH_GPIO_NUM   -1
#elif defined(CAMERA_MODEL_AI_THINKER)
  #define PWDN_GPIO_NUM    32
  #define RESET_GPIO_NUM   -1
  #define XCLK_GPIO_NUM     0
  #define SIOD_GPIO_NUM    26
  #define SIOC_GPIO_NUM    27
  #define Y9_GPIO_NUM      35
  #define Y8_GPIO_NUM      34
  #define Y7_GPIO_NUM      39
  #define Y6_GPIO_NUM      36
  #define Y5_GPIO_NUM      21
  #define Y4_GPIO_NUM      19
  #define Y3_GPIO_NUM      18
  #define Y2_GPIO_NUM       5
  #define VSYNC_GPIO_NUM   25
  #define HREF_GPIO_NUM    23
  #define PCLK_GPIO_NUM    22
  #define FLASH_GPIO_NUM    4
#endif

// ========== CONSTANTES ==========
#define MAX_QUESTIONS          10
#define AP_SSID               "ESP32-CAM-Config"
#define AP_PASSWORD           "12345678"

// ========== VALEURS PAR DÉFAUT ==========
#define DEFAULT_WIFI_SSID      "YourWiFiSSID"
#define DEFAULT_WIFI_PASSWORD  "YourWiFiPassword"
#define DEFAULT_LM_HOST        "http://192.168.1.100:1234"
#define DEFAULT_LM_MODEL       "qwen2.5-vl-7b-instruct"
#define DEFAULT_PROMPT         "Is there a person visible in this image?"
#define DEFAULT_MQTT_SERVER    "192.168.1.100"
#define DEFAULT_MQTT_PORT      1883
#define DEFAULT_MQTT_USER      ""
#define DEFAULT_MQTT_PASSWORD  ""
#define DEFAULT_MQTT_TOPIC     "iaction32"
#define DEFAULT_DEVICE_NAME    "IAction32"

// ========== ÉNUMÉRATIONS ==========
enum AIProviderType {
  PROVIDER_LMSTUDIO = 0,
  PROVIDER_OLLAMA = 1,
  PROVIDER_OPENAI = 2
};

enum QuestionType {
  QUESTION_BOOL = 0,
  QUESTION_DECIMAL = 1
};

// ========== STRUCTURES ==========
struct Question {
  String prompt;
  QuestionType type;
  String jsonKey;
  bool enabled;
};

struct CameraConfig {
  int framesize;
  int quality;
  int brightness;
  int contrast;
  int saturation;
  int sharpness;
  bool hmirror;
  bool vflip;
  bool awb;
  bool aec;
  int aec_value;
  bool agc;
  int agc_gain;
  int gainceiling;
  int bpc;
  int wpc;
  int raw_gma;
  int lenc;
  int dcw;
  int special_effect;
  int wb_mode;
  int ae_level;
  bool aec2;
  bool colorbar;
};

struct AppConfig {
  // WiFi
  String wifi_ssid;
  String wifi_password;

  // AI Provider
  AIProviderType provider;
  String lm_host;
  String lm_model;
  String openai_key;

  // MQTT
  String mqtt_server;
  int mqtt_port;
  String mqtt_user;
  String mqtt_password;
  String mqtt_topic;
  String device_name;
  bool mqtt_enabled;

  // Mode
  bool capture_enabled;  // ON/OFF global
  bool capture_mode_live; // true = Live, false = Intervalle
  int interval_seconds;  // Intervalle en secondes (utilisé si capture_mode_live = false)

  // Camera
  CameraConfig camera;

  // Questions
  Question questions[MAX_QUESTIONS];
  int questionsCount;
};

struct AppStats {
  int captureCount;
  int errorCount;
  String lastResult;
  unsigned long lastCaptureTimestamp;
  bool captureInProgress;
};

// ========== VARIABLES GLOBALES ==========
extern AppConfig cfg;
extern AppStats stats;

#endif

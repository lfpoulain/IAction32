/*
 * Implémentation de la gestion caméra
 */

#include "camera_setup.h"
#include "esp_camera.h"
#include "config.h"
#include "base64.h"

void Camera::init() {
  Serial.println("📷 Initialisation de la caméra...");

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_LATEST;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_SVGA; // Forcer 800x600
    config.jpeg_quality = cfg.camera.quality;
    config.fb_count = 2;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    Serial.println("  ✓ PSRAM détecté - Résolution forcée à SVGA (800x600)");
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 15;
    config.fb_count = 1;
    config.fb_location = CAMERA_FB_IN_DRAM;
    Serial.println("  ⚠ PSRAM non détecté");
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("  ✗ Erreur init caméra: 0x%x\n", err);
    return;
  }

  Serial.println("✓ Caméra initialisée");
}

void Camera::applySettings() {
  sensor_t* s = esp_camera_sensor_get();
  if (!s) {
    Serial.println("✗ Erreur: capteur non disponible");
    return;
  }

  s->set_framesize(s, FRAMESIZE_SVGA); // Forcer 800x600
  s->set_quality(s, cfg.camera.quality);
  s->set_brightness(s, cfg.camera.brightness);
  s->set_contrast(s, cfg.camera.contrast);
  s->set_saturation(s, cfg.camera.saturation);
  s->set_sharpness(s, cfg.camera.sharpness);
  s->set_hmirror(s, cfg.camera.hmirror ? 1 : 0);
  s->set_vflip(s, cfg.camera.vflip ? 1 : 0);
  s->set_whitebal(s, cfg.camera.awb ? 1 : 0);
  s->set_awb_gain(s, cfg.camera.awb ? 1 : 0);
  s->set_exposure_ctrl(s, cfg.camera.aec ? 1 : 0);
  if (!cfg.camera.aec) s->set_aec_value(s, cfg.camera.aec_value);
  s->set_gain_ctrl(s, cfg.camera.agc ? 1 : 0);
  if (!cfg.camera.agc) s->set_agc_gain(s, cfg.camera.agc_gain);
  s->set_gainceiling(s, (gainceiling_t)cfg.camera.gainceiling);
  s->set_bpc(s, cfg.camera.bpc);
  s->set_wpc(s, cfg.camera.wpc);
  s->set_raw_gma(s, cfg.camera.raw_gma);
  s->set_lenc(s, cfg.camera.lenc);
  s->set_dcw(s, cfg.camera.dcw);
  s->set_special_effect(s, cfg.camera.special_effect);
  s->set_wb_mode(s, cfg.camera.wb_mode);
  s->set_ae_level(s, cfg.camera.ae_level);
  s->set_aec2(s, cfg.camera.aec2 ? 1 : 0);
  s->set_colorbar(s, cfg.camera.colorbar ? 1 : 0);

  Serial.println("✓ Paramètres caméra appliqués");
}

String Camera::captureBase64() {
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("✗ Erreur capture image");
    return "";
  }

  String base64Image = base64::encode(fb->buf, fb->len);
  esp_camera_fb_return(fb);
  return base64Image;
}

/*
 * Implementation des pages HTML
 */

#include "web_pages.h"
#include "config.h"
#include "wifi_manager.h"

// ========== DASHBOARD ==========
String buildDashboardPage() {
  String html = "<div id='dashboard' class='tab-content active'>";

  // Stats
  html += "<div class='card'>";
  html += "<div class='card-header'>";
  html += "<div class='card-title'><div class='card-icon'>&#128202;</div> Statistiques</div>";
  html += "</div>";

  html += "<div class='grid grid-4'>";
  html += "<div class='stat-card'>";
  html += "<div class='stat-value' id='stat-captures'>" + String(stats.captureCount) + "</div>";
  html += "<div class='stat-label'>Captures</div>";
  html += "</div>";

  html += "<div class='stat-card error'>";
  html += "<div class='stat-value' id='stat-errors'>" + String(stats.errorCount) + "</div>";
  html += "<div class='stat-label'>Erreurs</div>";
  html += "</div>";
  html += "</div>";

  html += "<div class='grid grid-3' style='margin-top:1.5rem;'>";
  html += "<div class='stat-card'>";
  html += "<div class='stat-label'>Adresse IP</div>";
  html += "<div class='stat-value' style='font-size:1.2rem;' id='stat-ip'>" + WiFiManager::getIP() + "</div>";
  html += "</div>";

  html += "<div class='stat-card'>";
  html += "<div class='stat-label'>Dernier resultat</div>";
  html += "<div class='stat-value' style='font-size:0.9rem;' id='stat-last'>" + stats.lastResult + "</div>";
  html += "</div>";
  html += "</div>";

  // Bouton ON/OFF
  html += "<div style='margin-top:1.5rem;'>";
  html += "<label class='form-label' style='font-weight:600;'>Capture</label>";
  html += "<div class='mode-buttons'>";
  html += "<button type='button' class='mode-btn" + String(!cfg.capture_enabled ? " active" : "") + "' id='capture-off' onclick='toggleCapture(false)'>Off</button>";
  html += "<button type='button' class='mode-btn" + String(cfg.capture_enabled ? " active" : "") + "' id='capture-on' onclick='toggleCapture(true)'>On</button>";
  html += "</div>";
  html += "</div>";

  // Type de capture
  html += "<div style='margin-top:1.5rem;'>";
  html += "<label class='form-label' style='font-weight:600;'>Mode</label>";
  html += "<div class='mode-buttons'>";
  html += "<button type='button' class='mode-btn" + String(cfg.capture_mode_live ? " active" : "") + "' id='mode-live' onclick='setCaptureMode(true)'>Live (1s)</button>";
  html += "<button type='button' class='mode-btn" + String(!cfg.capture_mode_live ? " active" : "") + "' id='mode-interval' onclick='setCaptureMode(false)'>Intervalle</button>";
  html += "</div>";
  html += "<div id='interval-config' style='margin-top:1rem;display:" + String(!cfg.capture_mode_live ? "block" : "none") + ";'>";
  html += "<label class='form-label'>Intervalle (secondes)</label>";
  html += "<div class='input-group'>";
  html += "<input type='number' id='interval-input' class='form-input' value='" + String(cfg.interval_seconds) + "' min='5' max='3600'>";
  html += "<button type='button' class='btn btn-primary btn-small' onclick='saveInterval()'>Sauvegarder</button>";
  html += "</div>";
  html += "</div>";
  html += "</div>";

  html += "</div>";

  // Preview & Test
  html += "<div class='card'>";
  html += "<div class='card-header'>";
  html += "<div class='card-title'><div class='card-icon'>&#128247;</div> Apercu & Test</div>";
  html += "</div>";

  html += "<div class='preview-container'>";
  html += "<img id='preview-img' class='preview-img' src='/capture' alt='Preview'>";
  html += "</div>";

  html += "<div class='btn-group' style='margin-top:1.5rem;'>";
  html += "<button class='btn btn-secondary' onclick='refreshPreview()'>Rafraichir</button>";
  html += "<button class='btn btn-primary' id='test-btn' onclick='testCapture()'>Tester</button>";
  html += "</div>";

  html += "<div id='test-result' style='margin-top:1.5rem;'></div>";

  html += "</div>";

  html += "</div>";
  return html;
}

// ========== CONFIGURATION ==========
String buildConfigPage() {
  String html = "<div id='config' class='tab-content'>";

  // WiFi
  html += "<div class='card'>";
  html += "<div class='card-header'>";
  html += "<div class='card-title'><div class='card-icon'>&#128225;</div> WiFi</div>";
  html += "</div>";

  html += "<form action='/save_wifi' method='POST'>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>SSID WiFi</label>";
  html += "<div class='input-group'>";
  html += "<select id='ssid-select' name='wifi_ssid' class='form-select'>";
  html += "<option value='" + cfg.wifi_ssid + "'>" + cfg.wifi_ssid + "</option>";
  html += "</select>";
  html += "<button type='button' id='refresh-networks-btn' class='btn btn-secondary btn-small' onclick='refreshNetworks()'>Scanner</button>";
  html += "</div>";
  html += "</div>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Mot de passe WiFi</label>";
  html += "<input type='password' name='wifi_pass' class='form-input' value='" + cfg.wifi_password + "'>";
  html += "</div>";

  html += "<button type='submit' class='btn btn-primary'>Sauvegarder WiFi</button>";
  html += "</form>";
  html += "</div>";

  // IA Provider
  html += "<div class='card'>";
  html += "<div class='card-header'>";
  html += "<div class='card-title'><div class='card-icon'>&#129302;</div> Configuration IA</div>";
  html += "</div>";

  html += "<form action='/save_ai' method='POST'>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Provider IA</label>";
  html += "<input type='hidden' name='provider' id='provider-input' value='" + String(cfg.provider) + "'>";
  html += "<div class='mode-buttons'>";
  html += "<button type='button' class='mode-btn" + String(cfg.provider == PROVIDER_LMSTUDIO ? " active" : "") + "' id='provider-0' onclick='setProvider(0)'>LM Studio</button>";
  html += "<button type='button' class='mode-btn" + String(cfg.provider == PROVIDER_OLLAMA ? " active" : "") + "' id='provider-1' onclick='setProvider(1)'>Ollama</button>";
  html += "<button type='button' class='mode-btn" + String(cfg.provider == PROVIDER_OPENAI ? " active" : "") + "' id='provider-2' onclick='setProvider(2)'>OpenAI</button>";
  html += "</div>";
  html += "</div>";

  // LM Studio Config
  html += "<div id='lmstudio-config' style='display:" + String(cfg.provider == PROVIDER_LMSTUDIO ? "block" : "none") + ";'>";
  html += "<div class='alert alert-info'>LM Studio - Serveur local</div>";
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Serveur LM Studio</label>";
  html += "<input type='text' name='lm_host' id='input-lm-host' class='form-input' value='" + cfg.lm_host + "' placeholder='http://10.0.0.10:1234'>";
  html += "</div>";
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Modele</label>";
  html += "<div class='input-group'>";
  html += "<select id='model-select-lmstudio' name='lm_model' class='form-select'>";
  html += "<option value='" + cfg.lm_model + "'>" + cfg.lm_model + "</option>";
  html += "</select>";
  html += "<button type='button' id='refresh-btn-lmstudio' class='btn btn-secondary btn-small' onclick='refreshModels(0)'>Rafraichir</button>";
  html += "</div>";
  html += "</div>";
  html += "</div>";

  // Ollama Config
  html += "<div id='ollama-config' style='display:" + String(cfg.provider == PROVIDER_OLLAMA ? "block" : "none") + ";'>";
  html += "<div class='alert alert-info'>Ollama - Serveur local</div>";
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Serveur Ollama</label>";
  html += "<input type='text' name='lm_host' id='input-ollama-host' class='form-input' value='" + cfg.lm_host + "' placeholder='http://10.0.0.10:11434'>";
  html += "</div>";
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Modele</label>";
  html += "<div class='input-group'>";
  html += "<select id='model-select-ollama' name='lm_model' class='form-select'>";
  html += "<option value='" + cfg.lm_model + "'>" + cfg.lm_model + "</option>";
  html += "</select>";
  html += "<button type='button' id='refresh-btn-ollama' class='btn btn-secondary btn-small' onclick='refreshModels(1)'>Rafraichir</button>";
  html += "</div>";
  html += "</div>";
  html += "</div>";

  // OpenAI Config
  html += "<div id='openai-config' style='display:" + String(cfg.provider == PROVIDER_OPENAI ? "block" : "none") + ";'>";
  html += "<div class='alert alert-info'>OpenAI - API cloud GPT-4.1</div>";
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Cle API OpenAI</label>";
  html += "<input type='password' name='openai_key' id='input-openai-key' class='form-input' value='" + cfg.openai_key + "' placeholder='sk-...'>";
  html += "</div>";
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Modele</label>";
  html += "<select id='model-select-openai' name='lm_model' class='form-select'>";
  html += "<option value='gpt-4.1-nano'" + String(cfg.lm_model == "gpt-4.1-nano" ? " selected" : "") + ">gpt-4.1-nano</option>";
  html += "<option value='gpt-4.1-mini'" + String(cfg.lm_model == "gpt-4.1-mini" ? " selected" : "") + ">gpt-4.1-mini</option>";
  html += "<option value='gpt-4.1'" + String(cfg.lm_model == "gpt-4.1" ? " selected" : "") + ">gpt-4.1</option>";
  html += "</select>";
  html += "</div>";
  html += "</div>";

  html += "<button type='submit' class='btn btn-primary'>Sauvegarder IA</button>";
  html += "</form>";
  html += "</div>";

  // MQTT Configuration
  html += "<div class='card'>";
  html += "<div class='card-header'>";
  html += "<div class='card-title'><div class='card-icon'>&#128225;</div> MQTT / Home Assistant</div>";
  html += "</div>";

  html += "<div class='alert alert-info'>";
  html += "MQTT Auto-Discovery: Les capteurs seront detectes par Home Assistant.";
  html += "</div>";

  html += "<form action='/save_mqtt' method='POST'>";

  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>Activer MQTT</span>";
  html += "<label class='toggle'><input type='checkbox' name='mqtt_enabled' value='1'" + String(cfg.mqtt_enabled ? " checked" : "") + "><span class='toggle-slider'></span></label>";
  html += "</div>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Serveur MQTT</label>";
  html += "<input type='text' name='mqtt_server' class='form-input' value='" + cfg.mqtt_server + "' placeholder='10.0.0.5'>";
  html += "</div>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Port MQTT</label>";
  html += "<input type='number' name='mqtt_port' class='form-input' value='" + String(cfg.mqtt_port) + "' placeholder='1883'>";
  html += "</div>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Utilisateur MQTT</label>";
  html += "<input type='text' name='mqtt_user' class='form-input' value='" + cfg.mqtt_user + "'>";
  html += "</div>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Mot de passe MQTT</label>";
  html += "<input type='password' name='mqtt_pass' class='form-input' value='" + cfg.mqtt_password + "'>";
  html += "</div>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Topic de base</label>";
  html += "<input type='text' name='mqtt_topic' class='form-input' value='" + cfg.mqtt_topic + "'>";
  html += "</div>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Nom appareil</label>";
  html += "<input type='text' name='device_name' class='form-input' value='" + cfg.device_name + "'>";
  html += "</div>";

  html += "<button type='submit' class='btn btn-primary'>Sauvegarder MQTT</button>";
  html += "</form>";
  html += "</div>";

  // OTA Update
  html += "<div class='card'>";
  html += "<div class='card-header'>";
  html += "<div class='card-title'><div class='card-icon'>&#128260;</div> Mise a jour Firmware</div>";
  html += "</div>";
  html += "<div class='alert alert-info'>Fichier .bin pour ESP32 Wrover Module.</div>";
  html += "<form method='POST' action='/update' enctype='multipart/form-data'>";
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Fichier Firmware (.bin)</label>";
  html += "<input type='file' name='update' class='form-input' accept='.bin'>";
  html += "</div>";
  html += "<div id='update-progress' style='display:none;margin-top:1rem;'>";
  html += "<div style='background:rgba(0,0,0,.4);border-radius:8px;height:24px;overflow:hidden;'>";
  html += "<div id='update-bar' style='height:100%;width:0%;background:linear-gradient(90deg,var(--ab),var(--ag));transition:width .3s;'></div>";
  html += "</div>";
  html += "<div id='update-pct' style='text-align:center;margin-top:.5rem;font-weight:600;'>0%</div>";
  html += "</div>";
  html += "<button type='submit' class='btn btn-danger'>Mettre a jour</button>";
  html += "</form>";
  html += "</div>";

  html += "</div>";
  return html;
}

// ========== QUESTIONS ==========
String buildQuestionsPage() {
  String html = "<div id='questions' class='tab-content'>";

  html += "<div class='card'>";
  html += "<div class='card-header'>";
  html += "<div class='card-title'><div class='card-icon'>&#10067;</div> Questions IA</div>";
  html += "</div>";

  html += "<div class='alert alert-info'>";
  html += "Configurez les questions pour l'IA. Reponses publiees via MQTT.";
  html += "</div>";

  // Liste des questions
  for (int i = 0; i < cfg.questionsCount; i++) {
    html += "<div class='question-item'>";
    html += "<div class='question-header'>";
    html += "<span style='font-weight:600;'>Question " + String(i + 1) + "</span>";
    html += "<span class='badge " + String(cfg.questions[i].type == QUESTION_BOOL ? "badge-bool" : "badge-decimal") + "'>";
    html += cfg.questions[i].type == QUESTION_BOOL ? "Bool" : "Compteur";
    html += "</span>";
    html += "</div>";

    html += "<form action='/save_question' method='POST'>";
    html += "<input type='hidden' name='id' value='" + String(i) + "'>";

    html += "<div class='form-group'>";
    html += "<label class='form-label'>Prompt</label>";
    html += "<textarea name='prompt' class='form-textarea'>" + cfg.questions[i].prompt + "</textarea>";
    html += "</div>";

    html += "<div class='form-group'>";
    html += "<label class='form-label'>Type de reponse</label>";
    html += "<input type='hidden' name='type' id='type-" + String(i) + "' value='" + String(cfg.questions[i].type) + "'>";
    html += "<div class='mode-buttons'>";
    html += "<button type='button' class='mode-btn" + String(cfg.questions[i].type == QUESTION_BOOL ? " active" : "") + "' onclick='setQuestionType(" + String(i) + ", 0, this)'>Bool</button>";
    html += "<button type='button' class='mode-btn" + String(cfg.questions[i].type == QUESTION_DECIMAL ? " active" : "") + "' onclick='setQuestionType(" + String(i) + ", 1, this)'>Compteur</button>";
    html += "</div>";
    html += "</div>";

    html += "<div class='form-group'>";
    html += "<label class='form-label'>Cle MQTT</label>";
    html += "<input type='text' name='key' class='form-input' value='" + cfg.questions[i].jsonKey + "'>";
    html += "</div>";

    html += "<div class='btn-group'>";
    html += "<button type='submit' class='btn btn-primary btn-small'>Sauvegarder</button>";
    html += "<button type='button' class='btn btn-danger btn-small' onclick=\"if(confirm('Supprimer?'))location.href='/delete_question?id=" + String(i) + "'\">Supprimer</button>";
    html += "</div>";

    html += "</form>";
    html += "</div>";
  }

  // Bouton ajouter
  if (cfg.questionsCount < MAX_QUESTIONS) {
    html += "<button class='btn btn-success' style='margin-top:1rem;' onclick=\"location.href='/add_question'\">+ Ajouter</button>";
  } else {
    html += "<div class='alert alert-error'>Maximum atteint (" + String(MAX_QUESTIONS) + ")</div>";
  }

  html += "</div>";
  html += "</div>";
  return html;
}

// ========== CAMERA ==========
String buildCameraPage() {
  String html = "<div id='camera' class='tab-content'>";

  // Live Preview Card
  html += "<div class='card'>";
  html += "<div class='card-header'>";
  html += "<div class='card-title'><div class='card-icon'>&#127909;</div> Apercu</div>";
  html += "</div>";
  html += "<div style='text-align:center;padding:1rem;'>";
  html += "<img id='camera-preview' src='/capture?t=" + String(millis()) + "' style='max-width:100%;border-radius:8px;'>";
  html += "<div style='margin-top:1rem;'>";
  html += "<button type='button' class='btn btn-primary btn-small' onclick='refreshCameraPreview()'>Rafraichir</button>";
  html += "<button type='button' class='btn btn-secondary btn-small' id='auto-refresh-btn' onclick='toggleAutoRefresh()'>Auto</button>";
  html += "</div>";
  html += "</div>";
  html += "</div>";

  html += "<div class='card'>";
  html += "<div class='card-header'>";
  html += "<div class='card-title'><div class='card-icon'>&#9881;</div> Reglages Camera</div>";
  html += "</div>";

  html += "<form action='/save_camera' method='POST'>";

  // Resolution
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Resolution</label>";
  html += "<input type='text' class='form-input' value='800x600 (SVGA)' disabled style='background:#1a1f3c;color:#888;'>";
  html += "<input type='hidden' name='framesize' value='9'>";
  html += "</div>";

  // Qualite JPEG
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Qualite JPEG: <span id='quality-val'>" + String(cfg.camera.quality) + "</span></label>";
  html += "<input type='range' name='quality' class='form-input' min='4' max='63' value='" + String(cfg.camera.quality) + "' oninput=\"document.getElementById('quality-val').textContent=this.value\">";
  html += "</div>";

  // Luminosite
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Luminosite: <span id='bright-val'>" + String(cfg.camera.brightness) + "</span></label>";
  html += "<input type='range' name='brightness' class='form-input' min='-2' max='2' value='" + String(cfg.camera.brightness) + "' oninput=\"document.getElementById('bright-val').textContent=this.value\">";
  html += "</div>";

  // Contraste
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Contraste: <span id='contr-val'>" + String(cfg.camera.contrast) + "</span></label>";
  html += "<input type='range' name='contrast' class='form-input' min='-2' max='2' value='" + String(cfg.camera.contrast) + "' oninput=\"document.getElementById('contr-val').textContent=this.value\">";
  html += "</div>";

  // Saturation
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Saturation: <span id='sat-val'>" + String(cfg.camera.saturation) + "</span></label>";
  html += "<input type='range' name='saturation' class='form-input' min='-2' max='2' value='" + String(cfg.camera.saturation) + "' oninput=\"document.getElementById('sat-val').textContent=this.value\">";
  html += "</div>";

  // Nettete
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Nettete: <span id='sharp-val'>" + String(cfg.camera.sharpness) + "</span></label>";
  html += "<input type='range' name='sharpness' class='form-input' min='-2' max='2' value='" + String(cfg.camera.sharpness) + "' oninput=\"document.getElementById('sharp-val').textContent=this.value\">";
  html += "</div>";

  // Exposition & Gain
  html += "<div class='form-group' style='margin-top:1.5rem;border-top:1px solid #333;padding-top:1rem;'>";
  
  // AEC
  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>Exposition Auto (AEC)</span>";
  html += "<label class='toggle'><input type='checkbox' name='aec' value='1'" + String(cfg.camera.aec ? " checked" : "") + " onchange=\"document.getElementById('aec-manual').style.display=this.checked?'none':'block'\"><span class='toggle-slider'></span></label>";
  html += "</div>";
  
  html += "<div id='aec-manual' class='form-group' style='display:" + String(cfg.camera.aec ? "none" : "block") + ";'>";
  html += "<label class='form-label'>Exposition (0-1200): <span id='aec-val'>" + String(cfg.camera.aec_value) + "</span></label>";
  html += "<input type='range' name='aec_value' class='form-input' min='0' max='1200' value='" + String(cfg.camera.aec_value) + "' oninput=\"document.getElementById('aec-val').textContent=this.value\">";
  html += "</div>";

  // AGC
  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>Gain Auto (AGC)</span>";
  html += "<label class='toggle'><input type='checkbox' name='agc' value='1'" + String(cfg.camera.agc ? " checked" : "") + " onchange=\"document.getElementById('agc-manual').style.display=this.checked?'none':'block'\"><span class='toggle-slider'></span></label>";
  html += "</div>";

  html += "<div id='agc-manual' class='form-group' style='display:" + String(cfg.camera.agc ? "none" : "block") + ";'>";
  html += "<label class='form-label'>Gain (0-30): <span id='agc-val'>" + String(cfg.camera.agc_gain) + "</span></label>";
  html += "<input type='range' name='agc_gain' class='form-input' min='0' max='30' value='" + String(cfg.camera.agc_gain) + "' oninput=\"document.getElementById('agc-val').textContent=this.value\">";
  html += "</div>";

  // Gain Ceiling
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Plafond Gain</label>";
  html += "<select name='gainceiling' class='form-select'>";
  html += "<option value='0'" + String(cfg.camera.gainceiling == 0 ? " selected" : "") + ">2x</option>";
  html += "<option value='1'" + String(cfg.camera.gainceiling == 1 ? " selected" : "") + ">4x</option>";
  html += "<option value='2'" + String(cfg.camera.gainceiling == 2 ? " selected" : "") + ">8x</option>";
  html += "<option value='3'" + String(cfg.camera.gainceiling == 3 ? " selected" : "") + ">16x</option>";
  html += "<option value='4'" + String(cfg.camera.gainceiling == 4 ? " selected" : "") + ">32x</option>";
  html += "<option value='5'" + String(cfg.camera.gainceiling == 5 ? " selected" : "") + ">64x</option>";
  html += "<option value='6'" + String(cfg.camera.gainceiling == 6 ? " selected" : "") + ">128x</option>";
  html += "</select>";
  html += "</div>";

  // AEC2
  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>AEC DSP</span>";
  html += "<label class='toggle'><input type='checkbox' name='aec2' value='1'" + String(cfg.camera.aec2 ? " checked" : "") + "><span class='toggle-slider'></span></label>";
  html += "</div>";
  
  html += "</div>";

  // Toggles
  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>Miroir horizontal</span>";
  html += "<label class='toggle'><input type='checkbox' name='hmirror' value='1'" + String(cfg.camera.hmirror ? " checked" : "") + "><span class='toggle-slider'></span></label>";
  html += "</div>";

  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>Flip vertical</span>";
  html += "<label class='toggle'><input type='checkbox' name='vflip' value='1'" + String(cfg.camera.vflip ? " checked" : "") + "><span class='toggle-slider'></span></label>";
  html += "</div>";

  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>Balance blancs auto (AWB)</span>";
  html += "<label class='toggle'><input type='checkbox' name='awb' value='1'" + String(cfg.camera.awb ? " checked" : "") + "><span class='toggle-slider'></span></label>";
  html += "</div>";

  // Reglages Avances
  html += "<div class='form-group' style='margin-top:1.5rem;border-top:1px solid #333;padding-top:1rem;'>";
  html += "<label class='form-label'>Effet Special</label>";
  html += "<select name='special_effect' class='form-select'>";
  html += "<option value='0'" + String(cfg.camera.special_effect == 0 ? " selected" : "") + ">Aucun</option>";
  html += "<option value='1'" + String(cfg.camera.special_effect == 1 ? " selected" : "") + ">Negatif</option>";
  html += "<option value='2'" + String(cfg.camera.special_effect == 2 ? " selected" : "") + ">N&B</option>";
  html += "<option value='3'" + String(cfg.camera.special_effect == 3 ? " selected" : "") + ">Rouge</option>";
  html += "<option value='4'" + String(cfg.camera.special_effect == 4 ? " selected" : "") + ">Vert</option>";
  html += "<option value='5'" + String(cfg.camera.special_effect == 5 ? " selected" : "") + ">Bleu</option>";
  html += "<option value='6'" + String(cfg.camera.special_effect == 6 ? " selected" : "") + ">Sepia</option>";
  html += "</select>";
  html += "</div>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Mode WB</label>";
  html += "<select name='wb_mode' class='form-select'>";
  html += "<option value='0'" + String(cfg.camera.wb_mode == 0 ? " selected" : "") + ">Auto</option>";
  html += "<option value='1'" + String(cfg.camera.wb_mode == 1 ? " selected" : "") + ">Ensoleille</option>";
  html += "<option value='2'" + String(cfg.camera.wb_mode == 2 ? " selected" : "") + ">Nuageux</option>";
  html += "<option value='3'" + String(cfg.camera.wb_mode == 3 ? " selected" : "") + ">Bureau</option>";
  html += "<option value='4'" + String(cfg.camera.wb_mode == 4 ? " selected" : "") + ">Maison</option>";
  html += "</select>";
  html += "</div>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>AE Level: <span id='ael-val'>" + String(cfg.camera.ae_level) + "</span></label>";
  html += "<input type='range' name='ae_level' class='form-input' min='-2' max='2' value='" + String(cfg.camera.ae_level) + "' oninput=\"document.getElementById('ael-val').textContent=this.value\">";
  html += "</div>";

  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>Lens Correction (LENC)</span>";
  html += "<label class='toggle'><input type='checkbox' name='lenc' value='1'" + String(cfg.camera.lenc ? " checked" : "") + "><span class='toggle-slider'></span></label>";
  html += "</div>";

  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>White Pixel Correction (WPC)</span>";
  html += "<label class='toggle'><input type='checkbox' name='wpc' value='1'" + String(cfg.camera.wpc ? " checked" : "") + "><span class='toggle-slider'></span></label>";
  html += "</div>";

  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>Gamma Correction (GMA)</span>";
  html += "<label class='toggle'><input type='checkbox' name='raw_gma' value='1'" + String(cfg.camera.raw_gma ? " checked" : "") + "><span class='toggle-slider'></span></label>";
  html += "</div>";

  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>Black Pixel Correction (BPC)</span>";
  html += "<label class='toggle'><input type='checkbox' name='bpc' value='1'" + String(cfg.camera.bpc ? " checked" : "") + "><span class='toggle-slider'></span></label>";
  html += "</div>";

  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>Downsize EN (DCW)</span>";
  html += "<label class='toggle'><input type='checkbox' name='dcw' value='1'" + String(cfg.camera.dcw ? " checked" : "") + "><span class='toggle-slider'></span></label>";
  html += "</div>";

  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>Colorbar (Test)</span>";
  html += "<label class='toggle'><input type='checkbox' name='colorbar' value='1'" + String(cfg.camera.colorbar ? " checked" : "") + "><span class='toggle-slider'></span></label>";
  html += "</div>";

  html += "<div class='btn-group' style='margin-top:2rem;'>";
  html += "<button type='submit' class='btn btn-primary'>Appliquer</button>";
  html += "<button type='button' class='btn btn-danger' onclick=\"if(confirm('Redemarrer?'))location.href='/reboot'\">Redemarrer</button>";
  html += "</div>";

  html += "</form>";
  html += "</div>";

  html += "</div>";
  return html;
}

// ========== LOGS ==========
String buildLogsPage() {
  String html = "<div id='logs' class='tab-content'>";
  
  html += "<div class='card'>";
  html += "<div class='card-header'>";
  html += "<div class='card-title'><div class='card-icon'>&#128220;</div> Logs Systeme</div>";
  html += "<div style='display:flex;gap:10px;'>";
  html += "<button class='btn btn-secondary btn-small' onclick='refreshLogs()'>Actualiser</button>";
  html += "<button class='btn btn-danger btn-small' onclick='clearLogs()'>Effacer</button>";
  html += "</div>";
  html += "</div>";
  
  html += "<div id='log-container' style='background:#000;color:#0f0;font-family:monospace;padding:1rem;border-radius:8px;height:400px;overflow-y:auto;font-size:0.9rem;'>";
  html += "<div style='text-align:center;color:#888;'>Chargement...</div>";
  html += "</div>";
  
  html += "</div>";
  html += "</div>";
  return html;
}

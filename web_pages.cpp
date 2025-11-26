/*
 * Implémentation des pages HTML
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
  html += "<div class='card-title'><div class='card-icon'>📊</div> Statistiques en temps réel</div>";
  html += "</div>";

  html += "<div class='grid grid-4'>";
  html += "<div class='stat-card'>";
  html += "<div class='stat-value' id='stat-captures'>" + String(stats.captureCount) + "</div>";
  html += "<div class='stat-label'>Captures totales</div>";
  html += "</div>";

  html += "<div class='stat-card'>";
  html += "<div class='stat-value' id='stat-success'>" + String(stats.successCount) + "</div>";
  html += "<div class='stat-label'>Succès</div>";
  html += "</div>";

  html += "<div class='stat-card error'>";
  html += "<div class='stat-value' id='stat-errors'>" + String(stats.errorCount) + "</div>";
  html += "<div class='stat-label'>Erreurs</div>";
  html += "</div>";

  html += "<div class='stat-card'>";
  html += "<div class='stat-value' id='stat-wifi'>" + String(WiFiManager::isConnected() ? "✓" : "✗") + "</div>";
  html += "<div class='stat-label'>WiFi</div>";
  html += "</div>";
  html += "</div>";

  html += "<div class='grid grid-3' style='margin-top:1.5rem;'>";
  html += "<div class='stat-card'>";
  html += "<div class='stat-label'>Adresse IP</div>";
  html += "<div class='stat-value' style='font-size:1.2rem;' id='stat-ip'>" + WiFiManager::getIP() + "</div>";
  html += "</div>";

  html += "<div class='stat-card'>";
  html += "<div class='stat-label'>Dernière mise à jour</div>";
  html += "<div class='stat-value' style='font-size:0.9rem;' id='stat-last-update'>Jamais</div>";
  html += "</div>";

  html += "<div class='stat-card'>";
  html += "<div class='stat-label'>Dernier résultat</div>";
  html += "<div class='stat-value' style='font-size:0.9rem;' id='stat-last'>" + stats.lastResult + "</div>";
  html += "</div>";
  html += "</div>";

  // Bouton ON/OFF
  html += "<div style='margin-top:1.5rem;'>";
  html += "<label class='form-label' style='font-weight:600;'>🔌 Capture</label>";
  html += "<div class='mode-buttons'>";
  html += "<button type='button' class='mode-btn" + String(!cfg.capture_enabled ? " active" : "") + "' id='capture-off' onclick='toggleCapture(false)'>❌ Off</button>";
  html += "<button type='button' class='mode-btn" + String(cfg.capture_enabled ? " active" : "") + "' id='capture-on' onclick='toggleCapture(true)'>✅ On</button>";
  html += "</div>";
  html += "</div>";

  // Type de capture
  html += "<div style='margin-top:1.5rem;'>";
  html += "<label class='form-label' style='font-weight:600;'>⚡ Type de Capture</label>";
  html += "<div class='mode-buttons'>";
  html += "<button type='button' class='mode-btn" + String(cfg.capture_mode_live ? " active" : "") + "' id='mode-live' onclick='setCaptureMode(true)'>⚡ Live</button>";
  html += "<button type='button' class='mode-btn" + String(!cfg.capture_mode_live ? " active" : "") + "' id='mode-interval' onclick='setCaptureMode(false)'>⏱️ Intervalle</button>";
  html += "</div>";
  html += "<div id='interval-config' style='margin-top:1rem;display:" + String(!cfg.capture_mode_live ? "block" : "none") + ";'>";
  html += "<label class='form-label'>Intervalle (secondes)</label>";
  html += "<div class='input-group'>";
  html += "<input type='number' id='interval-input' class='form-input' value='" + String(cfg.interval_seconds) + "' min='5' max='3600'>";
  html += "<button type='button' class='btn btn-primary btn-small' onclick='saveInterval()'>💾 Sauvegarder</button>";
  html += "</div>";
  html += "</div>";
  html += "</div>";

  html += "</div>";

  // Preview & Test
  html += "<div class='card'>";
  html += "<div class='card-header'>";
  html += "<div class='card-title'><div class='card-icon'>📸</div> Aperçu & Test</div>";
  html += "</div>";

  html += "<div class='preview-container'>";
  html += "<img id='preview-img' class='preview-img' src='/capture' alt='Preview'>";
  html += "</div>";

  html += "<div class='btn-group' style='margin-top:1.5rem;'>";
  html += "<button class='btn btn-secondary' onclick='refreshPreview()'>🔄 Rafraîchir l'image</button>";
  html += "<button class='btn btn-primary' id='test-btn' onclick='testCapture()'>🚀 Tester maintenant</button>";
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
  html += "<div class='card-title'><div class='card-icon'>📡</div> Configuration WiFi</div>";
  html += "</div>";

  html += "<form action='/save_wifi' method='POST'>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>SSID WiFi</label>";
  html += "<div class='input-group'>";
  html += "<select id='ssid-select' name='wifi_ssid' class='form-select'>";
  html += "<option value='" + cfg.wifi_ssid + "'>" + cfg.wifi_ssid + "</option>";
  html += "</select>";
  html += "<button type='button' id='refresh-networks-btn' class='btn btn-secondary btn-small' onclick='refreshNetworks()'>📡 Scanner</button>";
  html += "</div>";
  html += "</div>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Mot de passe WiFi</label>";
  html += "<input type='password' name='wifi_pass' class='form-input' value='" + cfg.wifi_password + "'>";
  html += "</div>";

  html += "<button type='submit' class='btn btn-primary'>💾 Sauvegarder WiFi</button>";
  html += "</form>";
  html += "</div>";

  // IA Provider
  html += "<div class='card'>";
  html += "<div class='card-header'>";
  html += "<div class='card-title'><div class='card-icon'>🤖</div> Configuration IA</div>";
  html += "</div>";

  html += "<form action='/save_ai' method='POST'>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Provider IA</label>";
  html += "<input type='hidden' name='provider' id='provider-input' value='" + String(cfg.provider) + "'>";
  html += "<div class='mode-buttons'>";
  html += "<button type='button' class='mode-btn" + String(cfg.provider == PROVIDER_LMSTUDIO ? " active" : "") + "' id='provider-0' onclick='setProvider(0)'>🖥️ LM Studio</button>";
  html += "<button type='button' class='mode-btn" + String(cfg.provider == PROVIDER_OLLAMA ? " active" : "") + "' id='provider-1' onclick='setProvider(1)'>🦙 Ollama</button>";
  html += "<button type='button' class='mode-btn" + String(cfg.provider == PROVIDER_OPENAI ? " active" : "") + "' id='provider-2' onclick='setProvider(2)'>🌐 OpenAI</button>";
  html += "</div>";
  html += "</div>";

  // LM Studio Config
  html += "<div id='lmstudio-config' style='display:" + String(cfg.provider == PROVIDER_LMSTUDIO ? "block" : "none") + ";'>";
  html += "<div class='alert alert-info'>🖥️ LM Studio - Serveur local pour modèles vision</div>";
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Serveur LM Studio</label>";
  html += "<input type='text' name='lm_host' id='input-lm-host' class='form-input' value='" + cfg.lm_host + "' placeholder='http://10.0.0.10:1234'>";
  html += "</div>";
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Modèle</label>";
  html += "<div class='input-group'>";
  html += "<select id='model-select-lmstudio' name='lm_model' class='form-select'>";
  html += "<option value='" + cfg.lm_model + "'>" + cfg.lm_model + "</option>";
  html += "</select>";
  html += "<button type='button' id='refresh-btn-lmstudio' class='btn btn-secondary btn-small' onclick='refreshModels(0)'>🔄 Rafraîchir</button>";
  html += "</div>";
  html += "</div>";
  html += "</div>";

  // Ollama Config
  html += "<div id='ollama-config' style='display:" + String(cfg.provider == PROVIDER_OLLAMA ? "block" : "none") + ";'>";
  html += "<div class='alert alert-info'>🦙 Ollama - Serveur local pour modèles vision</div>";
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Serveur Ollama</label>";
  html += "<input type='text' name='lm_host' id='input-ollama-host' class='form-input' value='" + cfg.lm_host + "' placeholder='http://10.0.0.10:11434'>";
  html += "</div>";
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Modèle</label>";
  html += "<div class='input-group'>";
  html += "<select id='model-select-ollama' name='lm_model' class='form-select'>";
  html += "<option value='" + cfg.lm_model + "'>" + cfg.lm_model + "</option>";
  html += "</select>";
  html += "<button type='button' id='refresh-btn-ollama' class='btn btn-secondary btn-small' onclick='refreshModels(1)'>🔄 Rafraîchir</button>";
  html += "</div>";
  html += "</div>";
  html += "</div>";

  // OpenAI Config
  html += "<div id='openai-config' style='display:" + String(cfg.provider == PROVIDER_OPENAI ? "block" : "none") + ";'>";
  html += "<div class='alert alert-info'>🌐 OpenAI - API cloud avec modèles GPT-4.1</div>";
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Clé API OpenAI</label>";
  html += "<input type='password' name='openai_key' id='input-openai-key' class='form-input' value='" + cfg.openai_key + "' placeholder='sk-...'>";
  html += "</div>";
  html += "<div class='form-group'>";
  html += "<label class='form-label'>Modèle</label>";
  html += "<select id='model-select-openai' name='lm_model' class='form-select'>";
  html += "<option value='gpt-4.1-nano'" + String(cfg.lm_model == "gpt-4.1-nano" ? " selected" : "") + ">gpt-4.1-nano (Économique)</option>";
  html += "<option value='gpt-4.1-mini'" + String(cfg.lm_model == "gpt-4.1-mini" ? " selected" : "") + ">gpt-4.1-mini (Équilibré)</option>";
  html += "<option value='gpt-4.1'" + String(cfg.lm_model == "gpt-4.1" ? " selected" : "") + ">gpt-4.1 (Performant)</option>";
  html += "</select>";
  html += "</div>";
  html += "</div>";

  html += "<button type='submit' class='btn btn-primary'>💾 Sauvegarder IA</button>";
  html += "</form>";
  html += "</div>";

  // MQTT Configuration
  html += "<div class='card'>";
  html += "<div class='card-header'>";
  html += "<div class='card-title'><div class='card-icon'>📡</div> Configuration MQTT / Home Assistant</div>";
  html += "</div>";

  html += "<div class='alert alert-info'>";
  html += "💡 <strong>MQTT Auto-Discovery:</strong> Les capteurs seront automatiquement détectés par Home Assistant via MQTT Discovery.";
  html += "</div>";

  html += "<form action='/save_mqtt' method='POST'>";

  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>Activer MQTT</span>";
  html += "<label class='toggle'><input type='checkbox' name='mqtt_enabled' value='1'" + String(cfg.mqtt_enabled ? " checked" : "") + "><span class='toggle-slider'></span></label>";
  html += "</div>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Serveur MQTT (IP ou hostname)</label>";
  html += "<input type='text' name='mqtt_server' class='form-input' value='" + cfg.mqtt_server + "' placeholder='10.0.0.5'>";
  html += "</div>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Port MQTT</label>";
  html += "<input type='number' name='mqtt_port' class='form-input' value='" + String(cfg.mqtt_port) + "' placeholder='1883'>";
  html += "</div>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Utilisateur MQTT (optionnel)</label>";
  html += "<input type='text' name='mqtt_user' class='form-input' value='" + cfg.mqtt_user + "' placeholder='homeassistant'>";
  html += "</div>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Mot de passe MQTT (optionnel)</label>";
  html += "<input type='password' name='mqtt_pass' class='form-input' value='" + cfg.mqtt_password + "'>";
  html += "</div>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Topic de base</label>";
  html += "<input type='text' name='mqtt_topic' class='form-input' value='" + cfg.mqtt_topic + "' placeholder='esp32cam'>";
  html += "</div>";

  html += "<div class='form-group'>";
  html += "<label class='form-label'>Nom de l'appareil (Home Assistant)</label>";
  html += "<input type='text' name='device_name' class='form-input' value='" + cfg.device_name + "' placeholder='ESP32-CAM Vision Pro'>";
  html += "</div>";

  html += "<div class='alert alert-success'>";
  html += "✅ <strong>Topics publiés:</strong><br>";
  html += "- <code>" + cfg.mqtt_topic + "/[device_id]/[question_key]</code> : État de chaque capteur<br>";
  html += "- <code>" + cfg.mqtt_topic + "/[device_id]/state</code> : État JSON complet<br>";
  html += "- <code>" + cfg.mqtt_topic + "/[device_id]/availability</code> : État de disponibilité";
  html += "</div>";

  html += "<button type='submit' class='btn btn-primary'>💾 Sauvegarder MQTT</button>";
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
  html += "<div class='card-title'><div class='card-icon'>❓</div> Questions pour l'IA</div>";
  html += "</div>";

  html += "<div class='alert alert-info'>";
  html += "💡 <strong>Info:</strong> Configurez les questions à poser à l'IA. Les réponses seront publiées via MQTT vers Home Assistant avec auto-discovery.";
  html += "</div>";

  // Liste des questions
  for (int i = 0; i < cfg.questionsCount; i++) {
    html += "<div class='question-item'>";
    html += "<div class='question-header'>";
    html += "<span style='font-weight:600;'>📝 Question " + String(i + 1) + "</span>";
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
    html += "<label class='form-label'>Type de réponse</label>";
    html += "<input type='hidden' name='type' id='type-" + String(i) + "' value='" + String(cfg.questions[i].type) + "'>";
    html += "<div class='mode-buttons'>";
    html += "<button type='button' class='mode-btn" + String(cfg.questions[i].type == QUESTION_BOOL ? " active" : "") + "' onclick='setQuestionType(" + String(i) + ", 0, this)'>✓ Bool</button>";
    html += "<button type='button' class='mode-btn" + String(cfg.questions[i].type == QUESTION_DECIMAL ? " active" : "") + "' onclick='setQuestionType(" + String(i) + ", 1, this)'>🔢 Compteur</button>";
    html += "</div>";
    html += "</div>";

    html += "<div class='form-group'>";
    html += "<label class='form-label'>Clé MQTT (nom du capteur)</label>";
    html += "<input type='text' name='key' class='form-input' value='" + cfg.questions[i].jsonKey + "' placeholder='ex: person_detected, car_count'>";
    html += "</div>";

    html += "<div class='btn-group'>";
    html += "<button type='submit' class='btn btn-primary btn-small'>💾 Sauvegarder</button>";
    html += "<button type='button' class='btn btn-danger btn-small' onclick=\"if(confirm('Supprimer cette question?'))location.href='/delete_question?id=" + String(i) + "'\">🗑️ Supprimer</button>";
    html += "</div>";

    html += "</form>";
    html += "</div>";
  }

  // Bouton ajouter
  if (cfg.questionsCount < MAX_QUESTIONS) {
    html += "<button class='btn btn-success' style='margin-top:1rem;' onclick=\"location.href='/add_question'\">➕ Ajouter une question</button>";
  } else {
    html += "<div class='alert alert-error'>⚠️ Nombre maximum de questions atteint (" + String(MAX_QUESTIONS) + ")</div>";
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
  html += "<div class='card-title'><div class='card-icon'>🎥</div> Aperçu en Direct</div>";
  html += "</div>";
  html += "<div style='text-align:center;padding:1rem;'>";
  html += "<img id='camera-preview' src='/capture?t=" + String(millis()) + "' style='max-width:100%;border-radius:8px;box-shadow:0 4px 6px rgba(0,0,0,0.3);' onerror='this.src=\"data:image/svg+xml,%3Csvg xmlns=\\'http://www.w3.org/2000/svg\\' width=\\'640\\' height=\\'480\\'%3E%3Crect fill=\\'%230a0e27\\' width=\\'640\\' height=\\'480\\'/%3E%3Ctext fill=\\'%23fff\\' x=\\'50%25\\' y=\\'50%25\\' text-anchor=\\'middle\\' dy=\\'.3em\\' font-family=\\'sans-serif\\'%3EErreur caméra%3C/text%3E%3C/svg%3E\"'>";
  html += "<div style='margin-top:1rem;'>";
  html += "<button type='button' class='btn btn-primary btn-small' onclick='refreshCameraPreview()'>🔄 Rafraîchir</button>";
  html += "<button type='button' class='btn btn-secondary btn-small' id='auto-refresh-btn' onclick='toggleAutoRefresh()'>▶️ Auto-Refresh</button>";
  html += "</div>";
  html += "</div>";
  html += "</div>";

  html += "<div class='card'>";
  html += "<div class='card-header'>";
  html += "<div class='card-title'><div class='card-icon'>⚙️</div> Réglages Caméra</div>";
  html += "</div>";

  html += "<form action='/save_camera' method='POST'>";

  // Résolution
  html += "<div class='form-group'>";
  html += "<label class='form-label'>📐 Résolution (16:9 uniquement)</label>";
  html += "<select name='framesize' class='form-select'>";
  html += "<option value='3'" + String(cfg.camera.framesize == 3 ? " selected" : "") + ">240x176 (HQVGA)</option>";
  html += "<option value='7'" + String(cfg.camera.framesize == 7 ? " selected" : "") + ">480x320 (HVGA)</option>";
  html += "<option value='11'" + String(cfg.camera.framesize == 11 ? " selected" : "") + ">1280x720 (HD) ⭐</option>";
  html += "</select>";
  html += "</div>";

  // Qualité JPEG
  html += "<div class='form-group'>";
  html += "<label class='form-label'>🎨 Qualité JPEG : <span id='quality-val'>" + String(cfg.camera.quality) + "</span> (4=max, 63=min)</label>";
  html += "<input type='range' name='quality' class='form-input' min='4' max='63' value='" + String(cfg.camera.quality) + "' oninput=\"document.getElementById('quality-val').textContent=this.value\">";
  html += "</div>";

  // Luminosité
  html += "<div class='form-group'>";
  html += "<label class='form-label'>☀️ Luminosité : <span id='bright-val'>" + String(cfg.camera.brightness) + "</span></label>";
  html += "<input type='range' name='brightness' class='form-input' min='-2' max='2' value='" + String(cfg.camera.brightness) + "' oninput=\"document.getElementById('bright-val').textContent=this.value\">";
  html += "</div>";

  // Contraste
  html += "<div class='form-group'>";
  html += "<label class='form-label'>🔲 Contraste : <span id='contr-val'>" + String(cfg.camera.contrast) + "</span></label>";
  html += "<input type='range' name='contrast' class='form-input' min='-2' max='2' value='" + String(cfg.camera.contrast) + "' oninput=\"document.getElementById('contr-val').textContent=this.value\">";
  html += "</div>";

  // Saturation
  html += "<div class='form-group'>";
  html += "<label class='form-label'>🌈 Saturation : <span id='sat-val'>" + String(cfg.camera.saturation) + "</span></label>";
  html += "<input type='range' name='saturation' class='form-input' min='-2' max='2' value='" + String(cfg.camera.saturation) + "' oninput=\"document.getElementById('sat-val').textContent=this.value\">";
  html += "</div>";

  // Toggles
  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>🔄 Miroir horizontal</span>";
  html += "<label class='toggle'><input type='checkbox' name='hmirror' value='1'" + String(cfg.camera.hmirror ? " checked" : "") + "><span class='toggle-slider'></span></label>";
  html += "</div>";

  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>↕️ Flip vertical</span>";
  html += "<label class='toggle'><input type='checkbox' name='vflip' value='1'" + String(cfg.camera.vflip ? " checked" : "") + "><span class='toggle-slider'></span></label>";
  html += "</div>";

  html += "<div class='toggle-group'>";
  html += "<span style='font-weight:600;'>⚪ Balance des blancs auto (AWB)</span>";
  html += "<label class='toggle'><input type='checkbox' name='awb' value='1'" + String(cfg.camera.awb ? " checked" : "") + "><span class='toggle-slider'></span></label>";
  html += "</div>";

  html += "<div class='btn-group' style='margin-top:2rem;'>";
  html += "<button type='submit' class='btn btn-primary'>💾 Appliquer les réglages</button>";
  html += "<button type='button' class='btn btn-danger' onclick=\"if(confirm('Redémarrer l\\'ESP32-CAM?'))location.href='/reboot'\">🔄 Redémarrer</button>";
  html += "</div>";

  html += "</form>";
  html += "</div>";

  html += "</div>";
  return html;
}

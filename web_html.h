/*
 * Interface Web HTML/CSS/JS avec menus modernes
 */

#ifndef WEB_HTML_H
#define WEB_HTML_H

#include <Arduino.h>

// Header HTML avec CSS moderne
const char HTML_HEAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>IAction32</title>
  <style>
    :root {
      --bg-dark: #0a0e27;
      --bg-darker: #060918;
      --bg-card: rgba(16, 22, 47, 0.8);
      --accent-blue: #00d9ff;
      --accent-green: #00ff88;
      --accent-purple: #b744ff;
      --text-primary: #ffffff;
      --text-secondary: #8892b0;
      --border-color: rgba(255, 255, 255, 0.1);
      --shadow: 0 10px 40px rgba(0, 0, 0, 0.5);
    }

    * { box-sizing: border-box; margin: 0; padding: 0; }

    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Inter', sans-serif;
      background: var(--bg-dark);
      background-image:
        radial-gradient(at 0% 0%, rgba(0, 217, 255, 0.1) 0, transparent 50%),
        radial-gradient(at 100% 100%, rgba(183, 68, 255, 0.1) 0, transparent 50%);
      min-height: 100vh;
      color: var(--text-primary);
      padding: 0;
      overflow-x: hidden;
    }

    /* ========== NAVIGATION ========== */
    .navbar {
      background: var(--bg-card);
      backdrop-filter: blur(20px);
      border-bottom: 1px solid var(--border-color);
      padding: 0;
      position: sticky;
      top: 0;
      z-index: 1000;
      box-shadow: var(--shadow);
    }

    .navbar-content {
      max-width: 1400px;
      margin: 0 auto;
      display: flex;
      align-items: center;
      justify-content: space-between;
      padding: 1rem 2rem;
    }

    .logo {
      font-size: 1.5rem;
      font-weight: 700;
      background: linear-gradient(135deg, var(--accent-blue), var(--accent-green));
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
      display: flex;
      align-items: center;
      gap: 10px;
    }

    .logo span {
      -webkit-text-fill-color: initial;
      font-size: 1.8rem;
    }

    .nav-menu {
      display: flex;
      gap: 0;
      list-style: none;
    }

    .nav-item {
      position: relative;
    }

    .nav-link {
      padding: 0.75rem 1.5rem;
      color: var(--text-secondary);
      text-decoration: none;
      transition: all 0.3s;
      display: block;
      border-radius: 8px;
      font-weight: 500;
    }

    .nav-link:hover, .nav-link.active {
      color: var(--accent-blue);
      background: rgba(0, 217, 255, 0.1);
    }

    .mobile-toggle {
      display: none;
      background: none;
      border: none;
      color: var(--text-primary);
      font-size: 1.5rem;
      cursor: pointer;
    }

    /* ========== CONTAINER ========== */
    .container {
      max-width: 1400px;
      margin: 2rem auto;
      padding: 0 2rem;
    }

    /* ========== CARDS ========== */
    .card {
      background: var(--bg-card);
      backdrop-filter: blur(20px);
      border-radius: 16px;
      padding: 2rem;
      margin-bottom: 2rem;
      border: 1px solid var(--border-color);
      box-shadow: var(--shadow);
      animation: fadeInUp 0.5s ease;
    }

    @keyframes fadeInUp {
      from { opacity: 0; transform: translateY(20px); }
      to { opacity: 1; transform: translateY(0); }
    }

    .card-header {
      display: flex;
      align-items: center;
      justify-content: space-between;
      margin-bottom: 1.5rem;
      padding-bottom: 1rem;
      border-bottom: 1px solid var(--border-color);
    }

    .card-title {
      font-size: 1.3rem;
      font-weight: 600;
      display: flex;
      align-items: center;
      gap: 12px;
      color: var(--text-primary);
    }

    .card-icon {
      width: 32px;
      height: 32px;
      display: flex;
      align-items: center;
      justify-content: center;
      background: linear-gradient(135deg, var(--accent-blue), var(--accent-purple));
      border-radius: 8px;
      font-size: 1.2rem;
    }

    /* ========== GRID ========== */
    .grid {
      display: grid;
      gap: 1.5rem;
    }

    .grid-2 { grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .grid-3 { grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); }
    .grid-4 { grid-template-columns: repeat(auto-fit, minmax(150px, 1fr)); }

    /* ========== STATS ========== */
    .stat-card {
      background: rgba(0, 0, 0, 0.3);
      padding: 1.5rem;
      border-radius: 12px;
      text-align: center;
      border: 1px solid var(--border-color);
      transition: transform 0.3s;
    }

    .stat-card:hover {
      transform: translateY(-5px);
      border-color: var(--accent-blue);
    }

    .stat-value {
      font-size: 2rem;
      font-weight: 700;
      background: linear-gradient(135deg, var(--accent-blue), var(--accent-green));
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
      margin-bottom: 0.5rem;
    }

    .stat-label {
      color: var(--text-secondary);
      font-size: 0.9rem;
      text-transform: uppercase;
      letter-spacing: 1px;
    }

    .stat-card.error .stat-value {
      background: linear-gradient(135deg, #ff4757, #ff6348);
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
    }

    /* ========== FORMS ========== */
    .form-group {
      margin-bottom: 1.5rem;
    }

    .form-label {
      display: block;
      margin-bottom: 0.5rem;
      color: var(--text-secondary);
      font-size: 0.9rem;
      font-weight: 500;
      text-transform: uppercase;
      letter-spacing: 0.5px;
    }

    .form-input, .form-select, .form-textarea {
      width: 100%;
      padding: 0.875rem 1rem;
      background: rgba(0, 0, 0, 0.4);
      border: 1px solid var(--border-color);
      border-radius: 8px;
      color: var(--text-primary);
      font-size: 1rem;
      transition: all 0.3s;
    }

    .form-input:focus, .form-select:focus, .form-textarea:focus {
      outline: none;
      border-color: var(--accent-blue);
      box-shadow: 0 0 0 3px rgba(0, 217, 255, 0.1);
    }

    .form-textarea {
      resize: vertical;
      min-height: 100px;
      font-family: inherit;
    }

    .input-group {
      display: flex;
      gap: 0.5rem;
    }

    .input-group .form-input,
    .input-group .form-select {
      flex: 1;
    }

    /* ========== BUTTONS ========== */
    .btn {
      padding: 0.875rem 1.5rem;
      border: none;
      border-radius: 8px;
      font-size: 1rem;
      font-weight: 600;
      cursor: pointer;
      transition: all 0.3s;
      display: inline-flex;
      align-items: center;
      justify-content: center;
      gap: 8px;
      text-decoration: none;
    }

    .btn:disabled {
      opacity: 0.5;
      cursor: not-allowed;
    }

    .btn-primary {
      background: linear-gradient(135deg, var(--accent-blue), var(--accent-green));
      color: var(--bg-dark);
      font-weight: 700;
    }

    .btn-primary:hover:not(:disabled) {
      transform: translateY(-2px);
      box-shadow: 0 10px 25px rgba(0, 217, 255, 0.3);
    }

    .btn-secondary {
      background: rgba(255, 255, 255, 0.1);
      color: var(--text-primary);
      border: 1px solid var(--border-color);
    }

    .btn-secondary:hover:not(:disabled) {
      background: rgba(255, 255, 255, 0.15);
      border-color: var(--accent-blue);
    }

    .btn-danger {
      background: linear-gradient(135deg, #ff4757, #ff6348);
      color: white;
    }

    .btn-success {
      background: linear-gradient(135deg, var(--accent-green), #00d9ff);
      color: var(--bg-dark);
      font-weight: 700;
    }

    .btn-small {
      padding: 0.5rem 1rem;
      font-size: 0.875rem;
    }

    .btn-group {
      display: flex;
      gap: 0.75rem;
      flex-wrap: wrap;
    }

    /* ========== MODE BUTTONS ========== */
    .mode-buttons {
      display: flex;
      gap: 0.5rem;
      flex-wrap: wrap;
    }

    .mode-btn {
      padding: 0.75rem 1.25rem;
      border: 2px solid var(--border-color);
      border-radius: 8px;
      background: rgba(0, 0, 0, 0.3);
      color: var(--text-secondary);
      font-size: 0.95rem;
      font-weight: 600;
      cursor: pointer;
      transition: all 0.3s;
    }

    .mode-btn:hover {
      border-color: var(--accent-blue);
      color: var(--text-primary);
    }

    .mode-btn.active {
      background: linear-gradient(135deg, var(--accent-blue), var(--accent-green));
      border-color: transparent;
      color: var(--bg-dark);
    }

    /* ========== TOGGLE SWITCH ========== */
    .toggle-group {
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 1rem;
      background: rgba(0, 0, 0, 0.2);
      border-radius: 8px;
      margin-bottom: 1rem;
    }

    .toggle {
      position: relative;
      width: 60px;
      height: 32px;
    }

    .toggle input {
      opacity: 0;
      width: 0;
      height: 0;
    }

    .toggle-slider {
      position: absolute;
      cursor: pointer;
      top: 0; left: 0; right: 0; bottom: 0;
      background: rgba(255, 255, 255, 0.2);
      border-radius: 32px;
      transition: 0.3s;
    }

    .toggle-slider::before {
      position: absolute;
      content: "";
      height: 24px;
      width: 24px;
      left: 4px;
      bottom: 4px;
      background: white;
      border-radius: 50%;
      transition: 0.3s;
    }

    .toggle input:checked + .toggle-slider {
      background: linear-gradient(135deg, var(--accent-blue), var(--accent-green));
    }

    .toggle input:checked + .toggle-slider::before {
      transform: translateX(28px);
    }

    /* ========== BADGES ========== */
    .badge {
      padding: 0.375rem 0.75rem;
      border-radius: 6px;
      font-size: 0.75rem;
      font-weight: 700;
      text-transform: uppercase;
      letter-spacing: 0.5px;
    }

    .badge-bool {
      background: var(--accent-blue);
      color: var(--bg-dark);
    }

    .badge-decimal {
      background: var(--accent-purple);
      color: white;
    }

    /* ========== ALERTS ========== */
    .alert {
      padding: 1rem 1.25rem;
      border-radius: 8px;
      margin-bottom: 1rem;
      border-left: 4px solid;
    }

    .alert-success {
      background: rgba(0, 255, 136, 0.1);
      border-color: var(--accent-green);
      color: var(--accent-green);
    }

    .alert-error {
      background: rgba(255, 71, 87, 0.1);
      border-color: #ff4757;
      color: #ff6b7a;
    }

    .alert-info {
      background: rgba(0, 217, 255, 0.1);
      border-color: var(--accent-blue);
      color: var(--accent-blue);
    }

    /* ========== PREVIEW ========== */
    .preview-container {
      text-align: center;
      margin: 1.5rem 0;
      position: relative;
    }

    .preview-img {
      max-width: 100%;
      border-radius: 12px;
      border: 2px solid var(--border-color);
      box-shadow: var(--shadow);
    }

    /* ========== QUESTION ITEMS ========== */
    .question-item {
      background: rgba(0, 0, 0, 0.3);
      padding: 1.5rem;
      border-radius: 12px;
      border: 1px solid var(--border-color);
      margin-bottom: 1rem;
      transition: all 0.3s;
    }

    .question-item:hover {
      border-color: var(--accent-blue);
      transform: translateX(5px);
    }

    .question-header {
      display: flex;
      justify-content: space-between;
      align-items: center;
      margin-bottom: 1rem;
    }

    /* ========== LOADING ========== */
    .loading {
      display: inline-block;
      width: 16px;
      height: 16px;
      border: 2px solid rgba(255, 255, 255, 0.3);
      border-top-color: var(--accent-blue);
      border-radius: 50%;
      animation: spin 0.8s linear infinite;
    }

    @keyframes spin {
      to { transform: rotate(360deg); }
    }

    /* ========== TABS ========== */
    .tabs {
      display: flex;
      gap: 0.5rem;
      margin-bottom: 1.5rem;
      border-bottom: 2px solid var(--border-color);
    }

    .tab {
      padding: 0.75rem 1.5rem;
      background: none;
      border: none;
      color: var(--text-secondary);
      cursor: pointer;
      font-size: 1rem;
      font-weight: 600;
      border-bottom: 3px solid transparent;
      transition: all 0.3s;
    }

    .tab:hover {
      color: var(--text-primary);
    }

    .tab.active {
      color: var(--accent-blue);
      border-bottom-color: var(--accent-blue);
    }

    .tab-content {
      display: none;
    }

    .tab-content.active {
      display: block;
      animation: fadeInUp 0.3s ease;
    }

    /* ========== RESPONSIVE ========== */
    @media (max-width: 768px) {
      .navbar-content {
        padding: 1rem;
      }

      .nav-menu {
        display: none;
        position: absolute;
        top: 100%;
        left: 0;
        right: 0;
        background: var(--bg-card);
        flex-direction: column;
        border-top: 1px solid var(--border-color);
      }

      .nav-menu.active {
        display: flex;
      }

      .mobile-toggle {
        display: block;
      }

      .container {
        padding: 0 1rem;
      }

      .card {
        padding: 1.5rem;
      }

      .btn-group {
        flex-direction: column;
      }

      .btn {
        width: 100%;
      }
    }

    /* ========== SCROLLBAR ========== */
    ::-webkit-scrollbar {
      width: 10px;
    }

    ::-webkit-scrollbar-track {
      background: var(--bg-darker);
    }

    ::-webkit-scrollbar-thumb {
      background: linear-gradient(180deg, var(--accent-blue), var(--accent-purple));
      border-radius: 5px;
    }
  </style>
</head>
<body>
  <nav class="navbar">
    <div class="navbar-content">
      <div class="logo">
        <span>🤖</span>
        IAction32
      </div>
      <button class="mobile-toggle" onclick="toggleMobileMenu()">☰</button>
      <ul class="nav-menu" id="navMenu">
        <li class="nav-item"><a href="#dashboard" class="nav-link active" onclick="showTab('dashboard')">Dashboard</a></li>
        <li class="nav-item"><a href="#config" class="nav-link" onclick="showTab('config')">Configuration</a></li>
        <li class="nav-item"><a href="#questions" class="nav-link" onclick="showTab('questions')">Questions</a></li>
        <li class="nav-item"><a href="#camera" class="nav-link" onclick="showTab('camera')">Caméra</a></li>
      </ul>
    </div>
  </nav>
  <div class="container">
)rawliteral";

const char HTML_FOOT[] PROGMEM = R"rawliteral(
  </div>
  <script>
    let liveMode = false;
    let capturingLive = false;
    let currentTab = 'dashboard';
    let intervalSeconds = 0;
    let intervalTimer = null;

    // ========== NAVIGATION ==========
    function showTab(tab) {
      currentTab = tab;
      document.querySelectorAll('.tab-content').forEach(el => el.classList.remove('active'));
      document.querySelectorAll('.nav-link').forEach(el => el.classList.remove('active'));

      document.getElementById(tab).classList.add('active');
      document.querySelector(`[onclick="showTab('${tab}')"]`).classList.add('active');
    }

    function toggleMobileMenu() {
      document.getElementById('navMenu').classList.toggle('active');
    }

    // ========== STATUS & AUTO-REFRESH ==========
    function refreshStatus() {
      fetch('/api/status')
        .then(r => r.json())
        .then(d => {
          document.getElementById('stat-captures').textContent = d.captures;
          document.getElementById('stat-success').textContent = d.success;
          document.getElementById('stat-errors').textContent = d.errors;
          document.getElementById('stat-last').textContent = d.lastResult;
          document.getElementById('stat-last-update').textContent = d.lastUpdate;
          
          // Mettre à jour les variables globales et l'UI
          captureEnabled = d.captureEnabled;
          captureModeLive = d.captureModeLive;
          updateCaptureButtons(captureEnabled, captureModeLive);
        })
        .catch(e => console.error('Status error:', e));
    }

    function updateCaptureButtons(enabled, modeLive) {
      const intervalConfig = document.getElementById('interval-config');
      
      // Boutons ON/OFF
      document.getElementById('capture-off').classList.toggle('active', !enabled);
      document.getElementById('capture-on').classList.toggle('active', enabled);
      
      // Boutons Live/Intervalle
      document.getElementById('mode-live').classList.toggle('active', modeLive);
      document.getElementById('mode-interval').classList.toggle('active', !modeLive);
      
      // Afficher config intervalle si mode intervalle
      intervalConfig.style.display = modeLive ? 'none' : 'block';
    }

    // ========== TEST MANUEL ==========
    function testCapture() {
      const btn = document.getElementById('test-btn');
      const resultDiv = document.getElementById('test-result');
      const previewImg = document.getElementById('preview-img');
      
      btn.disabled = true;
      btn.innerHTML = '<span class="loading"></span> Analyse...';
      resultDiv.innerHTML = '';
      resultDiv.className = '';

      fetch('/api/capture')
        .then(r => r.json())
        .then(d => {
          btn.disabled = false;
          btn.innerHTML = '🚀 Tester maintenant';
          
          if (d.success) {
            resultDiv.className = 'alert alert-success';
            resultDiv.innerHTML = '<strong>Succès!</strong><br>' + 
              JSON.stringify(d.results, null, 2).replace(/\n/g, '<br>');
            refreshStatus();
            
            // Forcer le rechargement de l'image
            previewImg.src = '/capture?t=' + Date.now();
          } else {
            resultDiv.className = 'alert alert-danger';
            resultDiv.innerHTML = '<strong>Erreur:</strong> ' + d.error;
          }
        })
        .catch(e => {
          btn.disabled = false;
          btn.innerHTML = '🚀 Tester maintenant';
          resultDiv.className = 'alert alert-danger';
          resultDiv.innerHTML = '<strong>Erreur communication:</strong> ' + e;
        });
    }

    // ========== CAPTURE CONTROL ==========
    let captureEnabled = false;
    let captureModeLive = true;

    function toggleCapture(enabled) {
      captureEnabled = enabled;
      updateCaptureButtons(captureEnabled, captureModeLive);
      fetch('/api/toggle_capture?enabled=' + (enabled ? '1' : '0'));
    }

    function setCaptureMode(isLive) {
      captureModeLive = isLive;
      updateCaptureButtons(captureEnabled, captureModeLive);
      fetch('/api/set_mode?live=' + (isLive ? '1' : '0'));
    }

    function saveInterval() {
      const secs = parseInt(document.getElementById('interval-input').value) || 30;
      fetch('/api/set_interval?seconds=' + secs).then(() => {
        alert('✅ Intervalle sauvegardé: ' + secs + ' secondes');
      });
    }

    function refreshPreview() {
      document.getElementById('preview-img').src = '/capture?' + Date.now();
    }

    // ========== CAMERA LIVE FEED ==========
    let cameraAutoRefresh = false;
    let cameraRefreshInterval = null;

    function refreshCameraPreview() {
      const img = document.getElementById('camera-preview');
      if (img) {
        img.src = '/capture?t=' + Date.now();
      }
    }

    function toggleAutoRefresh() {
      cameraAutoRefresh = !cameraAutoRefresh;
      const btn = document.getElementById('auto-refresh-btn');

      if (cameraAutoRefresh) {
        btn.textContent = '⏸️ Pause';
        btn.classList.remove('btn-secondary');
        btn.classList.add('btn-success');
        cameraRefreshInterval = setInterval(refreshCameraPreview, 1000);
      } else {
        btn.textContent = '▶️ Auto-Refresh';
        btn.classList.remove('btn-success');
        btn.classList.add('btn-secondary');
        if (cameraRefreshInterval) {
          clearInterval(cameraRefreshInterval);
          cameraRefreshInterval = null;
        }
      }
    }

    // ========== MODELS & NETWORKS ==========
    function refreshModels() {
      const btn = document.getElementById('refresh-models-btn');
      btn.disabled = true;
      btn.innerHTML = '<span class="loading"></span> Chargement...';

      fetch('/api/refresh_models')
        .then(r => r.json())
        .then(d => {
          let select = document.getElementById('model-select');
          select.innerHTML = '';
          d.models.forEach(m => {
            let opt = document.createElement('option');
            opt.value = m;
            opt.textContent = m;
            select.appendChild(opt);
          });
          btn.disabled = false;
          btn.innerHTML = '🔄 Rafraîchir';
        })
        .catch(e => {
          alert('Erreur: ' + e);
          btn.disabled = false;
          btn.innerHTML = '🔄 Rafraîchir';
        });
    }

    function refreshNetworks() {
      const btn = document.getElementById('refresh-networks-btn');
      btn.disabled = true;
      btn.innerHTML = '<span class="loading"></span> Recherche...';

      fetch('/api/scan_wifi')
        .then(r => r.json())
        .then(d => {
          let select = document.getElementById('ssid-select');
          select.innerHTML = '';
          d.networks.forEach(n => {
            let opt = document.createElement('option');
            opt.value = n;
            opt.textContent = n;
            select.appendChild(opt);
          });
          btn.disabled = false;
          btn.innerHTML = '📡 Scanner';
        })
        .catch(e => {
          alert('Erreur: ' + e);
          btn.disabled = false;
          btn.innerHTML = '📡 Scanner';
        });
    }

    function providerChanged(provider) {
      document.getElementById('lmstudio-config').style.display = provider == '0' ? 'block' : 'none';
      document.getElementById('ollama-config').style.display = provider == '1' ? 'block' : 'none';
      document.getElementById('openai-config').style.display = provider == '2' ? 'block' : 'none';
    }

    function setProvider(provider) {
      // Mettre à jour l'input hidden
      document.getElementById('provider-input').value = provider;
      
      // Mettre à jour les boutons
      for (let i = 0; i < 3; i++) {
        document.getElementById('provider-' + i).classList.remove('active');
      }
      document.getElementById('provider-' + provider).classList.add('active');
      
      // Afficher la config correspondante
      providerChanged(provider);
    }

    function setQuestionType(questionId, type, btn) {
      // Mettre à jour l'input hidden
      document.getElementById('type-' + questionId).value = type;
      
      // Mettre à jour les boutons (dans le même groupe)
      btn.parentElement.querySelectorAll('.mode-btn').forEach(b => b.classList.remove('active'));
      btn.classList.add('active');
    }

    // ========== INIT ==========
    document.addEventListener('DOMContentLoaded', () => {
      refreshStatus();
      // Vérifier s'il y a un hash dans l'URL pour afficher le bon onglet
      const hash = window.location.hash.replace('#', '');
      if (hash && document.getElementById(hash)) {
        showTab(hash);
      } else {
        showTab('dashboard');
      }
    });
  </script>
</body>
</html>
)rawliteral";

#endif

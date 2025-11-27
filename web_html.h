/*
 * Interface Web HTML/CSS/JS avec menus modernes
 */

#ifndef WEB_HTML_H
#define WEB_HTML_H

#include <Arduino.h>

// Header HTML avec CSS moderne
const char HTML_HEAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html lang="fr"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>IAction32</title>
<style>
:root{--bd:#0a0e27;--bc:rgba(16,22,47,.8);--ab:#00d9ff;--ag:#00ff88;--ap:#b744ff;--tp:#fff;--ts:#8892b0;--br:rgba(255,255,255,.1);--sh:0 10px 40px rgba(0,0,0,.5)}
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;background:var(--bd);background-image:radial-gradient(at 0 0,rgba(0,217,255,.1) 0,transparent 50%),radial-gradient(at 100% 100%,rgba(183,68,255,.1) 0,transparent 50%);min-height:100vh;color:var(--tp);overflow-x:hidden}
.navbar{background:var(--bc);backdrop-filter:blur(20px);border-bottom:1px solid var(--br);position:sticky;top:0;z-index:1000;box-shadow:var(--sh)}
.navbar-content{max-width:1400px;margin:0 auto;display:flex;align-items:center;justify-content:space-between;padding:1rem 2rem}
.logo{font-size:1.5rem;font-weight:700;background:linear-gradient(135deg,var(--ab),var(--ag));-webkit-background-clip:text;-webkit-text-fill-color:transparent;display:flex;align-items:center;gap:10px}
.logo span{-webkit-text-fill-color:initial;font-size:1.8rem}
.nav-menu{display:flex;gap:0;list-style:none}
.nav-link{padding:.75rem 1.5rem;color:var(--ts);text-decoration:none;transition:.3s;display:block;border-radius:8px;font-weight:500}
.nav-link:hover,.nav-link.active{color:var(--ab);background:rgba(0,217,255,.1)}
.mobile-toggle{display:none;background:0;border:0;color:var(--tp);font-size:1.5rem;cursor:pointer}
.container{max-width:1400px;margin:2rem auto;padding:0 2rem}
.card{background:var(--bc);backdrop-filter:blur(20px);border-radius:16px;padding:2rem;margin-bottom:2rem;border:1px solid var(--br);box-shadow:var(--sh)}
.card-header{display:flex;align-items:center;justify-content:space-between;margin-bottom:1.5rem;padding-bottom:1rem;border-bottom:1px solid var(--br)}
.card-title{font-size:1.3rem;font-weight:600;display:flex;align-items:center;gap:12px}
.card-icon{width:32px;height:32px;display:flex;align-items:center;justify-content:center;background:linear-gradient(135deg,var(--ab),var(--ap));border-radius:8px;font-size:1.2rem}
.grid{display:grid;gap:1.5rem}.grid-2{grid-template-columns:repeat(auto-fit,minmax(300px,1fr))}.grid-3{grid-template-columns:repeat(auto-fit,minmax(200px,1fr))}.grid-4{grid-template-columns:repeat(auto-fit,minmax(150px,1fr))}
.stat-card{background:rgba(0,0,0,.3);padding:1.5rem;border-radius:12px;text-align:center;border:1px solid var(--br);transition:.3s}
.stat-card:hover{transform:translateY(-5px);border-color:var(--ab)}
.stat-value{font-size:2rem;font-weight:700;background:linear-gradient(135deg,var(--ab),var(--ag));-webkit-background-clip:text;-webkit-text-fill-color:transparent;margin-bottom:.5rem}
.stat-label{color:var(--ts);font-size:.9rem;text-transform:uppercase;letter-spacing:1px}
.stat-card.error .stat-value{background:linear-gradient(135deg,#ff4757,#ff6348);-webkit-background-clip:text;-webkit-text-fill-color:transparent}
.form-group{margin-bottom:1.5rem}
.form-label{display:block;margin-bottom:.5rem;color:var(--ts);font-size:.9rem;font-weight:500;text-transform:uppercase;letter-spacing:.5px}
.form-input,.form-select,.form-textarea{width:100%;padding:.875rem 1rem;background:rgba(0,0,0,.4);border:1px solid var(--br);border-radius:8px;color:var(--tp);font-size:1rem;transition:.3s}
.form-input:focus,.form-select:focus,.form-textarea:focus{outline:0;border-color:var(--ab);box-shadow:0 0 0 3px rgba(0,217,255,.1)}
.form-textarea{resize:vertical;min-height:100px;font-family:inherit}
.input-group{display:flex;gap:.5rem}.input-group .form-input,.input-group .form-select{flex:1}
.btn{padding:.875rem 1.5rem;border:0;border-radius:8px;font-size:1rem;font-weight:600;cursor:pointer;transition:.3s;display:inline-flex;align-items:center;justify-content:center;gap:8px;text-decoration:none}
.btn:disabled{opacity:.5;cursor:not-allowed}
.btn-primary{background:linear-gradient(135deg,var(--ab),var(--ag));color:var(--bd);font-weight:700}
.btn-primary:hover:not(:disabled){transform:translateY(-2px);box-shadow:0 10px 25px rgba(0,217,255,.3)}
.btn-secondary{background:rgba(255,255,255,.1);color:var(--tp);border:1px solid var(--br)}
.btn-secondary:hover:not(:disabled){background:rgba(255,255,255,.15);border-color:var(--ab)}
.btn-danger{background:linear-gradient(135deg,#ff4757,#ff6348);color:#fff}
.btn-success{background:linear-gradient(135deg,var(--ag),var(--ab));color:var(--bd);font-weight:700}
.btn-small{padding:.5rem 1rem;font-size:.875rem}
.btn-group{display:flex;gap:.75rem;flex-wrap:wrap}
.mode-buttons{display:flex;gap:.5rem;flex-wrap:wrap}
.mode-btn{padding:.75rem 1.25rem;border:2px solid var(--br);border-radius:8px;background:rgba(0,0,0,.3);color:var(--ts);font-size:.95rem;font-weight:600;cursor:pointer;transition:.3s}
.mode-btn:hover{border-color:var(--ab);color:var(--tp)}
.mode-btn.active{background:linear-gradient(135deg,var(--ab),var(--ag));border-color:transparent;color:var(--bd)}
.toggle-group{display:flex;justify-content:space-between;align-items:center;padding:1rem;background:rgba(0,0,0,.2);border-radius:8px;margin-bottom:1rem}
.toggle{position:relative;width:60px;height:32px}
.toggle input{opacity:0;width:0;height:0}
.toggle-slider{position:absolute;cursor:pointer;inset:0;background:rgba(255,255,255,.2);border-radius:32px;transition:.3s}
.toggle-slider::before{position:absolute;content:"";height:24px;width:24px;left:4px;bottom:4px;background:#fff;border-radius:50%;transition:.3s}
.toggle input:checked+.toggle-slider{background:linear-gradient(135deg,var(--ab),var(--ag))}
.toggle input:checked+.toggle-slider::before{transform:translateX(28px)}
.badge{padding:.375rem .75rem;border-radius:6px;font-size:.75rem;font-weight:700;text-transform:uppercase}
.badge-bool{background:var(--ab);color:var(--bd)}
.badge-decimal{background:var(--ap);color:#fff}
.alert{padding:1rem 1.25rem;border-radius:8px;margin-bottom:1rem;border-left:4px solid}
.alert-success{background:rgba(0,255,136,.1);border-color:var(--ag);color:var(--ag)}
.alert-error{background:rgba(255,71,87,.1);border-color:#ff4757;color:#ff6b7a}
.alert-info{background:rgba(0,217,255,.1);border-color:var(--ab);color:var(--ab)}
.preview-container{text-align:center;margin:1.5rem 0}
.preview-img{max-width:100%;border-radius:12px;border:2px solid var(--br);box-shadow:var(--sh)}
.question-item{background:rgba(0,0,0,.3);padding:1.5rem;border-radius:12px;border:1px solid var(--br);margin-bottom:1rem;transition:.3s}
.question-item:hover{border-color:var(--ab);transform:translateX(5px)}
.question-header{display:flex;justify-content:space-between;align-items:center;margin-bottom:1rem}
.loading{display:inline-block;width:16px;height:16px;border:2px solid rgba(255,255,255,.3);border-top-color:var(--ab);border-radius:50%;animation:spin .8s linear infinite}
@keyframes spin{to{transform:rotate(360deg)}}
.tab-content{display:none}.tab-content.active{display:block}
.log-line{border-bottom:1px solid #333;padding:2px 0;white-space:pre-wrap;word-wrap:break-word}
@media(max-width:768px){.navbar-content{padding:1rem}.nav-menu{display:none;position:absolute;top:100%;left:0;right:0;background:var(--bc);flex-direction:column;border-top:1px solid var(--br)}.nav-menu.active{display:flex}.mobile-toggle{display:block}.container{padding:0 1rem}.card{padding:1.5rem}.btn-group{flex-direction:column}.btn{width:100%}}
::-webkit-scrollbar{width:10px}::-webkit-scrollbar-track{background:#060918}::-webkit-scrollbar-thumb{background:linear-gradient(180deg,var(--ab),var(--ap));border-radius:5px}
</style></head><body>
<nav class="navbar"><div class="navbar-content"><div class="logo"><span>🤖</span>IAction32</div><button class="mobile-toggle" onclick="toggleMobileMenu()">☰</button>
<ul class="nav-menu" id="navMenu"><li><a href="#dashboard" class="nav-link active" onclick="showTab('dashboard')">Dashboard</a></li><li><a href="#config" class="nav-link" onclick="showTab('config')">Configuration</a></li><li><a href="#questions" class="nav-link" onclick="showTab('questions')">Questions</a></li><li><a href="#camera" class="nav-link" onclick="showTab('camera')">Caméra</a></li><li><a href="#logs" class="nav-link" onclick="showTab('logs')">Logs</a></li></ul>
</div></nav><div class="container">
)rawliteral";

const char HTML_FOOT[] PROGMEM = R"rawliteral(
</div><script>
let currentTab='dashboard',captureEnabled=false,captureModeLive=true,cameraAutoRefresh=false,cameraRefreshInterval=null;
function $(id){return document.getElementById(id)}
function showTab(t){currentTab=t;document.querySelectorAll('.tab-content,.nav-link').forEach(e=>e.classList.remove('active'));$(t).classList.add('active');document.querySelector(`[onclick="showTab('${t}')"]`).classList.add('active')}
function toggleMobileMenu(){$('navMenu').classList.toggle('active')}
function refreshStatus(){fetch('/api/status').then(r=>r.json()).then(d=>{$('stat-captures').textContent=d.captures;$('stat-errors').textContent=d.errors;$('stat-last').textContent=d.lastResult;if($('stat-ip'))$('stat-ip').textContent=d.ip;captureEnabled=d.captureEnabled;captureModeLive=d.captureModeLive;updateCaptureButtons()}).catch(e=>console.error(e))}
function updateCaptureButtons(){$('capture-off').classList.toggle('active',!captureEnabled);$('capture-on').classList.toggle('active',captureEnabled);$('mode-live').classList.toggle('active',captureModeLive);$('mode-interval').classList.toggle('active',!captureModeLive);$('interval-config').style.display=captureModeLive?'none':'block'}
function testCapture(){let b=$('test-btn'),r=$('test-result'),p=$('preview-img');b.disabled=true;b.innerHTML='<span class="loading"></span> Analyse...';r.innerHTML='';r.className='';fetch('/api/capture').then(x=>x.json()).then(d=>{b.disabled=false;b.innerHTML='🚀 Tester';if(d.success){r.className='alert alert-success';r.innerHTML='<b>OK!</b><br>'+JSON.stringify(d.results,null,2).replace(/\n/g,'<br>');refreshStatus();p.src='/capture?t='+Date.now()}else{r.className='alert alert-error';r.innerHTML='<b>Erreur:</b> '+d.error}}).catch(e=>{b.disabled=false;b.innerHTML='🚀 Tester';r.className='alert alert-error';r.innerHTML='Erreur: '+e})}
function toggleCapture(e){captureEnabled=e;updateCaptureButtons();fetch('/api/toggle_capture?enabled='+(e?'1':'0'))}
function setCaptureMode(l){captureModeLive=l;updateCaptureButtons();fetch('/api/set_mode?live='+(l?'1':'0'))}
function saveInterval(){let s=parseInt($('interval-input').value)||30;fetch('/api/set_interval?seconds='+s).then(()=>alert('✅ Intervalle: '+s+'s'))}
function refreshPreview(){$('preview-img').src='/capture?'+Date.now()}
function refreshCameraPreview(){let i=$('camera-preview');if(i)i.src='/capture?t='+Date.now()}
function toggleAutoRefresh(){cameraAutoRefresh=!cameraAutoRefresh;let b=$('auto-refresh-btn');if(cameraAutoRefresh){b.textContent='⏸ Pause';b.className='btn btn-success btn-small';cameraRefreshInterval=setInterval(refreshCameraPreview,1000)}else{b.textContent='▶ Auto';b.className='btn btn-secondary btn-small';if(cameraRefreshInterval){clearInterval(cameraRefreshInterval);cameraRefreshInterval=null}}}
function refreshModels(p){let cfg=['lmstudio','ollama'][p];if(!cfg)return;let b=$('refresh-btn-'+cfg),s=$('model-select-'+cfg),h=$(p==0?'input-lm-host':'input-ollama-host').value;b.disabled=true;b.innerHTML='<span class="loading"></span>';fetch('/api/refresh_models?provider='+p+'&host='+encodeURIComponent(h)).then(r=>r.json()).then(d=>{s.innerHTML='';d.models.forEach(m=>{let o=document.createElement('option');o.value=o.textContent=m;s.appendChild(o)});b.disabled=false;b.innerHTML='🔄'}).catch(e=>{alert(e);b.disabled=false;b.innerHTML='🔄'})}
function refreshNetworks(){let b=$('refresh-networks-btn');b.disabled=true;b.innerHTML='<span class="loading"></span>';fetch('/api/scan_wifi').then(r=>r.json()).then(d=>{let s=$('ssid-select');s.innerHTML='';d.networks.forEach(n=>{let o=document.createElement('option');o.value=o.textContent=n;s.appendChild(o)});b.disabled=false;b.innerHTML='📡 Scanner'}).catch(e=>{alert(e);b.disabled=false;b.innerHTML='📡 Scanner'})}
function toggleInputs(c,e){$(c).querySelectorAll('input,select,button').forEach(i=>{if(e)i.removeAttribute('disabled');else i.setAttribute('disabled','true')})}
function providerChanged(p){['lmstudio','ollama','openai'].forEach((c,i)=>{$(c+'-config').style.display=p==i?'block':'none';toggleInputs(c+'-config',p==i)})}
function setProvider(p){$('provider-input').value=p;for(let i=0;i<3;i++)$('provider-'+i).classList.toggle('active',i==p);providerChanged(p)}
function setQuestionType(q,t,b){$('type-'+q).value=t;b.parentElement.querySelectorAll('.mode-btn').forEach(x=>x.classList.remove('active'));b.classList.add('active')}
function refreshLogs(){let c=$('log-container');if(!c)return;fetch('/api/logs').then(r=>r.text()).then(h=>{c.innerHTML=h}).catch(()=>{c.innerHTML='<div style="color:red">Erreur</div>'})}
function clearLogs(){if(!confirm('Effacer?'))return;fetch('/api/clear_logs').then(()=>refreshLogs())}
function initUpdateForm(){let f=document.querySelector('form[action="/update"]');if(!f)return;f.onsubmit=function(e){e.preventDefault();let fi=f.querySelector('input[type="file"]'),btn=f.querySelector('button[type="submit"]');if(!fi.files.length){alert('Selectionnez un fichier');return}let fd=new FormData();fd.append('update',fi.files[0]);btn.disabled=true;btn.innerHTML='<span class="loading"></span> Upload...';let pg=$('update-progress');if(pg)pg.style.display='block';let xhr=new XMLHttpRequest();xhr.open('POST','/update',true);xhr.upload.onprogress=function(ev){if(ev.lengthComputable){let pct=Math.round(ev.loaded/ev.total*100);let bar=$('update-bar');if(bar)bar.style.width=pct+'%';let txt=$('update-pct');if(txt)txt.textContent=pct+'%'}};xhr.onload=function(){if(xhr.status==200){btn.innerHTML='OK! Redemarrage...';let bar=$('update-bar');if(bar){bar.style.width='100%';bar.style.background='#00ff88'}setTimeout(()=>location.reload(),5000)}else{btn.disabled=false;btn.innerHTML='Mettre a jour';alert('Erreur: '+xhr.statusText)}};xhr.onerror=function(){btn.disabled=false;btn.innerHTML='Mettre a jour';alert('Erreur reseau')};xhr.send(fd)}}
document.addEventListener('DOMContentLoaded',()=>{refreshStatus();setInterval(refreshStatus,2000);setInterval(()=>{if(currentTab==='logs')refreshLogs()},3000);let p=$('provider-input');if(p)providerChanged(p.value);let h=location.hash.slice(1);showTab(h&&$(h)?h:'dashboard');initUpdateForm()})
</script></body></html>
)rawliteral";

#endif

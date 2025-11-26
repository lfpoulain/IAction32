# IAction32 v2.4

Système de vision IA multi-provider pour ESP32-CAM avec **MQTT natif** et intégration **Home Assistant automatique**.

---

## 🚀 Fonctionnalités

### ⚡ Optimisation Batch (v2.4)
- **Une seule requête IA** pour toutes les questions (au lieu d'une par question)
- Réduction drastique du temps de traitement et des coûts API
- Réponse JSON structurée parsée automatiquement

### 📡 MQTT + Home Assistant
- **Auto-Discovery** : Les capteurs apparaissent automatiquement dans Home Assistant
- **MQTT natif** : Communication temps réel via protocole standard IoT
- **État persistant** : Messages retained pour conserver les valeurs
- **Availability tracking** : Home Assistant sait quand l'ESP32-CAM est en ligne/hors ligne
- **Contrôle bidirectionnel** : Switch MQTT pour activer/désactiver la capture à distance
- **Republication automatique** : Discovery republié toutes les 5 minutes
- **Pas de configuration manuelle** : Tout est automatique !

### 🤖 Multi-Provider IA
- **LM Studio** : Serveur local pour modèles vision (Qwen, LLaVA, etc.)
- **Ollama** : Serveur local alternatif
- **OpenAI** : API cloud (gpt-4.1-nano, gpt-4.1-mini, gpt-4.1)

### 🎨 Interface Web Moderne
- Navigation par onglets (Dashboard, Configuration, Questions, Caméra)
- **Boutons toggle** intuitifs (fini les menus déroulants !)
- Design moderne avec dégradés et animations
- Responsive (mobile-friendly)
- Refresh automatique des statistiques

### ✨ Fonctionnalités Avancées
- ✅ Détection automatique des modèles IA disponibles
- ✅ Scan automatique des réseaux WiFi
- ✅ Questions multiples avec types (Bool / Compteur)
- ✅ **Instructions automatiques** ajoutées selon le type de question
- ✅ **Capture ON/OFF** : Activation/désactivation globale (contrôlable via Home Assistant)
- ✅ **Type Live** : Envoi dès que l'IA répond
- ✅ **Type Intervalle** : Capture toutes les X secondes
- ✅ Résolutions 16:9 optimisées (HQVGA, HVGA, HD)
- ✅ MQTT avec auto-discovery Home Assistant
- ✅ Statistiques en temps réel
- ✅ **Fonctionnement autonome** : La capture continue même si l'interface web est fermée

---

## 📁 Structure du Projet

```
IAction32/
├── IAction32.ino              # Fichier principal (setup/loop)
├── config.h                    # Configuration et structures
├── globals.cpp                 # Variables globales
├── storage.h/cpp               # Gestion Preferences (EEPROM)
├── camera_setup.h/cpp          # Gestion caméra
├── wifi_manager.h/cpp          # Gestion WiFi
├── mqtt_manager.h/cpp          # Gestion MQTT + Auto-Discovery
├── ai_provider.h/cpp           # Providers IA (LM Studio, Ollama, OpenAI)
├── web_server.h/cpp            # Serveur web et handlers HTTP
├── web_html.h                  # Templates HTML/CSS/JS
├── web_pages.h/cpp             # Construction des pages
└── README.md                   # Documentation
```

---

## 🔧 Installation

### 1. Prérequis Arduino IDE

Installez les bibliothèques suivantes via **Outils → Gérer les bibliothèques** :

- **ESP32** (Board Manager)
- **ArduinoJson** (v6.x)
- **PubSubClient** (v2.8+) ⬅️ NOUVEAU v2.1
- **Base64** by Arturo Guadalupi

### 2. Configuration du Board

Dans `config.h`, sélectionnez votre modèle :

```cpp
#define CAMERA_MODEL_WROVER_KIT  // Pour ESP32-CAM Wrover
// #define CAMERA_MODEL_AI_THINKER  // Pour AI-Thinker
```

**Paramètres Arduino IDE** :
```
Board: AI Thinker ESP32-CAM ou Wrover Kit
Upload Speed: 115200
Flash Frequency: 80MHz
Flash Mode: QIO
Partition Scheme: Huge APP (3MB No OTA)
```

### 3. Configuration Initiale (config.h)

Avant de compiler, modifiez les valeurs par défaut :

```cpp
#define DEFAULT_WIFI_SSID      "VotreSSID"           // ⚠️ À MODIFIER
#define DEFAULT_WIFI_PASSWORD  "VotreMotDePasse"     // ⚠️ À MODIFIER
#define DEFAULT_LM_HOST        "http://192.168.X.X:1234"  // IP de votre serveur
#define DEFAULT_MQTT_SERVER    "192.168.X.X"         // IP de votre Home Assistant
```

Ou configurez directement via l'interface web après le premier démarrage.

### 4. Téléversement

1. Ouvrez `IAction32.ino` dans Arduino IDE
2. Sélectionnez le board : **AI Thinker ESP32-CAM ou Wrover**
3. Sélectionnez le port COM
4. Appuyez sur Upload
5. Appuyez sur **RESET**

---

## 🌐 Première Utilisation

### Connexion Initiale

1. Allumez l'ESP32-CAM (alimentation 5V 2A minimum)
2. Connectez-vous au WiFi : **ESP32-CAM-Config** (mot de passe : `12345678`)
3. Accédez à : `http://192.168.4.1`
4. Configurez votre réseau WiFi principal dans l'onglet **Configuration**
5. Sauvegardez et redémarrez

### Vérification des Logs

Ouvrez le **Moniteur Série** (115200 baud) :

```
╔════════════════════════════════════════╗
║   IAction32 v2.3                      ║
║   Multi-Provider AI + MQTT/HA         ║
╚════════════════════════════════════════╝

✓ Configuration chargée
✓ Caméra initialisée
✓ WiFi connecté! IP: 192.168.1.200
✓ MQTT connecté
✓ Serveur web démarré
✓ Système prêt
```

---

## ⚙️ Configuration

### Provider IA

#### Option 1 : LM Studio (Local - Recommandé)
```
Serveur : http://192.168.1.100:1234
Modèle : qwen/qwen3-vl-30b (ou autre)
```
Cliquez sur **"Rafraîchir les modèles"** pour auto-détecter les modèles disponibles.

#### Option 2 : Ollama (Local)
```
Serveur : http://192.168.1.100:11434
Modèle : llava:latest (ou autre)
```

#### Option 3 : OpenAI (Cloud)
```
Clé API : sk-...
Modèles disponibles :
  - gpt-4.1-nano (défaut, très économique)
  - gpt-4.1-mini (équilibré performance/coût)
  - gpt-4.1 (haute performance)
```

**Conseils modèles OpenAI** :
- **gpt-4.1-nano** : Parfait pour détections simples (person/package)
- **gpt-4.1-mini** : Bon compromis pour la plupart des cas
- **gpt-4.1** : Pour analyses complexes ou haute précision

### Configuration MQTT

Dans l'onglet **Configuration → MQTT / Home Assistant** :

```
☑️ Activer MQTT
Serveur: 192.168.1.100 (IP de votre Home Assistant)
Port: 1883
Utilisateur: homeassistant (optionnel)
Mot de passe: ******** (optionnel)
Topic de base: iaction32
```

**Dans Home Assistant** :

Éditez `configuration.yaml` :

```yaml
mqtt:
  broker: localhost
  discovery: true  # ⚠️ OBLIGATOIRE pour l'auto-discovery
```

Redémarrez Home Assistant. Les capteurs apparaissent automatiquement dans :
```
Configuration → Appareils et services → MQTT → IAction32
```

### Questions IA

Dans l'onglet **Questions** :

1. Cliquez sur **➕ Ajouter une question**
2. Configurez :
   - **Prompt** : La question à poser à l'IA
   - **Type** : Bool (true/false) ou Compteur (entier 0,1,2,3...)
   - **Clé MQTT** : Nom du capteur

> 💡 **Instructions automatiques** : Selon le type choisi, le système ajoute automatiquement les instructions de format à l'IA. Vous n'avez plus besoin d'écrire "Answer only with true or false" !

**Exemples de questions** :

```
Question 1:
  Prompt: "Is there a person visible in this image?"
  Type: Bool ✓
  Clé: person_detected
  → L'IA reçoit automatiquement: "... Answer only with 'true' or 'false', nothing else."
  → Créera: binary_sensor.iaction32_XXXXXX_person_detected

Question 2:
  Prompt: "How many cars are visible?"
  Type: Compteur 🔢
  Clé: car_count
  → L'IA reçoit automatiquement: "... Answer only with a whole number (integer like 0, 1, 2, 3...), nothing else."
  → Créera: sensor.iaction32_XXXXXX_car_count

Question 3:
  Prompt: "Is there a package or delivery box visible?"
  Type: Bool ✓
  Clé: package_detected
  → Créera: binary_sensor.iaction32_XXXXXX_package_detected
```

---

## 📡 MQTT et Home Assistant

### Topics Publiés

Structure automatique :

```
iaction32/iaction32_[MAC]/
├── person_detected      → ON/OFF
├── package_detected     → ON/OFF
├── car_count           → 0,1,2,3...
├── state               → JSON complet
├── availability        → online/offline
└── last_update         → timestamp
```

### Entités Créées Automatiquement

Les entités suivantes apparaissent automatiquement dans Home Assistant :

- `binary_sensor.iaction32_XXXXXX_person_detected` (pour les questions Bool)
- `sensor.iaction32_XXXXXX_car_count` (pour les questions Compteur)
- `switch.iaction32_XXXXXX_capture` (Activer/Désactiver la capture)
- `sensor.iaction32_XXXXXX_ip` (Diagnostic IP)

**Aucune configuration manuelle nécessaire !**

### Exemples d'Automations

#### Activation à distance

```yaml
automation:
  - id: iaction32_activate_when_away
    alias: "IAction32 - Activer quand absent"
    trigger:
      - platform: state
        entity_id: group.family
        to: 'not_home'
    action:
      - service: switch.turn_on
        target:
          entity_id: switch.iaction32_abc123_capture
```

```yaml
automation:
  - id: iaction32_person_alert
    alias: "IAction32 - Alerte Personne"
    trigger:
      - platform: state
        entity_id: binary_sensor.iaction32_abc123_person_detected
        to: 'on'
    action:
      - service: notify.mobile_app_iphone
        data:
          title: "🚨 Détection"
          message: "Une personne a été détectée !"
```

#### Lumière automatique la nuit

```yaml
automation:
  - id: iaction32_night_light
    alias: "IAction32 - Lumière Nocturne"
    trigger:
      - platform: state
        entity_id: binary_sensor.iaction32_abc123_person_detected
        to: 'on'
    condition:
      - condition: sun
        after: sunset
        before: sunrise
    action:
      - service: light.turn_on
        target:
          entity_id: light.entree
        data:
          brightness: 255
          transition: 1
```

#### Alerte colis livré

```yaml
automation:
  - id: iaction32_package_alert
    alias: "IAction32 - Colis Livré"
    trigger:
      - platform: state
        entity_id: binary_sensor.iaction32_abc123_package_detected
        to: 'on'
    action:
      - service: notify.mobile_app_iphone
        data:
          title: "📦 Livraison"
          message: "Un colis a été livré !"
```

#### Parking plein

```yaml
automation:
  - id: iaction32_parking_full
    alias: "IAction32 - Parking Complet"
    trigger:
      - platform: numeric_state
        entity_id: sensor.iaction32_abc123_car_count
        above: 10
    action:
      - service: notify.family
        data:
          message: "⚠️ Le parking est complet ({{ states('sensor.iaction32_abc123_car_count') }} voitures)"
```

#### Template sensor personnalisé

```yaml
template:
  - sensor:
      - name: "Statut Parking"
        unique_id: iaction32_parking_status
        state: >
          {% set count = states('sensor.iaction32_abc123_car_count') | int %}
          {% if count == 0 %}
            Vide
          {% elif count < 5 %}
            Disponible ({{ count }}/10)
          {% elif count < 10 %}
            Presque plein ({{ count }}/10)
          {% else %}
            Complet ({{ count }}/10)
          {% endif %}
        icon: >
          {% set count = states('sensor.iaction32_abc123_car_count') | int %}
          {% if count == 0 %}
            mdi:parking
          {% elif count < 10 %}
            mdi:car
          {% else %}
            mdi:car-off
          {% endif %}
```

---

## 🎨 Interface Web

Accédez à `http://[IP_ESP32]/` pour accéder aux différents onglets :

### Dashboard
- Statistiques en temps réel (captures, succès, erreurs)
- **Dernière mise à jour** : Affichage lisible ("il y a 30s", "il y a 5min")
- **Mode de capture** : Boutons Off / Live / Intervalle
- Aperçu caméra en direct
- Test manuel avec bouton "Tester maintenant"

### Configuration
- **WiFi** : Scan automatique des réseaux disponibles
- **Provider IA** : Boutons LM Studio / Ollama / OpenAI + détection auto des modèles
- **MQTT** : Configuration complète avec authentification

### Questions
- Gestion des questions multiples
- Types Bool (true/false) ou Compteur (entier) avec **boutons toggle**
- **Instructions automatiques** ajoutées selon le type
- Ajout/Suppression dynamique
- **Reste sur l'onglet** après modification (plus de refresh intempestif !)

### Caméra
- **Résolutions 16:9** : HQVGA (240x176), HVGA (480x320), HD (1280x720)
- **Réglages image** : Qualité JPEG, Luminosité, Contraste, Saturation, Netteté
- **Options** : Miroir horizontal, Flip vertical
- **Auto** : Balance des blancs, AEC, AGC

---

## 📊 Modes de Capture

> **💡 Optimisation v2.4** : Toutes les questions sont envoyées en une seule requête à l'IA, qui répond avec un JSON structuré. Cela réduit drastiquement le temps de traitement et les coûts API.

### 🔌 Activation (ON/OFF)
Contrôle global de la capture automatique :
- **OFF** : Capture manuelle uniquement via le bouton "Tester maintenant"
- **ON** : Capture automatique selon le type choisi (Live ou Intervalle)

Le switch Home Assistant `switch.iaction32_XXXXXX_capture` permet d'activer/désactiver à distance.

### ⚡ Type Live
Envoi automatique continu à vitesse maximale :

1. Capture une image
2. Envoie **toutes les questions** à l'IA en une seule requête
3. Dès que l'IA répond (JSON), parse et envoie les résultats via MQTT
4. Capture immédiatement une nouvelle image
5. Retour à l'étape 2

**Caractéristiques** :
- Pas de temporisation fixe = réactivité maximale
- S'adapte automatiquement à la vitesse de l'IA
- Idéal pour surveillance active courte durée
- ⚠️ Coût élevé si OpenAI activé en continu

### ⏱️ Type Intervalle
Capture à intervalles réguliers configurables :

1. Capture une image
2. Envoie **toutes les questions** à l'IA en une seule requête
3. Parse le JSON et envoie les résultats via MQTT
4. **Attend X secondes** (configurable de 5 à 3600s)
5. Retour à l'étape 1

**Caractéristiques** :
- Intervalle configurable (5s à 1h)
- Bouton "💾 Sauvegarder" pour appliquer le nouvel intervalle
- Idéal pour monitoring régulier sans surcharge
- Économique avec OpenAI (ex: 1 capture/minute = 1440/jour)

---

## 🐛 Dépannage

### ESP32 ne démarre pas
- Vérifier alimentation 5V 2A minimum
- Débrancher IO0 après upload
- Appuyer sur bouton RESET

### WiFi ne se connecte pas
- Vérifiez le SSID et le mot de passe dans `config.h`
- Utilisez le mode AP : `ESP32-CAM-Config` / `12345678`
- Vérifiez que votre WiFi est en 2.4GHz (pas 5GHz)

### MQTT ne se connecte pas
```bash
# Vérifier que le broker est accessible
ping 192.168.1.100

# Tester connexion MQTT
mosquitto_sub -h 192.168.1.100 -t "#"
```

**Codes d'erreur MQTT** :
- `-2` : Mauvais user/password ou broker inaccessible
- `-4` : Timeout de connexion
- `0` : Connecté avec succès

### Capteurs n'apparaissent pas dans Home Assistant

1. Vérifier `mqtt: discovery: true` dans `configuration.yaml`
2. Redémarrer Home Assistant
3. Redémarrer l'ESP32-CAM (republier discovery)
4. Vérifier logs HA : **Configuration → Logs**
5. Utiliser MQTT Explorer pour voir les topics publiés

### Erreur "HTTP Error: 500" (LM Studio/Ollama)
- Vérifiez que le serveur LM Studio/Ollama est démarré
- Testez l'URL dans un navigateur : `http://IP:PORT/v1/models`
- Vérifiez que le modèle supporte la vision (pas uniquement texte)

### Pas de réponse de l'IA
- **LM Studio** : Utilisez un modèle VL (Vision-Language) comme Qwen-VL, LLaVA
- **Ollama** : Utilisez `llava:latest`, `bakllava`, etc.
- **OpenAI** : Vérifiez votre clé API et crédit disponible

### OpenAI "HTTP Error: 401"
- Vérifiez que votre clé API commence par `sk-`
- Vérifiez votre crédit sur [platform.openai.com](https://platform.openai.com/)

### Caméra ne s'initialise pas
- Vérifiez la connexion du module caméra
- Vérifiez que le bon modèle est sélectionné dans `config.h`
- Essayez de baisser la résolution

---

## 💡 Conseils d'Utilisation

### Résolutions Recommandées
- **HQVGA (240x176)** : Tests rapides, faible bande passante
- **HVGA (480x320)** : Bon équilibre qualité/vitesse
- **HD (1280x720)** : Meilleure qualité IA (recommandé pour production)

### Performance
- HD nécessite **PSRAM**
- Résolution plus haute = meilleur résultat IA mais plus lent
- Mode Live avec OpenAI = coûteux (préférez triggers manuels/temporisés)

### Sécurité
- Les mots de passe sont masqués dans l'interface
- La clé OpenAI est stockée de manière sécurisée dans les Preferences
- Utilisez l'authentification MQTT en production
- Changez le mot de passe du WiFi AP par défaut

---

## 🎯 Cas d'Usage

### Surveillance Entrée
```
Questions :
1. "Is there a person at the door?" → person_detected (Bool)
2. "Is there a package visible?" → package_detected (Bool)

Automation :
- Si person_detected + nuit → Allumer lumière
- Si package_detected → Notification mobile
```

### Parking Intelligent
```
Questions :
1. "How many cars are visible?" → car_count (Decimal)

Automation :
- Si car_count > 10 → Notification "Parking plein"
- Template sensor → Statut parking (Vide/Disponible/Plein)
```

### Sécurité
```
Questions :
1. "Is there a person?" → person_detected (Bool)

Automation :
- Si person_detected + alarme activée → Alerte intrusion
- Enregistrer snapshot
- Envoyer notification avec image
```

### Surveillance Animaux
```
Questions :
1. "Is there a dog or cat visible?" → pet_detected (Bool)
2. "Is the pet eating from the bowl?" → pet_eating (Bool)
```

### Lecture de Compteurs
```
Questions :
1. "What number is displayed on the meter?" → meter_reading (Decimal)

Automation :
- Enregistrer la lecture quotidienne
- Alerter si consommation anormale
```

---

## 🔍 Test MQTT

### Écouter les messages publiés

```bash
# Tous les topics IAction32
mosquitto_sub -h localhost -t "iaction32/#" -v

# Topic spécifique
mosquitto_sub -h localhost -t "iaction32/iaction32_XXXXXX/person_detected" -v
```

**Vous devriez voir** :
```
iaction32/iaction32_XXXXXX/availability online
iaction32/iaction32_XXXXXX/person_detected ON
iaction32/iaction32_XXXXXX/car_count 3
```

### MQTT Explorer (Outil graphique)

1. Téléchargez [MQTT Explorer](http://mqtt-explorer.com/)
2. Connectez-vous à votre broker
3. Naviguez dans l'arborescence des topics
4. Visualisez les messages en temps réel

---

## 📝 Notes Techniques

### Limitations
- **Max questions** : 10 questions simultanées
- **PSRAM requis** : Pour résolution HD
- **WiFi** : 2.4GHz uniquement (pas 5GHz)
- **MQTT Buffer** : 2048 bytes (suffisant pour la plupart des cas)

### Versions
- **v2.3** : Switch MQTT bidirectionnel, Capture autonome server-side, Interface optimisée
- **v2.2** : Mode Intervalle, Interface boutons toggle, Instructions auto, Timestamp lisible
- **v2.1** : MQTT natif + Auto-Discovery Home Assistant
- **v2.0** : Architecture modulaire + Interface moderne
- **v1.0** : Version initiale monolithique

### Compatibilité
- ESP32-CAM AI-Thinker ✅
- ESP32-CAM Wrover Kit ✅
- Home Assistant 2024.x ✅
- Mosquitto Broker 2.0+ ✅

---

## 🔗 Liens Utiles

- [LM Studio](https://lmstudio.ai/) - Serveur local pour modèles IA
- [Ollama](https://ollama.ai/) - Alternative à LM Studio
- [OpenAI API](https://platform.openai.com/) - API cloud
- [Home Assistant MQTT](https://www.home-assistant.io/integrations/mqtt/) - Documentation officielle
- [MQTT Discovery](https://www.home-assistant.io/integrations/mqtt/#mqtt-discovery) - Auto-discovery HA
- [PubSubClient](https://github.com/knolleary/pubsubclient) - Bibliothèque MQTT ESP32
- [MQTT Explorer](http://mqtt-explorer.com/) - Outil de debug MQTT

---

## 👨‍💻 Auteur

Les Frères Poulain**
2024

---

## 📄 Licence

MIT License - Utilisation libre

---


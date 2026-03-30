# Guide : Architecture de Projet VS Code
## Smart Study Desk Monitor - Structure Professionnelle

---

## 📁 Structure du Projet Recommandée

```
smart-study-desk-monitor/
│
├── 📁 docs/                          # Documentation
│   ├── architecture.md
│   ├── setup-guide.md
│   └── user-manual.md
│
├── 📁 hardware/                      # Code ESP32 et firmware
│   ├── 📁 src/
│   │   ├── main.ino
│   │   ├── sensors.h
│   │   ├── sensors.cpp
│   │   ├── wifi_manager.h
│   │   ├── wifi_manager.cpp
│   │   ├── firebase_client.h
│   │   ├── firebase_client.cpp
│   │   ├── comfort_calculator.h
│   │   └── comfort_calculator.cpp
│   │
│   ├── 📁 lib/                       # Bibliothèques externes
│   │   └── README.md
│   │
│   ├── 📁 config/
│   │   └── config.h                  # Configuration WiFi/Firebase
│   │
│   └── platformio.ini                # Configuration PlatformIO
│
├── 📁 cloud/                         # Backend et Firebase
│   ├── 📁 firebase/
│   │   ├── database.rules.json       # Règles de sécurité
│   │   ├── firestore.rules          # Si utilisation Firestore
│   │   └── firebase.json             # Config Firebase
│   │
│   ├── 📁 functions/                 # Cloud Functions (optionnel)
│   │   ├── index.js
│   │   ├── package.json
│   │   └── README.md
│   │
│   └── README.md
│
├── 📁 web/                           # Dashboard et frontend
│   ├── 📁 public/
│   │   ├── index.html
│   │   ├── favicon.ico
│   │   └── manifest.json
│   │
│   ├── 📁 src/
│   │   ├── 📁 components/            # Composants réutilisables
│   │   │   ├── ComfortGauge.js
│   │   │   ├── SensorCard.js
│   │   │   ├── Chart.js
│   │   │   └── AlertPanel.js
│   │   │
│   │   ├── 📁 services/              # Services Firebase
│   │   │   ├── firebase.js
│   │   │   └── dataService.js
│   │   │
│   │   ├── 📁 utils/                 # Utilitaires
│   │   │   ├── calculator.js
│   │   │   └── formatter.js
│   │   │
│   │   ├── 📁 styles/
│   │   │   ├── main.css
│   │   │   ├── variables.css
│   │   │   └── components.css
│   │   │
│   │   └── app.js                    # Point d'entrée
│   │
│   ├── package.json
│   ├── .env.example                  # Template variables d'env
│   ├── .gitignore
│   └── README.md
│
├── 📁 tests/                         # Tests
│   ├── 📁 hardware/
│   │   └── sensor_tests.ino
│   │
│   ├── 📁 web/
│   │   └── firebase-test.html
│   │
│   └── README.md
│
├── 📁 assets/                        # Ressources
│   ├── 📁 images/
│   ├── 📁 diagrams/
│   └── 📁 presentations/
│
├── .gitignore                        # Fichiers à ignorer
├── .vscode/                          # Configuration VS Code
│   ├── settings.json
│   ├── extensions.json
│   └── tasks.json
│
├── README.md                         # Documentation principale
├── LICENSE
└── smart-study-desk-monitor.code-workspace  # Workspace VS Code
```

---

## 🔧 Configuration VS Code

### 1. Créer le Workspace

Créez un fichier `smart-study-desk-monitor.code-workspace` :

```json
{
    "folders": [
        {
            "name": "🔌 Hardware (ESP32)",
            "path": "./hardware"
        },
        {
            "name": "☁️ Cloud (Firebase)",
            "path": "./cloud"
        },
        {
            "name": "🌐 Web (Dashboard)",
            "path": "./web"
        },
        {
            "name": "📚 Documentation",
            "path": "./docs"
        }
    ],
    "settings": {
        "files.exclude": {
            "**/node_modules": true,
            "**/.git": true,
            "**/build": true,
            "**/.pio": true
        },
        "editor.formatOnSave": true,
        "editor.defaultFormatter": "esbenp.prettier-vscode",
        "editor.tabSize": 2,
        "files.autoSave": "afterDelay",
        "files.autoSaveDelay": 1000,
        
        // Configuration C/C++ pour ESP32
        "C_Cpp.default.includePath": [
            "${workspaceFolder}/hardware/src/**",
            "${workspaceFolder}/hardware/lib/**"
        ],
        
        // Configuration Python (si utilisé)
        "python.linting.enabled": true,
        "python.linting.pylintEnabled": true,
        
        // Configuration JavaScript/HTML
        "[javascript]": {
            "editor.defaultFormatter": "esbenp.prettier-vscode"
        },
        "[html]": {
            "editor.defaultFormatter": "esbenp.prettier-vscode"
        },
        "[css]": {
            "editor.defaultFormatter": "esbenp.prettier-vscode"
        }
    },
    "extensions": {
        "recommendations": [
            "platformio.platformio-ide",
            "ms-vscode.cpptools",
            "esbenp.prettier-vscode",
            "dbaeumer.vscode-eslint",
            "ritwickdey.liveserver",
            "formulahendry.auto-close-tag",
            "formulahendry.auto-rename-tag",
            "ms-vscode.vscode-serial-monitor",
            "toba.vsfire"
        ]
    },
    "tasks": {
        "version": "2.0.0",
        "tasks": [
            {
                "label": "Build ESP32",
                "type": "shell",
                "command": "platformio run",
                "options": {
                    "cwd": "${workspaceFolder}/hardware"
                },
                "group": "build"
            },
            {
                "label": "Upload ESP32",
                "type": "shell",
                "command": "platformio run --target upload",
                "options": {
                    "cwd": "${workspaceFolder}/hardware"
                }
            },
            {
                "label": "Start Web Server",
                "type": "shell",
                "command": "npm start",
                "options": {
                    "cwd": "${workspaceFolder}/web"
                }
            },
            {
                "label": "Deploy Firebase",
                "type": "shell",
                "command": "firebase deploy",
                "options": {
                    "cwd": "${workspaceFolder}/cloud"
                }
            }
        ]
    }
}
```

---

## 📦 Extensions VS Code Recommandées

### Installation automatique

1. Ouvrez VS Code
2. Ouvrez le fichier `.code-workspace`
3. VS Code vous proposera d'installer les extensions recommandées

### Extensions essentielles

#### Pour ESP32/Arduino
- **PlatformIO IDE** - Développement ESP32
- **C/C++** - IntelliSense pour C/C++
- **Serial Monitor** - Moniteur série intégré

#### Pour Web/JavaScript
- **Prettier** - Formateur de code
- **ESLint** - Linter JavaScript
- **Live Server** - Serveur web local
- **Auto Close Tag** - Fermeture auto des balises HTML
- **Auto Rename Tag** - Renommage des balises

#### Pour Firebase
- **VSFire** - Extension Firebase
- **Firebase Explorer** - Explorateur Firebase

#### Utilitaires
- **GitLens** - Git amélioré
- **Path Intellisense** - Autocomplétion des chemins
- **TODO Highlight** - Mise en évidence des TODO
- **Better Comments** - Commentaires colorés

---

## 🎨 Configuration .vscode/settings.json

Créez `.vscode/settings.json` :

```json
{
  "files.associations": {
    "*.ino": "cpp",
    "*.h": "c"
  },
  
  "editor.formatOnSave": true,
  "editor.minimap.enabled": true,
  "editor.suggestSelection": "first",
  "editor.fontSize": 14,
  "editor.lineHeight": 22,
  "editor.fontFamily": "'Fira Code', 'Consolas', 'Courier New', monospace",
  "editor.fontLigatures": true,
  
  "workbench.colorTheme": "One Dark Pro",
  "workbench.iconTheme": "material-icon-theme",
  
  "terminal.integrated.fontSize": 13,
  "terminal.integrated.fontFamily": "'Fira Code', monospace",
  
  "emmet.includeLanguages": {
    "javascript": "javascriptreact"
  },
  
  "files.exclude": {
    "**/.git": true,
    "**/.DS_Store": true,
    "**/node_modules": true,
    "**/.pio": true,
    "**/build": true
  },
  
  "search.exclude": {
    "**/node_modules": true,
    "**/build": true,
    "**/.pio": true
  }
}
```

---

## 🔌 Configuration PlatformIO pour ESP32

Créez `hardware/platformio.ini` :

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino

; Port série (ajustez selon votre système)
upload_port = /dev/ttyUSB0  ; Linux
; upload_port = COM3         ; Windows

; Vitesse de transmission
monitor_speed = 115200

; Bibliothèques
lib_deps = 
    bblanchon/ArduinoJson@^6.21.3
    claws/BH1750@^1.3.0
    adafruit/DHT sensor library@^1.4.4
    adafruit/Adafruit Unified Sensor@^1.1.9

; Options de build
build_flags = 
    -DCORE_DEBUG_LEVEL=3
    -DCONFIG_ARDUHAL_LOG_COLORS=1

; Options de téléversement
upload_speed = 921600
```

---

## 📝 .gitignore

Créez `.gitignore` à la racine :

```gitignore
# VS Code
.vscode/*
!.vscode/settings.json
!.vscode/tasks.json
!.vscode/extensions.json

# PlatformIO
.pio/
.pioenvs/
.piolibdeps/
build/

# Node modules
node_modules/
npm-debug.log*
yarn-debug.log*
yarn-error.log*

# Firebase
.firebase/
firebase-debug.log

# Configuration sensible
*.env
config.h
firebase-config.js

# OS
.DS_Store
Thumbs.db

# IDE
*.swp
*.swo
*~

# Builds
dist/
*.o
*.a
*.so
*.exe

# Logs
*.log
logs/

# Temporary files
tmp/
temp/
```

---

## 📂 Exemple : Organisation du Code ESP32

### Structure modulaire

#### `hardware/src/main.ino`
```cpp
#include "config.h"
#include "sensors.h"
#include "wifi_manager.h"
#include "firebase_client.h"
#include "comfort_calculator.h"

Sensors sensors;
WiFiManager wifiManager;
FirebaseClient firebaseClient;
ComfortCalculator comfortCalc;

void setup() {
    Serial.begin(115200);
    
    sensors.init();
    wifiManager.connect();
    firebaseClient.init();
}

void loop() {
    SensorData data = sensors.readAll();
    int score = comfortCalc.calculate(data);
    firebaseClient.send(data, score);
    
    delay(5000);
}
```

#### `hardware/src/sensors.h`
```cpp
#ifndef SENSORS_H
#define SENSORS_H

#include <BH1750.h>
#include <DHT.h>

struct SensorData {
    float light;
    float noise;
    float temperature;
    float humidity;
};

class Sensors {
private:
    BH1750 lightMeter;
    DHT dht;
    
public:
    void init();
    SensorData readAll();
    float readLight();
    float readNoise();
    void readTempHumidity(float &temp, float &hum);
};

#endif
```

#### `hardware/config/config.h`
```cpp
#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Firebase Configuration
#define FIREBASE_HOST "your-project.firebaseio.com"
#define FIREBASE_PATH "/study_desk_monitor/readings.json"

// Sensor Pins
#define DHT_PIN 4
#define SOUND_PIN 34

// Settings
#define READING_INTERVAL 5000

#endif
```

---

## 🌐 Exemple : Organisation du Code Web

### Structure modulaire

#### `web/src/services/firebase.js`
```javascript
// Service Firebase isolé
import { initializeApp } from 'firebase/app';
import { getDatabase } from 'firebase/database';

const firebaseConfig = {
    // Configuration...
};

const app = initializeApp(firebaseConfig);
export const database = getDatabase(app);
```

#### `web/src/services/dataService.js`
```javascript
// Service de gestion des données
import { database } from './firebase';
import { ref, onValue } from 'firebase/database';

export class DataService {
    static listenToReadings(callback) {
        const readingsRef = ref(database, 'study_desk_monitor/readings');
        return onValue(readingsRef, (snapshot) => {
            callback(snapshot.val());
        });
    }
}
```

#### `web/src/components/ComfortGauge.js`
```javascript
// Composant jauge de confort
export class ComfortGauge {
    constructor(canvasId) {
        this.canvas = document.getElementById(canvasId);
        this.chart = null;
    }
    
    update(score) {
        // Mise à jour du graphique...
    }
}
```

---

## 🚀 Commandes VS Code Utiles

### Terminal intégré (Ctrl + `)

```bash
# ESP32
cd hardware
pio run              # Compiler
pio run -t upload    # Téléverser
pio device monitor   # Moniteur série

# Web
cd web
npm install          # Installer dépendances
npm start           # Démarrer serveur dev

# Firebase
cd cloud
firebase login       # Se connecter
firebase deploy     # Déployer
```

### Raccourcis clavier essentiels

- `Ctrl + P` : Ouvrir un fichier rapidement
- `Ctrl + Shift + P` : Palette de commandes
- `Ctrl + B` : Afficher/masquer sidebar
- `Ctrl + J` : Afficher/masquer terminal
- `Ctrl + K Ctrl + O` : Ouvrir dossier
- `Alt + ↑/↓` : Déplacer ligne
- `Ctrl + D` : Sélection multiple

---

## 📋 Scripts package.json (Web)

Créez `web/package.json` :

```json
{
  "name": "smart-study-desk-monitor-web",
  "version": "1.0.0",
  "scripts": {
    "start": "live-server public --port=8080",
    "build": "webpack --mode production",
    "test": "jest",
    "lint": "eslint src/**/*.js",
    "format": "prettier --write src/**/*.{js,css,html}",
    "deploy": "firebase deploy"
  },
  "devDependencies": {
    "eslint": "^8.0.0",
    "prettier": "^2.8.0",
    "live-server": "^1.2.2"
  },
  "dependencies": {
    "firebase": "^10.8.0",
    "chart.js": "^4.4.0"
  }
}
```

---

## 💡 Bonnes Pratiques

### 1. Séparation des préoccupations
- Un fichier = Une responsabilité
- Modules réutilisables
- Pas de duplication de code

### 2. Nommage cohérent
- **Fichiers** : `snake_case.cpp` ou `kebab-case.js`
- **Classes** : `PascalCase`
- **Fonctions** : `camelCase`
- **Constantes** : `UPPER_SNAKE_CASE`

### 3. Commentaires
```cpp
/**
 * @brief Calcule le score de confort
 * @param light Luminosité en Lux
 * @param noise Niveau sonore en dB
 * @param temp Température en °C
 * @return Score de 0 à 100
 */
int calculateComfortScore(float light, float noise, float temp);
```

### 4. Gestion des erreurs
```javascript
try {
    await firebaseClient.send(data);
} catch (error) {
    console.error('Erreur Firebase:', error);
    // Gestion de l'erreur
}
```

---

## ✅ Checklist de Setup

- [ ] Installer VS Code
- [ ] Installer extensions recommandées
- [ ] Créer structure de dossiers
- [ ] Configurer workspace
- [ ] Installer PlatformIO (pour ESP32)
- [ ] Installer Node.js (pour web)
- [ ] Cloner/créer repository Git
- [ ] Configurer .gitignore
- [ ] Tester compilation ESP32
- [ ] Tester serveur web local

---

Votre projet est maintenant organisé de manière professionnelle ! 🎉

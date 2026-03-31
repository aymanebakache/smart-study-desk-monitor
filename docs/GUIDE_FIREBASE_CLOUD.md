# Guide Complet : Configuration Firebase Cloud
## Smart Study Desk Monitor - Backend & Cloud

---

## Table des matières
1. [Création du projet Firebase](#1-création-du-projet-firebase)
2. [Configuration Realtime Database](#2-configuration-realtime-database)
3. [Règles de sécurité](#3-règles-de-sécurité)
4. [Structure de données](#4-structure-de-données)
5. [Code ESP32 pour envoi](#5-code-esp32-pour-envoi)
6. [Code Dashboard Web](#6-code-dashboard-web)
7. [Tests et débogage](#7-tests-et-débogage)

---

## 1. Création du projet Firebase

### Étape 1.1 : Créer un compte Google
- Allez sur https://console.firebase.google.com/
- Connectez-vous avec votre compte Google

### Étape 1.2 : Créer un nouveau projet
1. Cliquez sur "Ajouter un projet"
2. Nom du projet : `smart-study-desk-monitor`
3. Désactivez Google Analytics (pas nécessaire pour ce projet)
4. Cliquez sur "Créer le projet"

### Étape 1.3 : Configuration initiale
Une fois le projet créé, vous serez redirigé vers le tableau de bord Firebase.

---

## 2. Configuration Realtime Database

### Étape 2.1 : Activer Realtime Database
1. Dans le menu de gauche, cliquez sur **"Realtime Database"**
2. Cliquez sur **"Créer une base de données"**
3. Sélectionnez une région (par exemple : `europe-west1`)
4. Mode de sécurité : **Choisir "Mode test"** (temporairement)
5. Cliquez sur **"Activer"**

### Étape 2.2 : Récupérer l'URL de la base
Votre URL ressemblera à :
```
https://smart-study-desk-monitor-default-rtdb.europe-west1.firebasedatabase.app/
```
**💡 Notez cette URL, vous en aurez besoin !**

### Étape 2.3 : Structure initiale de la base
Dans l'interface Firebase Console, créez cette structure initiale :

```json
{
  "study_desk_monitor": {
    "readings": {},
    "alerts": {},
    "settings": {
      "thresholds": {
        "light_min": 300,
        "light_max": 500,
        "noise_min": 30,
        "noise_max": 50,
        "temp_min": 20,
        "temp_max": 24
      },
      "update_interval": 5000
    }
  }
}
```

---

## 3. Règles de sécurité

### Étape 3.1 : Configuration des règles
Dans Firebase Console > Realtime Database > Onglet "Règles"

Remplacez les règles par :

```json
{
  "rules": {
    "study_desk_monitor": {
      "readings": {
        ".write": true,
        ".read": true,
        "$reading_id": {
          ".validate": "newData.hasChildren(['timestamp', 'light', 'noise', 'temperature'])"
        }
      },
      "alerts": {
        ".write": true,
        ".read": true
      },
      "settings": {
        ".read": true,
        ".write": false
      }
    }
  }
}
```

### Explication des règles :
- **readings** : Lecture et écriture autorisées (pour ESP32 et Dashboard)
- **alerts** : Lecture et écriture autorisées
- **settings** : Lecture seule (modifiable uniquement depuis la console)
- **Validation** : Vérifie que les données contiennent les champs obligatoires

**⚠️ IMPORTANT** : Pour la production, ajoutez une authentification !

---

## 4. Structure de données

### Format d'un enregistrement de lecture

```json
{
  "study_desk_monitor": {
    "readings": {
      "1710084625000": {
        "timestamp": "2024-03-10T14:30:25Z",
        "light": 450,
        "noise": 45,
        "temperature": 22.5,
        "humidity": 55,
        "comfort_score": 85
      },
      "1710084630000": {
        "timestamp": "2024-03-10T14:30:30Z",
        "light": 448,
        "noise": 46,
        "temperature": 22.4,
        "humidity": 55,
        "comfort_score": 84
      }
    }
  }
}
```

### Format d'une alerte

```json
{
  "study_desk_monitor": {
    "alerts": {
      "1710084625001": {
        "type": "low_light",
        "message": "Éclairage insuffisant détecté",
        "timestamp": "2024-03-10T14:35:00Z",
        "value": 180,
        "severity": "warning"
      }
    }
  }
}
```

---

## 5. Code ESP32 pour envoi

Voici le code complet pour l'ESP32 qui envoie les données à Firebase.

### Installation des bibliothèques Arduino

Dans Arduino IDE :
1. Allez dans **Outils > Gérer les bibliothèques**
2. Installez :
   - `ArduinoJson` by Benoit Blanchon
   - `BH1750` by Christopher Laws
   - `DHT sensor library` by Adafruit

### Configuration WiFi et Firebase

```cpp
// Remplacez par vos informations
#define WIFI_SSID "VOTRE_WIFI_SSID"
#define WIFI_PASSWORD "VOTRE_WIFI_PASSWORD"

// URL Firebase (remplacez par votre URL)
#define FIREBASE_HOST "smart-study-desk-monitor-default-rtdb.europe-west1.firebasedatabase.app"
```

### Code complet ESP32

Voir le fichier `esp32_firebase_client.ino` fourni séparément.

---

## 6. Code Dashboard Web

### Étape 6.1 : Structure du projet web

```
dashboard/
├── index.html
├── css/
│   └── style.css
├── js/
│   ├── firebase-config.js
│   └── dashboard.js
└── README.md
```

### Étape 6.2 : Configuration Firebase SDK

Créez un fichier `firebase-config.js` :

```javascript
// Configuration Firebase (obtenez ces valeurs depuis Firebase Console)
const firebaseConfig = {
  apiKey: "VOTRE_API_KEY",
  authDomain: "smart-study-desk-monitor.firebaseapp.com",
  databaseURL: "https://smart-study-desk-monitor-default-rtdb.europe-west1.firebasedatabase.app",
  projectId: "smart-study-desk-monitor",
  storageBucket: "smart-study-desk-monitor.appspot.com",
  messagingSenderId: "123456789",
  appId: "1:123456789:web:abcdef123456"
};

// Initialiser Firebase
firebase.initializeApp(firebaseConfig);
const database = firebase.database();
```

**Pour obtenir votre configuration :**
1. Firebase Console > Paramètres du projet (icône engrenage)
2. Faites défiler jusqu'à "Vos applications"
3. Cliquez sur l'icône Web `</>`
4. Copiez l'objet `firebaseConfig`

### Étape 6.3 : Dashboard HTML complet

Voir le fichier `index.html` fourni séparément.

---

## 7. Tests et débogage

### Test 1 : Envoyer des données manuellement

Utilisez cURL ou Postman pour tester l'API Firebase :

```bash
curl -X POST \
  'https://VOTRE-PROJET.firebaseio.com/study_desk_monitor/readings.json' \
  -H 'Content-Type: application/json' \
  -d '{
    "timestamp": "2024-03-10T14:30:00Z",
    "light": 450,
    "noise": 45,
    "temperature": 22.5,
    "humidity": 55,
    "comfort_score": 85
  }'
```

### Test 2 : Vérifier les données dans Firebase Console

1. Ouvrez Firebase Console
2. Allez dans Realtime Database
3. Vous devriez voir vos données apparaître en temps réel

### Test 3 : Console de débogage ESP32

Ajoutez ces lignes dans votre code ESP32 :

```cpp
Serial.println("Connexion WiFi...");
Serial.println("WiFi connecté!");
Serial.print("Adresse IP: ");
Serial.println(WiFi.localIP());
Serial.println("Envoi des données...");
Serial.println(httpResponseCode);
```

### Moniteur série Arduino IDE :
- Outils > Moniteur série
- Baud rate : 115200

---

## Checklist de déploiement

### Configuration Firebase
- [ ] Projet Firebase créé
- [ ] Realtime Database activée
- [ ] URL de la base notée
- [ ] Règles de sécurité configurées
- [ ] Structure de données créée

### Code ESP32
- [ ] Bibliothèques installées
- [ ] WiFi configuré (SSID/Password)
- [ ] URL Firebase configurée
- [ ] Code téléversé sur ESP32
- [ ] Moniteur série vérifié

### Dashboard Web
- [ ] Configuration Firebase SDK ajoutée
- [ ] Firebase initialisé correctement
- [ ] HTML/CSS/JS créés
- [ ] Tests de lecture temps réel effectués

### Tests
- [ ] Envoi manuel de données réussi
- [ ] ESP32 envoie des données
- [ ] Dashboard affiche les données
- [ ] Graphiques fonctionnels
- [ ] Score de confort calculé correctement

---

## Dépannage courant

### Problème : ESP32 ne se connecte pas au WiFi
**Solution :**
- Vérifiez le SSID et le mot de passe
- Assurez-vous que le WiFi est en 2.4 GHz (ESP32 ne supporte pas 5 GHz)
- Vérifiez que le WiFi n'a pas de portail captif

### Problème : Erreur 401 Unauthorized
**Solution :**
- Vérifiez les règles de sécurité Firebase
- Mode test devrait autoriser les lectures/écritures

### Problème : Données n'apparaissent pas dans le dashboard
**Solution :**
- Vérifiez la console JavaScript (F12 dans le navigateur)
- Vérifiez que l'URL Firebase est correcte
- Assurez-vous que Firebase SDK est chargé

### Problème : Erreur CORS
**Solution :**
- Firebase gère CORS automatiquement
- Si problème persiste, testez avec Firebase Hosting

---

## Ressources supplémentaires

- [Documentation Firebase Realtime Database](https://firebase.google.com/docs/database)
- [Guide ESP32 WiFi](https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/)
- [Firebase SDK Web](https://firebase.google.com/docs/web/setup)
- [Chart.js Documentation](https://www.chartjs.org/docs/)

---

## Prochaines étapes

1. ✅ Configuration Firebase terminée
2. 📝 Développer le code ESP32
3. 🌐 Créer le dashboard web
4. 🧪 Tests d'intégration
5. 📊 Optimisation et améliorations

---

**Date de création :** Mars 2024  
**Version :** 1.0  
**Auteur :** Projet Académique IoT - 3 Étudiants

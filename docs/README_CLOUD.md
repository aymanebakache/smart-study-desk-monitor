# Smart Study Desk Monitor - Partie Cloud

## 📦 Contenu du package

Vous avez reçu tous les fichiers nécessaires pour implémenter la partie Cloud de votre projet IoT :

### Fichiers inclus

1. **GUIDE_FIREBASE_CLOUD.md** - Guide complet étape par étape
2. **esp32_firebase_client.ino** - Code Arduino pour ESP32
3. **dashboard.html** - Interface web dashboard complète
4. **firebase-config.js** - Configuration Firebase
5. **firebase-rules.json** - Règles de sécurité Firebase
6. **firebase-test.html** - Page de test Firebase
7. **README_CLOUD.md** - Ce fichier

---

## 🚀 Démarrage rapide (15 minutes)

### Étape 1 : Créer le projet Firebase (5 min)

1. Allez sur https://console.firebase.google.com/
2. Cliquez sur "Ajouter un projet"
3. Nom : `smart-study-desk-monitor`
4. Désactivez Google Analytics
5. Cliquez sur "Créer le projet"

### Étape 2 : Activer Realtime Database (3 min)

1. Menu gauche → **Realtime Database**
2. Cliquez sur **"Créer une base de données"**
3. Région : `europe-west1`
4. Mode : **Mode test** (pour commencer)
5. Cliquez sur **"Activer"**

**💡 NOTEZ VOTRE URL :** 
```
https://smart-study-desk-monitor-default-rtdb.europe-west1.firebasedatabase.app/
```

### Étape 3 : Configurer les règles de sécurité (2 min)

1. Dans Realtime Database, onglet **"Règles"**
2. Copiez-collez le contenu de `firebase-rules.json`
3. Cliquez sur **"Publier"**

### Étape 4 : Obtenir la configuration Web (2 min)

1. Cliquez sur l'icône ⚙️ → **Paramètres du projet**
2. Section "Vos applications" → Icône Web `</>`
3. Nom de l'app : "Dashboard"
4. **COPIEZ** l'objet `firebaseConfig`

### Étape 5 : Tester la configuration (3 min)

1. Ouvrez `firebase-test.html` dans votre navigateur
2. Remplacez la configuration Firebase avec vos valeurs
3. Cliquez sur "🔌 Tester la connexion"
4. Si ✓ apparaît, c'est bon !
5. Testez "📤 Envoyer des données de test"

---

## 📱 Configuration ESP32

### Bibliothèques Arduino nécessaires

Ouvrez Arduino IDE → Outils → Gérer les bibliothèques :

```
1. ArduinoJson (by Benoit Blanchon)
2. BH1750 (by Christopher Laws)  
3. DHT sensor library (by Adafruit)
```

### Modifier le code ESP32

Ouvrez `esp32_firebase_client.ino` et modifiez :

```cpp
// Ligne 25-26 : Vos identifiants WiFi
#define WIFI_SSID "VOTRE_WIFI_SSID"
#define WIFI_PASSWORD "VOTRE_WIFI_PASSWORD"

// Ligne 32 : Votre URL Firebase (sans https://)
#define FIREBASE_HOST "votre-projet-default-rtdb.europe-west1.firebasedatabase.app"
```

### Téléverser le code

1. Branchez l'ESP32 via USB
2. Arduino IDE → Outils → Port → Sélectionnez votre ESP32
3. Arduino IDE → Outils → Type de carte → ESP32 Dev Module
4. Cliquez sur **Téléverser** (→)
5. Ouvrez le **Moniteur série** (115200 baud)

---

## 🌐 Configuration Dashboard Web

### Méthode 1 : Fichier local (simple)

1. Ouvrez `dashboard.html` avec un éditeur de texte
2. Ligne 489-496 : Remplacez par votre configuration Firebase
3. Enregistrez le fichier
4. Ouvrez `dashboard.html` dans Chrome/Firefox

### Méthode 2 : Firebase Hosting (recommandé)

```bash
# Installer Firebase CLI
npm install -g firebase-tools

# Se connecter à Firebase
firebase login

# Initialiser le projet
firebase init hosting

# Choisir votre projet
# Répertoire public : public
# Page index : index.html (renommez dashboard.html)

# Déployer
firebase deploy
```

Votre dashboard sera accessible à :
```
https://smart-study-desk-monitor.web.app
```

---

## 🔧 Structure de la base de données

```json
study_desk_monitor/
├── readings/
│   ├── 1710084625000/
│   │   ├── timestamp: "2024-03-10T14:30:25Z"
│   │   ├── light: 450
│   │   ├── noise: 45
│   │   ├── temperature: 22.5
│   │   ├── humidity: 55
│   │   └── comfort_score: 85
│   └── ...
├── alerts/
│   └── ...
└── settings/
    └── thresholds/
        ├── light_min: 300
        ├── light_max: 500
        ├── noise_min: 30
        ├── noise_max: 50
        ├── temp_min: 20
        └── temp_max: 24
```

---

## ✅ Checklist de vérification

### Firebase
- [ ] Projet créé
- [ ] Realtime Database activée
- [ ] URL de la base notée
- [ ] Règles de sécurité configurées
- [ ] Configuration Web récupérée

### ESP32
- [ ] Bibliothèques installées
- [ ] WiFi configuré
- [ ] Firebase URL configurée
- [ ] Code téléversé
- [ ] Moniteur série affiche "✓ WiFi connecté"
- [ ] Moniteur série affiche "✓ Données envoyées"

### Dashboard
- [ ] Configuration Firebase ajoutée
- [ ] Dashboard s'ouvre dans le navigateur
- [ ] État "Connecté" affiché
- [ ] Données s'affichent en temps réel

---

## 🐛 Dépannage

### Problème : ESP32 ne se connecte pas au WiFi

**Solutions :**
- Vérifiez SSID et mot de passe (sensible à la casse)
- Assurez-vous que le WiFi est en 2.4 GHz (ESP32 ne supporte pas 5 GHz)
- Vérifiez que le WiFi n'a pas de portail captif

### Problème : Erreur 401 Unauthorized

**Solutions :**
- Vérifiez les règles Firebase (onglet "Règles")
- Mode test doit autoriser `.read: true` et `.write: true`

### Problème : Dashboard ne reçoit pas de données

**Solutions :**
- F12 dans le navigateur → Console → Vérifiez les erreurs
- Vérifiez que la configuration Firebase est correcte
- Vérifiez que l'ESP32 envoie des données (moniteur série)

### Problème : CORS Error

**Solutions :**
- Firebase gère CORS automatiquement
- Utilisez Firebase Hosting au lieu d'ouvrir le fichier local
- Ou utilisez un serveur local : `python -m http.server 8000`

---

## 📊 Tester le système complet

### Test end-to-end (bout en bout)

1. **ESP32 allumé** → Moniteur série affiche lectures
2. **Firebase Console** → Onglet "Données" → Voir les données arriver
3. **Dashboard** → Actualisation automatique des valeurs

### Simulation de données

Si vous n'avez pas encore les capteurs :

1. Ouvrez `firebase-test.html`
2. Cliquez sur "🎲 Envoyer des données aléatoires" plusieurs fois
3. Regardez le dashboard se mettre à jour

---

## 🎯 Prochaines étapes

Une fois que tout fonctionne :

### Améliorations possibles

1. **Authentification** : Ajouter Firebase Authentication
2. **Notifications** : Firebase Cloud Messaging pour alertes push
3. **Machine Learning** : Prédiction des tendances avec ML
4. **Export de données** : Bouton pour télécharger CSV
5. **Graphiques avancés** : Histogrammes, heat maps
6. **Mode sombre** : Toggle dark/light mode
7. **Multi-utilisateurs** : Plusieurs bureaux surveillés

### Optimisations

1. **Nettoyage automatique** : Cloud Functions pour supprimer anciennes données
2. **Compression** : Agréger les données par heure/jour
3. **Cache** : localStorage pour données offline
4. **PWA** : Transformer en Progressive Web App

---

## 📚 Ressources

### Documentation officielle
- [Firebase Realtime Database](https://firebase.google.com/docs/database)
- [ESP32 Arduino Core](https://docs.espressif.com/projects/arduino-esp32/)
- [Chart.js](https://www.chartjs.org/docs/)

### Tutoriels recommandés
- [Random Nerd Tutorials - ESP32](https://randomnerdtutorials.com/projects-esp32/)
- [Firebase Quickstart](https://firebase.google.com/docs/database/web/start)

### Communauté
- [Arduino Forum](https://forum.arduino.cc/)
- [ESP32 Reddit](https://www.reddit.com/r/esp32/)
- [Stack Overflow - Firebase tag](https://stackoverflow.com/questions/tagged/firebase)

---

## 🎓 Pour votre rapport académique

### Points à mentionner

1. **Architecture Cloud** : Expliquez pourquoi Firebase
2. **Temps réel** : WebSocket vs HTTP polling
3. **Sécurité** : Règles Firebase, HTTPS
4. **Scalabilité** : NoSQL vs SQL pour IoT
5. **Coût** : Firebase gratuit jusqu'à 1GB/jour

### Diagrammes à inclure

- Architecture 3-tiers (IoT → Cloud → Web)
- Flux de données (sequence diagram)
- Structure de la base de données
- Calcul du score de confort

---

## 👥 Répartition des tâches

### Étudiant 2 (Backend & Cloud)

**Responsabilités principales :**
- Configuration Firebase
- Définition structure de données
- Règles de sécurité
- Tests d'intégration

**Livrables :**
- Base de données opérationnelle
- Documentation des règles
- Tests de charge (si applicable)

**Temps estimé :** 8-10 heures

---

## 📝 Notes finales

- Toujours utiliser HTTPS (Firebase le fait automatiquement)
- Ne jamais commiter vos clés API sur GitHub (utilisez `.gitignore`)
- Testez régulièrement pendant le développement
- Documentez vos choix techniques

**Bon courage pour votre projet ! 🚀**

---

**Dernière mise à jour :** Mars 2024  
**Version :** 1.0  
**Auteurs :** Projet Académique IoT - 3 Étudiants

/*
 * Smart Study Desk Monitor - ESP32 Client
 * 
 * Ce programme lit les données des capteurs et les envoie à Firebase
 * 
 * Capteurs utilisés :
 * - BH1750 : Luminosité (I2C)
 * - DHT22 : Température et Humidité (GPIO 4)
 * - KY-038 : Niveau sonore (ADC GPIO 34)
 * 
 * Auteur : Projet Académique IoT
 * Date : Mars 2024
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <BH1750.h>
#include <DHT.h>
#include <time.h>

// ========================================
// CONFIGURATION WiFi
// ========================================
#define WIFI_SSID "VOTRE_WIFI_SSID"          // Remplacez par votre SSID WiFi
#define WIFI_PASSWORD "VOTRE_WIFI_PASSWORD"  // Remplacez par votre mot de passe WiFi

// ========================================
// CONFIGURATION Firebase
// ========================================
// Remplacez par l'URL de votre projet Firebase
#define FIREBASE_HOST "smart-study-desk-monitor-default-rtdb.europe-west1.firebasedatabase.app"
#define FIREBASE_PATH "/study_desk_monitor/readings.json"

// ========================================
// CONFIGURATION des Capteurs
// ========================================
#define DHT_PIN 4              // Pin du DHT22
#define DHT_TYPE DHT22         // Type de capteur DHT
#define SOUND_PIN 34           // Pin ADC pour le capteur sonore (KY-038)

// Intervalle de lecture (millisecondes)
#define READING_INTERVAL 5000  // 5 secondes

// ========================================
// Objets des capteurs
// ========================================
BH1750 lightMeter;             // Capteur de luminosité
DHT dht(DHT_PIN, DHT_TYPE);    // Capteur température/humidité

// ========================================
// Variables globales
// ========================================
unsigned long lastReading = 0;
int readingCount = 0;

// Configuration NTP pour l'heure
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;

// ========================================
// Structures de données
// ========================================
struct SensorData {
  float light;           // Luminosité en Lux
  float noise;           // Niveau sonore en dB
  float temperature;     // Température en °C
  float humidity;        // Humidité en %
  int comfortScore;      // Score de confort 0-100
  String timestamp;      // Horodatage ISO 8601
};

// ========================================
// Fonctions utilitaires
// ========================================

/**
 * Initialise la connexion WiFi
 */
void setupWiFi() {
  Serial.println("\n========================================");
  Serial.println("Connexion WiFi en cours...");
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi connecté avec succès!");
    Serial.print("Adresse IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Force du signal: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\n✗ Échec de connexion WiFi!");
    Serial.println("Vérifiez votre SSID et mot de passe");
  }
  Serial.println("========================================\n");
}

/**
 * Initialise les capteurs
 */
void setupSensors() {
  Serial.println("Initialisation des capteurs...");
  
  // Initialisation I2C
  Wire.begin();
  
  // Initialisation BH1750 (Luminosité)
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("✓ BH1750 (Luminosité) initialisé");
  } else {
    Serial.println("✗ Erreur BH1750 - Vérifiez les connexions I2C");
  }
  
  // Initialisation DHT22 (Température/Humidité)
  dht.begin();
  Serial.println("✓ DHT22 (Température/Humidité) initialisé");
  
  // Initialisation capteur sonore (ADC)
  pinMode(SOUND_PIN, INPUT);
  Serial.println("✓ KY-038 (Son) initialisé sur GPIO 34");
  
  Serial.println("Capteurs prêts!\n");
}

/**
 * Configure la synchronisation NTP pour obtenir l'heure
 */
void setupTime() {
  Serial.println("Configuration de l'heure NTP...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println("✓ Heure synchronisée");
    Serial.println(&timeinfo, "Date/Heure: %A, %B %d %Y %H:%M:%S");
  } else {
    Serial.println("✗ Échec synchronisation NTP");
  }
  Serial.println();
}

/**
 * Obtient l'horodatage actuel au format ISO 8601
 */
String getTimestamp() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "1970-01-01T00:00:00Z";
  }
  
  char buffer[25];
  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
  return String(buffer);
}

/**
 * Lit la luminosité depuis le BH1750
 * Retourne la valeur en Lux
 */
float readLight() {
  float lux = lightMeter.readLightLevel();
  
  // Validation de la valeur
  if (lux < 0) {
    Serial.println("⚠ Erreur lecture BH1750");
    return 0;
  }
  
  return lux;
}

/**
 * Lit la température et l'humidité depuis le DHT22
 */
void readTempHumidity(float &temp, float &hum) {
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  
  // Validation des valeurs
  if (isnan(temp) || isnan(hum)) {
    Serial.println("⚠ Erreur lecture DHT22");
    temp = 0;
    hum = 0;
  }
}

/**
 * Lit le niveau sonore depuis le KY-038
 * Retourne une valeur approximative en dB
 */
float readNoise() {
  // Lecture multiple pour moyenne
  const int samples = 50;
  long sum = 0;
  
  for (int i = 0; i < samples; i++) {
    sum += analogRead(SOUND_PIN);
    delayMicroseconds(100);
  }
  
  float average = sum / (float)samples;
  
  // Conversion approximative ADC -> dB
  // Formule simplifiée : dB ≈ 20 * log10(ADC / référence) + offset
  // Cette formule doit être calibrée selon votre capteur
  float db = map(average, 0, 4095, 30, 100); // Mapping basique 30-100 dB
  
  return db;
}

/**
 * Calcul du score de confort basé sur les paramètres environnementaux
 * Retourne un score de 0 à 100
 */
int calculateComfortScore(float light, float noise, float temp) {
  int lightScore = 0;
  int noiseScore = 0;
  int tempScore = 0;
  
  // Score de luminosité (poids: 40%)
  if (light < 200) {
    lightScore = map(light, 0, 200, 0, 50);
  } else if (light >= 200 && light < 300) {
    lightScore = map(light, 200, 300, 50, 75);
  } else if (light >= 300 && light <= 500) {
    lightScore = map(light, 300, 500, 90, 100);
  } else {
    lightScore = map(light, 500, 1000, 90, 75);
    if (lightScore < 75) lightScore = 75;
  }
  
  // Score de bruit (poids: 30%)
  if (noise < 30) {
    noiseScore = 100;
  } else if (noise >= 30 && noise <= 50) {
    noiseScore = map(noise, 30, 50, 100, 90);
  } else if (noise > 50 && noise <= 70) {
    noiseScore = map(noise, 50, 70, 75, 50);
  } else {
    noiseScore = map(noise, 70, 100, 50, 0);
  }
  
  // Score de température (poids: 30%)
  if (temp < 18) {
    tempScore = map(temp, 10, 18, 40, 60);
  } else if (temp >= 18 && temp < 20) {
    tempScore = map(temp, 18, 20, 70, 85);
  } else if (temp >= 20 && temp <= 24) {
    tempScore = map(temp, 20, 24, 90, 100);
  } else {
    tempScore = map(temp, 24, 30, 80, 60);
    if (tempScore < 60) tempScore = 60;
  }
  
  // Calcul du score final avec pondération
  int finalScore = (lightScore * 0.4) + (noiseScore * 0.3) + (tempScore * 0.3);
  
  // Contraindre entre 0 et 100
  if (finalScore < 0) finalScore = 0;
  if (finalScore > 100) finalScore = 100;
  
  return finalScore;
}

/**
 * Lit tous les capteurs et retourne les données
 */
SensorData readAllSensors() {
  SensorData data;
  
  // Lecture des capteurs
  data.light = readLight();
  data.noise = readNoise();
  readTempHumidity(data.temperature, data.humidity);
  
  // Calcul du score de confort
  data.comfortScore = calculateComfortScore(data.light, data.noise, data.temperature);
  
  // Horodatage
  data.timestamp = getTimestamp();
  
  return data;
}

/**
 * Affiche les données des capteurs dans le moniteur série
 */
void printSensorData(SensorData &data) {
  Serial.println("┌─────────────────────────────────────┐");
  Serial.printf("│ Lecture #%-3d                      │\n", ++readingCount);
  Serial.println("├─────────────────────────────────────┤");
  Serial.printf("│ 💡 Luminosité:    %6.1f Lux     │\n", data.light);
  Serial.printf("│ 🔊 Bruit:         %6.1f dB      │\n", data.noise);
  Serial.printf("│ 🌡️  Température:   %6.1f °C      │\n", data.temperature);
  Serial.printf("│ 💧 Humidité:      %6.1f %%       │\n", data.humidity);
  Serial.println("├─────────────────────────────────────┤");
  Serial.printf("│ 🎯 Score Confort: %3d/100          │\n", data.comfortScore);
  Serial.println("└─────────────────────────────────────┘");
  Serial.println();
}

/**
 * Envoie les données à Firebase via HTTP POST
 */
bool sendToFirebase(SensorData &data) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("✗ WiFi déconnecté, impossible d'envoyer les données");
    return false;
  }
  
  HTTPClient http;
  
  // Construction de l'URL complète
  String url = "https://" + String(FIREBASE_HOST) + FIREBASE_PATH;
  
  // Début de la requête HTTP
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  // Création du JSON
  StaticJsonDocument<256> doc;
  doc["timestamp"] = data.timestamp;
  doc["light"] = round(data.light * 10) / 10.0;  // Arrondi à 1 décimale
  doc["noise"] = round(data.noise * 10) / 10.0;
  doc["temperature"] = round(data.temperature * 10) / 10.0;
  doc["humidity"] = round(data.humidity * 10) / 10.0;
  doc["comfort_score"] = data.comfortScore;
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  // Affichage du JSON pour débogage
  Serial.println("📤 Envoi des données à Firebase...");
  Serial.print("URL: ");
  Serial.println(url);
  Serial.print("JSON: ");
  Serial.println(jsonString);
  
  // Envoi de la requête POST
  int httpResponseCode = http.POST(jsonString);
  
  // Traitement de la réponse
  bool success = false;
  if (httpResponseCode > 0) {
    Serial.print("✓ Réponse HTTP: ");
    Serial.println(httpResponseCode);
    
    if (httpResponseCode == 200) {
      String response = http.getString();
      Serial.println("✓ Données envoyées avec succès!");
      Serial.print("Réponse: ");
      Serial.println(response);
      success = true;
    }
  } else {
    Serial.print("✗ Erreur HTTP: ");
    Serial.println(http.errorToString(httpResponseCode));
  }
  
  http.end();
  Serial.println();
  
  return success;
}

/**
 * Vérifie les conditions et génère des alertes si nécessaire
 */
void checkAlerts(SensorData &data) {
  // Seuils d'alerte
  const float LIGHT_MIN = 300;
  const float NOISE_MAX = 70;
  const float TEMP_MIN = 18;
  const float TEMP_MAX = 26;
  
  // Alerte luminosité faible
  if (data.light < LIGHT_MIN) {
    Serial.println("⚠️  ALERTE: Luminosité insuffisante!");
  }
  
  // Alerte bruit élevé
  if (data.noise > NOISE_MAX) {
    Serial.println("⚠️  ALERTE: Niveau sonore trop élevé!");
  }
  
  // Alerte température
  if (data.temperature < TEMP_MIN) {
    Serial.println("⚠️  ALERTE: Température trop basse!");
  } else if (data.temperature > TEMP_MAX) {
    Serial.println("⚠️  ALERTE: Température trop élevée!");
  }
}

// ========================================
// Setup - Exécuté une seule fois au démarrage
// ========================================
void setup() {
  // Initialisation du port série
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n");
  Serial.println("╔═══════════════════════════════════════╗");
  Serial.println("║  Smart Study Desk Monitor - ESP32    ║");
  Serial.println("║  Système IoT de surveillance d'étude  ║");
  Serial.println("╚═══════════════════════════════════════╝");
  Serial.println();
  
  // Connexion WiFi
  setupWiFi();
  
  // Configuration de l'heure
  if (WiFi.status() == WL_CONNECTED) {
    setupTime();
  }
  
  // Initialisation des capteurs
  setupSensors();
  
  Serial.println("✓ Système prêt - Début des lectures\n");
}

// ========================================
// Loop - Exécuté en boucle
// ========================================
void loop() {
  unsigned long currentMillis = millis();
  
  // Vérifier si c'est le moment de faire une lecture
  if (currentMillis - lastReading >= READING_INTERVAL) {
    lastReading = currentMillis;
    
    // Lecture des capteurs
    SensorData data = readAllSensors();
    
    // Affichage dans le moniteur série
    printSensorData(data);
    
    // Vérification des alertes
    checkAlerts(data);
    
    // Envoi à Firebase
    if (WiFi.status() == WL_CONNECTED) {
      sendToFirebase(data);
    } else {
      Serial.println("✗ WiFi déconnecté, reconnexion...");
      setupWiFi();
    }
    
    Serial.println("─────────────────────────────────────────");
    Serial.printf("Prochaine lecture dans %d secondes\n", READING_INTERVAL / 1000);
    Serial.println("─────────────────────────────────────────\n");
  }
}

nrf#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Broches NRF24L01
#define CE_PIN   9
#define CSN_PIN 10

// Adresse de communication
const byte slaveAddress[5] = {'R','x','A','A','A'};

// Création de l'objet radio
RF24 radio(CE_PIN, CSN_PIN);

// Structure des données envoyées
struct data {
  char direction;  // MARCHE_AVANT = 'V', MARCHE_ARRIERE = 'R'
  char etat;       // DEMARRE = 'D', STOP = 'S'
};

// Instance de la structure
data myData = {'V', 'S'};  // Valeurs initiales

// Définir les broches des boutons
int buttonStop = 8;       // Bouton Rouge
int buttonDemarre = 7;    // Bouton Vert
int buttonRecule = 2;     // Bouton Blanc
int buttonAvance = 4;     // Bouton Bleu

// Variables pour temporisation
unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000; // Envoi toutes les 1000 ms

void setup() {
  Serial.begin(9600);
  Serial.println("Émetteur NRF24L01 prêt");

  // Initialisation du module NRF24L01
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(3, 5);
  radio.openWritingPipe(slaveAddress);

  // Configurer les boutons comme entrées avec pull-up activé
  pinMode(buttonStop, INPUT_PULLUP);
  pinMode(buttonDemarre, INPUT_PULLUP);
  pinMode(buttonRecule, INPUT_PULLUP);
  pinMode(buttonAvance, INPUT_PULLUP);
}

void loop() {
  // Temporisation pour envoi périodique
  currentMillis = millis();
  
    // Lire l'état des boutons et mettre à jour la structure
    if (digitalRead(buttonStop) == LOW) myData.etat = 'S';        // Bouton Stop
    if (digitalRead(buttonDemarre) == LOW) myData.etat = 'D';     // Bouton Démarrer
    if (digitalRead(buttonRecule) == LOW) myData.direction = 'R'; // Bouton Reculer
    if (digitalRead(buttonAvance) == LOW) myData.direction = 'V'; // Bouton Avancer
  if (currentMillis - prevMillis >= txIntervalMillis) {
    send();
    prevMillis = millis();
  }
}

void send() {
      // Envoyer les données
    if (radio.write(&myData, sizeof(myData))) {
      Serial.print("Données envoyées -> Direction: ");
      Serial.print(myData.direction);
      Serial.print(", État: ");
      Serial.println(myData.etat);
    } else {
      Serial.println("Erreur lors de l'envoi");
    }
}

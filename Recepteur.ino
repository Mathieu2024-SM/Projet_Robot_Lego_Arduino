#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Broches NRF24L01
#define CE_PIN   9
#define CSN_PIN 10

// Pins du moteur (pont en H)
#define borneENA 4
#define borneIN1 7
#define borneIN2 8

// Paramètres pour la vitesse
#define delaiChangementVitesse 20
#define vitesseMinimale 60
#define vitesseMaximale 150

// Adresse de communication
const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

// Création de l'objet radio
RF24 radio(CE_PIN, CSN_PIN);

// Structure pour stocker les données reçues
struct data {
  char direction;  // MARCHE_AVANT = 'V', MARCHE_ARRIERE = 'R'
  char etat;       // DEMARRE = 'D', STOP = 'S'
};

data receivedData; // Instance pour recevoir les données
bool newData = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Récepteur NRF24L01 prêt");

  // Configuration des pins moteur
  pinMode(borneENA, OUTPUT);
  pinMode(borneIN1, OUTPUT);
  pinMode(borneIN2, OUTPUT);
  
  // Initialisation du module NRF24L01
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, thisSlaveAddress);
  radio.startListening();
}

void loop() {
  // Vérifier si des données sont disponibles
  getData();
  showData();
  
  if (receivedData.etat == 'D') { // Démarrer le moteur
      configurerSensDeRotationPontA(receivedData.direction);
      //changeVitesseMoteurPontA(vitesseMaximale);
      marcherAvecDesArrets();

  } else if (receivedData.etat == 'S') { // Arrêter le moteur
      descendreProgressivement();
      arreterMoteur();
  }
}

void getData() {
  if (radio.available()) {
    // Lire les données dans la structure
    radio.read(&receivedData, sizeof(receivedData));
    newData = true;
  }
  else 
  Serial.println("Probleme de connexion");
}

void showData() {
  // Afficher les données reçues
  if (newData) {
    Serial.print("Données reçues -> Direction: ");
    Serial.print(receivedData.direction);
    Serial.print(", État: ");
    Serial.println(receivedData.etat);
    newData = false;
  }
}

void configurerSensDeRotationPontA(char sensDeRotation) {
  if (sensDeRotation == 'V') {  // Marche avant
    digitalWrite(borneIN1, HIGH);
    digitalWrite(borneIN2, LOW);
  } else if (sensDeRotation == 'R') {  // Marche arrière
    digitalWrite(borneIN1, LOW);
    digitalWrite(borneIN2, HIGH);
  }
}

void monterProgressivement() {
  for (int vitesse = vitesseMinimale; vitesse <= vitesseMaximale; vitesse++) {
    changeVitesseMoteurPontA(vitesse);
    delay(delaiChangementVitesse);
  }
}

void descendreProgressivement() {
  for (int vitesse = vitesseMaximale; vitesse >= vitesseMinimale; vitesse--) {
    changeVitesseMoteurPontA(vitesse);
    delay(delaiChangementVitesse);
  }
}

void marcherAvecDesArrets() {
      monterProgressivement();
      delay(500);
      descendreProgressivement();
}

void changeVitesseMoteurPontA(int nouvelleVitesse) {
  analogWrite(borneENA, nouvelleVitesse); // Modifie la vitesse via PWM
}

void arreterMoteur() {
  digitalWrite(borneIN1, LOW);
  digitalWrite(borneIN2, LOW);
  changeVitesseMoteurPontA(0); // Arrête complètement le moteur
}

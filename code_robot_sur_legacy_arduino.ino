//*******************************************************************************//
// Association des entrées du L298N, aux sorties utilisées sur notre Arduino Uno //
//*******************************************************************************//
#define borneENA        4      // On associe la borne "ENA" du L298N à la pin D6 de l'Arduino (PWM possible sur cette broche)
#define borneIN1        7       // On associe la borne "IN1" du L298N à la pin D5 de l'Arduino
#define borneIN2        8       // On associe la borne "IN2" du L298N à la pin D4 de l'Arduino
/*
#define borneIN3        7       // On associe la borne "IN3" du L298N à la pin D7 de l'Arduino
#define borneIN4        6       // On associe la borne "IN4" du L298N à la pin D6 de l'Arduino
#define borneENB        5       // On associe la borne "ENB" du L298N à la pin D5 de l'Arduino (PWM possible sur cette broche)
*/
//*************************//
// Constantes du programme //
//*************************//
#define delaiChangementVitesse     20       // Défini un petit délai, exprimé en millisecondes, avant tout autre changement de vitesse
#define vitesseMinimale            60       // Rapport cylique minimal du signal PWM, pour faire tourner le moteur au minimum de sa vitesse (en pratique, on évitera de trop approcher la valeur  0)
#define vitesseMaximale            150      // Rapport cylique maximal du signal PWM, pour faire tourner le moteur au maximum de sa vitesse

const char MARCHE_AVANT   = 'V';            // Défini une constante pour la "marche avant" (peu importe la valeur)
const char MARCHE_ARRIERE = 'R';            // Défini une constante pour la "marche arrière" (peu importe la valeur)
int buttonStop = 10;     
int buttonDemarre = 9;    
//************************//
// Variables du programme //
//************************//
int vitesse;                    // Spécifie la vitesse de rotation du moteur, entre son minimum (0) et son maximum (255) <= signal PWM

//*******//
// SETUP //
//*******//
void setup() {
  
  // Configuration de toutes les pins de l'Arduino en "sortie" (car elles attaquent les entrées du module L298N)
  pinMode(borneENA, OUTPUT);
  pinMode(borneIN1, OUTPUT);
  pinMode(borneIN2, OUTPUT);
  /*
  pinMode(borneIN3, OUTPUT);
  pinMode(borneIN4, OUTPUT);
  pinMode(borneENB, OUTPUT);
  */
}

//**************************//
// Boucle principale : LOOP //
//**************************//
void loop() {

  // ***************** MARCHE AVANT ******************** //
  /*digitalWrite(borneIN1, HIGH);                 // L'entrée IN1 doit être au niveau haut
  digitalWrite(borneIN2, LOW);   
  delay(1000);
  digitalWrite(borneIN2, HIGH);                 // L'entrée IN1 doit être au niveau haut
  digitalWrite(borneIN1, LOW);   
  */
  // On configure le sens de rotation du moteur branché sur le pont A en "marche avant"
  configurerSensDeRotationPontA(MARCHE_AVANT);

   if (digitalRead(buttonDemarre) == LOW) {  // Démarrer
      for (int vitesse = vitesseMinimale; vitesse <= vitesseMaximale; vitesse++) {
        changeVitesseMoteurPontA(vitesse);
        delay(delaiChangementVitesse);
      }
      // Puis on ramène sa vitesse à 0, au même rythme
      for(vitesse=vitesseMaximale ; vitesse>vitesseMinimale ; vitesse=vitesse-1) {
        changeVitesseMoteurPontA(vitesse);            // Met à jour la vitesse du moteur, en modifiant le rapport cyclique du signal PWM envoyé
        delay(delaiChangementVitesse);                // ... et on rajoute un petit délai pour que tout n'aille pas trop vite !
      }
    } else if (digitalRead(buttonStop) == LOW) {  // Stop
      changeVitesseMoteurPontA(0);
    }
}



//************************************************************************************//
// Fonction : configurerSensDeRotationPontA()                                         //
// But :      Permet de définir le sens de rotation du moteu branché sur le pont A    //
//************************************************************************************//
void configurerSensDeRotationPontA(char sensDeRotation) {

  if(sensDeRotation == MARCHE_AVANT) {
    // Configuration du L298N en "marche avant", pour le moteur connecté au pont A. Selon sa table de vérité, il faut que :
    digitalWrite(borneIN1, HIGH);                 // L'entrée IN1 doit être au niveau haut
    digitalWrite(borneIN2, LOW);                  // L'entrée IN2 doit être au niveau bas    
  }
  
  if(sensDeRotation == MARCHE_ARRIERE) {
    // Configuration du L298N en "marche arrière", pour le moteur câblé sur le pont A. Selon sa table de vérité, il faut que :
    digitalWrite(borneIN1, LOW);                  // L'entrée IN1 doit être au niveau bas
    digitalWrite(borneIN2, HIGH);                 // L'entrée IN2 doit être au niveau haut
  }
}

//************************************************************************************//
// Fonction : changeVitesseMoteurPontA()                                              //
// But :      Change la vitesse de rotation du moteur branché sur le pont A           //
//            (cette valeur doit être comprise entre 0 et 255)                        //
//************************************************************************************//
void changeVitesseMoteurPontA(int nouvelleVitesse) {
  
  // Génère un signal PWM permanent, de rapport cyclique égal à "nouvelleVitesse" (valeur comprise entre 0 et 255)
  analogWrite(borneENA, nouvelleVitesse);
}

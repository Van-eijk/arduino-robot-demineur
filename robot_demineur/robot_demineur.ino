// Inclusion des bibliothèques
#include <Servo.h> // On inclut cette bibliothèque pour piloter le servo moteur

/* ************  Definition des variables *********** */

// cas des moteur
int vitesse = 140 ; // Variable permettant de définir la vitesse des moteurs
int vitesseMoteurGauche = 8;
int vitesseMoteurDroit = 13;
int rotation1MoteurGauche = 9;
int rotation2MoteurGauche = 10;
int rotation1MoteurDroit = 11;
int rotation2MoteurDroit = 12;


// cas du servo moteur
Servo monServo;  // on crée l'objet monServo
int pin = 5;
int positionServo = 0;


// cas du capteur ultrason

const int trigger = 7;
const int echo = 6 ;
long duration ;
int distance;
int resultatDistanceObstacle;







/************** Définitions des fonctions ************* */

// cas du servo moteur

void pivotRadarGauche(int positionInitiale, int positionFinale ){
   for (int position = positionInitiale; position <=positionFinale; position ++){ // on crée une variable position qui prend des valeurs entre 0 à 180 degrés
        monServo.write(position);  // le bras du servomoteur prend la position de la variable position
        delay(15);  // on attend 15 millisecondes
    }
}

void pivotRadarMilieu(){
        monServo.write(90);  // le bras du servomoteur prend la position de la variable position
        
   
}

void pivotRadarDroit(int positionInitiale, int positionFinale){
   for (int position = positionInitiale; position >=positionFinale; position --){ // cette fois la variable position passe de 180 à 0°
        monServo.write(position);  // le bras du servomoteur prend la position de la variable position
        delay(15);  // le bras du servomoteur prend la position de la variable position
    }
}



// cas du capteur à ultrason

int distanceObstacle(){
  digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);

  duration = pulseIn(echo,HIGH);
  distance = duration * 0.034 / 2;
  return distance ;

}


// cas des moteurs

void arret(int tempsArret){
  
  analogWrite(vitesseMoteurGauche,0);
  analogWrite(vitesseMoteurDroit,0);

  digitalWrite(rotation1MoteurGauche,0);
  digitalWrite(rotation2MoteurGauche,0);

  digitalWrite(rotation1MoteurDroit,0);
  digitalWrite(rotation2MoteurDroit,0);
  delay(tempsArret);
  
}

void avancer(){
  arret(10); // On stop d'abord le robot

  analogWrite(vitesseMoteurGauche,vitesse); // On active les moteurs gauche à une vitesse de 255
  analogWrite(vitesseMoteurDroit,vitesse); // On active les moteurs droit à une vitesse de 255
  
  digitalWrite(rotation1MoteurGauche,0);
  digitalWrite(rotation2MoteurGauche,1);

  digitalWrite(rotation1MoteurDroit,0);
  digitalWrite(rotation2MoteurDroit,1);
}

void reculer(){
  arret(10); // On stop d'abord le robot

  analogWrite(vitesseMoteurGauche,vitesse); // On active les moteurs gauche à une vitesse de 255
  analogWrite(vitesseMoteurDroit,vitesse); // On active les moteurs droit à une vitesse de 255
  
  digitalWrite(rotation1MoteurGauche,1);
  digitalWrite(rotation2MoteurGauche,0);

  digitalWrite(rotation1MoteurDroit,1);
  digitalWrite(rotation2MoteurDroit,0);
}

void directionGauche(int t){
  arret(10); // On stop d'abord le robot

  analogWrite(vitesseMoteurGauche,0); // On active les moteurs gauche à une vitesse de 255
  analogWrite(vitesseMoteurDroit,vitesse); // On active les moteurs droit à une vitesse de 255
  
  digitalWrite(rotation1MoteurGauche,0);
  digitalWrite(rotation2MoteurGauche,0);

  digitalWrite(rotation1MoteurDroit,0);
  digitalWrite(rotation2MoteurDroit,1);
  delay(t);
  
}

void directionDroite(int t){
  arret(10); // On stop d'abord le robot

  analogWrite(vitesseMoteurGauche,vitesse); // On active les moteurs gauche à une vitesse de 255
  analogWrite(vitesseMoteurDroit,0); // On active les moteurs droit à une vitesse de 255
  
  digitalWrite(rotation1MoteurGauche,0);
  digitalWrite(rotation2MoteurGauche,1);

  digitalWrite(rotation1MoteurDroit,0);
  digitalWrite(rotation2MoteurDroit,0);
  delay(t);
  
}



void setup() {
  // put your setup code here, to run once:
  
  //Configuration des sorties

  /* les moteurs */
  pinMode(vitesseMoteurGauche,OUTPUT);
  pinMode(vitesseMoteurDroit,OUTPUT);
  pinMode(rotation1MoteurGauche,OUTPUT);
  pinMode(rotation2MoteurGauche,OUTPUT);
  pinMode(rotation1MoteurDroit,OUTPUT);
  pinMode(rotation2MoteurDroit,OUTPUT);

  arret(3000);

  /* le servo moteur */
  monServo.attach(5); // on définit le Pin utilisé par le servomoteur
  monServo.write(90);

  /* capteur ultrason */

  pinMode(trigger, OUTPUT);
  pinMode(echo,INPUT);
  Serial.begin(9600);

  

}

void loop() {
  //arret(2000);

  resultatDistanceObstacle = distanceObstacle(); // detecte obstacle
  if(resultatDistanceObstacle >= 100){
    // s'il n'ya aucun obstacle on avance
    avancer();
    delay(500);
  }
  else{

    // comme il y'a un obstacle devant, on detecte l'obstacle à gauche
    arret(3000);
    pivotRadarGauche(90,180);

    resultatDistanceObstacle = distanceObstacle();

    if(resultatDistanceObstacle >= 100){
      // comme il n'y a pas d'obstacle à devant, on vire à gauche et on  avance
      directionGauche(300);
      pivotRadarMilieu();
      avancer();
      delay(500);
    }
    else{
      // s'il y'a l'obstacle à gauche, on le détecte à droite
      pivotRadarDroit(180,0);
      resultatDistanceObstacle = distanceObstacle();
      if(resultatDistanceObstacle >= 100){
        // s'il n'ya pas d'obstacle à droite, on vire à droite et on avance
        directionDroite(300);
        pivotRadarMilieu();
        avancer();
        delay(500);
      }
      else{
        // s'il y'a l'obstacle à droite, on recule; on vire à droite et on avance
        reculer();
        delay(500);
        arret(400);
        pivotRadarDroit(0,90);
        pivotRadarMilieu();
        directionDroite(300);
        avancer();
        delay(500);
      }
    }
    
  }




  
  //Serial.println(resultatDistanceObstacle); 
  
  
 
    



  
  

}

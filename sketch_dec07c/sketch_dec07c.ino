#include <Servo.h>  // on inclut la bibliothèque pour piloter un servomoteur

Servo monServo;     // on crée l'objet monServo

void setup()
{
    monServo.attach(5);   // on définit le Pin utilisé par le servomoteur
    //pinMode(13,OUTPUT);   // la Pin13 est mise en mode OUTPUT
}

void loop()
{
    monServo.write(90);      // on dit à l'objet de mettre le servo à 0°
                  // appel de la fonction diode13
}

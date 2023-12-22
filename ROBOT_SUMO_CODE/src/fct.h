#include <Arduino.h>

// DEL IFR
#define PIN_DETEC_1 6
#define PIN_BURST_1 4

// MOTEUR
#define PIN_MOT_1A 15
#define PIN_MOT_1B 16
#define PIN_MOT_2A 17
#define PIN_MOT_2B 18

// CAPTEUR LIGNE
#define PIN_LIGNE_IN 3


// Variables contenant les conditions de transitions
bool C1 = false;
bool C2 = false;
bool C4 = false;



enum Etat {
  E1,
  E2,
};


// État de démarrage (toujours le premier en théorie)
enum Etat etatActuel = E1;




void burst()
{
  for(char i=0; i<23; i++)
  {
    ledcWrite(0,100);
    delay(0.000026316);
  }

  Serial.println("BURST_FINIT");

  delay(0.9);
}



void detec_obj()
{
    if((digitalRead(PIN_DETEC_1)==LOW))
      C1 = true;
}


void perdu_obj()
{
    if((digitalRead(PIN_DETEC_1)==HIGH))
      C2 = true;
}








void moteur_avance()
{
  
  digitalWrite(PIN_MOT_1A, HIGH);
  digitalWrite(PIN_MOT_1B, LOW);
  digitalWrite(PIN_MOT_2A, HIGH);
  digitalWrite(PIN_MOT_2B, LOW);
  

}


void moteur_recul()
{
  
  digitalWrite(PIN_MOT_1A, LOW);
  digitalWrite(PIN_MOT_1B, HIGH);
  digitalWrite(PIN_MOT_2A, LOW);
  digitalWrite(PIN_MOT_2B, HIGH);

}


void moteur_tourne()
{
  
  digitalWrite(PIN_MOT_1A, HIGH);
  digitalWrite(PIN_MOT_1B, LOW);
  digitalWrite(PIN_MOT_2A, LOW);
  digitalWrite(PIN_MOT_2B, HIGH);
  
}



void transition()
{
    if(C1)
      etatActuel = E2;
    else if(C2)
      etatActuel = E1;
    else if(C4)
      etatActuel = E1;
}



void etat_E1()
{
    C2 = false;
    C4 = false;
    
    while((digitalRead(PIN_DETEC_1)==HIGH))
    {
        burst();
        moteur_tourne();
    }
      
    C1 = true;
}



void etat_E2()
{
    C4 = false;
    
    while((digitalRead(PIN_DETEC_1)==LOW))
    {
        burst();
        moteur_avance();
    }
      
    C2 = true;
    C1 = false;
}




void etat_now()
{
    switch (etatActuel)
    {
    case E1:
      etat_E1();
      break;
    case E2:
      etat_E2();
      break;
    }
}
#include <Arduino.h>




#define PIN_DETEC_1 4
#define PIN_DETEC_2 5
#define PIN_DETEC_3 13
#define PIN_BURST_1 6
#define PIN_BURST_2 7
#define PIN_BURST_3 14
#define PIN_MOT_1A 15
#define PIN_MOT_1B 16
#define PIN_MOT_2A 17
#define PIN_MOT_2B 18
#define PIN_LIGNE_OUT 8
#define PIN_LIGNE_IN 3



enum Etat {
  E1,
  E2,
  E3
};

// Variables contenant les conditions de transitions
bool C1 = false;
bool C2 = false;
bool C3 = false;
bool C4 = false;




// État de démarrage (toujours le premier en théorie)
enum Etat etatActuel = E1;


void burst();
void detec_obj();
void perdu_obj();
void detec_ligne();
void moteur_avance();
void moteur_recul();
void moteur_tourne();
void transition();
void etat_E1();
void etat_E2();
void etat_E3();
void etat_now();


void setup()
{
  Serial.begin(115200);
  
  // put your setup code here, to run once:
  //pinMode(10,OUTPUT);
  ledcSetup(0, 38000, 8);
  
  // CHANNEL POUR LES MOTEURS
  /*ledcSetup(1, 1000, 8);
  ledcSetup(2, 1000, 8);
  ledcSetup(3, 1000, 8);
  ledcSetup(4, 1000, 8);*/

  ledcAttachPin(PIN_BURST_1, 0);
  ledcAttachPin(PIN_BURST_2, 0);
  ledcAttachPin(PIN_BURST_3, 0);

  pinMode(PIN_DETEC_1, INPUT);
  pinMode(PIN_DETEC_2, INPUT);
  pinMode(PIN_DETEC_3, INPUT);

  // GPIO DES MOTEURS EN SORTIE PWM
  /*ledcAttachPin(PIN_MOT_1A, 1);
  ledcAttachPin(PIN_MOT_1B, 2);
  ledcAttachPin(PIN_MOT_2A, 3);
  ledcAttachPin(PIN_MOT_2B, 4);
*/
  

  
  pinMode(PIN_MOT_1A, OUTPUT);
  pinMode(PIN_MOT_1B, OUTPUT);
  pinMode(PIN_MOT_2A, OUTPUT);
  pinMode(PIN_MOT_2B, OUTPUT);
  
  pinMode(PIN_LIGNE_OUT, OUTPUT);
  pinMode(PIN_LIGNE_IN, INPUT);

  Serial.println("BURST_FINIT");
}

void loop()
{
  
  //while(1)
    //moteur_avance();



  //moteur_avance();
  //detec_ligne();
  transition();
  etat_now();
  //delay(0.1);
}



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
    if((digitalRead(PIN_DETEC_1)==LOW))// && (digitalRead(PIN_DETEC_2)==LOW));
      C1 = true;
}


void perdu_obj()
{
    if((digitalRead(PIN_DETEC_1)==HIGH))// || (digitalRead(PIN_DETEC_2)==HIGH));
      C2 = true;
}



void detec_ligne()
{
    if(digitalRead(PIN_LIGNE_IN)==HIGH)
      C3 = true;
}





void moteur_avance()
{
  
  digitalWrite(PIN_MOT_1A, HIGH);
  digitalWrite(PIN_MOT_1B, LOW);
  digitalWrite(PIN_MOT_2A, HIGH);
  digitalWrite(PIN_MOT_2B, LOW);
  
  
  // MOTEUR A
  /*ledcWrite(1, 95);  // PWM SUR AIN1 (DUTY CYCLE 95%)
  ledcWrite(2, 0);   // PWM SUR AIN2 (DUTY CYCLE 0%)
   
  // MOTEUR B
  ledcWrite(3, 95);  // PWM SUR BIN1 (DUTY CYCLE 95%)
  ledcWrite(4, 0);   // PWM SUR BIN2 (DUTY CYCLE 0%)*/
}


void moteur_recul()
{
  
  digitalWrite(PIN_MOT_1A, LOW);
  digitalWrite(PIN_MOT_1B, HIGH);
  digitalWrite(PIN_MOT_2A, LOW);
  digitalWrite(PIN_MOT_2B, HIGH);
  
  // MOTEUR A
  /*ledcWrite(1, 0);  // PWM SUR AIN1 (DUTY CYCLE 0%)
  ledcWrite(2, 95); // PWM SUR AIN2 (DUTY CYCLE 95%)

  // MOTEUR B
  ledcWrite(3, 0);  // PWM SUR BIN1 (DUTY CYCLE 0%)
  ledcWrite(4, 95); // PWM SUR BIN2 (DUTY CYCLE 95%)*/
}


void moteur_tourne()
{
  
  digitalWrite(PIN_MOT_1A, HIGH);
  digitalWrite(PIN_MOT_1B, LOW);
  digitalWrite(PIN_MOT_2A, LOW);
  digitalWrite(PIN_MOT_2B, HIGH);
  
  // MOTEUR A
  /*ledcWrite(1, 95);  // PWM SUR AIN1 (DUTY CYCLE 95%)
  ledcWrite(2, 0);   // PWM SUR AIN2 (DUTY CYCLE 0%)

  // MOTEUR B
  ledcWrite(3, 0);  // PWM SUR BIN1 (DUTY CYCLE 0%)
  ledcWrite(4, 95); // PWM SUR BIN2 (DUTY CYCLE 95%)*/
}



void transition()
{
    if(C1)
      etatActuel = E2;
    else if(C2)
      etatActuel = E1;
    else if(C3)
      etatActuel = E3;
    else if(C4)
      etatActuel = E1;
}



void etat_E1()
{
    C2 = false;
    C3 = false;
    C4 = false;
    
    while((digitalRead(PIN_DETEC_1)==HIGH))// && (digitalRead(PIN_DETEC_2)==HIGH))
    {
        burst();
        moteur_tourne();
    }
      
    C1 = true;
}



void etat_E2()
{
    C3 = false;
    C4 = false;
    
    while((digitalRead(PIN_DETEC_1)==LOW))// && (digitalRead(PIN_DETEC_2)==LOW))
    {
        burst();
        moteur_avance();
    }
      
    C2 = true;
    C1 = false;
}



void etat_E3()
{
    C1 = false;
    C2 = false;

    
    while(digitalRead(PIN_LIGNE_IN)==HIGH)
      moteur_tourne();

    C3 = false;
    C4 = true;
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
    case E3:
      etat_E3();
      break;
    }
}
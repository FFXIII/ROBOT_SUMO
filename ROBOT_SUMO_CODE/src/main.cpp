#include <Arduino.h>
#include "fct.h"


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


// INTERRUPT CAPTEUR LIGNE
void IRAM_ATTR isr()
{
  while(digitalRead(PIN_LIGNE_IN)==LOW)
      moteur_recul();
}




void setup()
{
  Serial.begin(115200);
  
  ledcSetup(0, 38000, 8);

  ledcAttachPin(PIN_BURST_1, 0);


  pinMode(PIN_DETEC_1, INPUT);

  pinMode(PIN_MOT_1A, OUTPUT);
  pinMode(PIN_MOT_1B, OUTPUT);
  pinMode(PIN_MOT_2A, OUTPUT);
  pinMode(PIN_MOT_2B, OUTPUT);
  

  pinMode(PIN_LIGNE_IN, INPUT);
  attachInterrupt(PIN_LIGNE_IN, isr, FALLING);
  
  Serial.println("BURST_FINIT");
}

void loop()
{
  transition();
  etat_now();
  delay(1);
}
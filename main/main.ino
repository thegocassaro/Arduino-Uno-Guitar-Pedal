#include "efeitos.h"

#define LED 13
#define TOGGLE 2
#define POTENTIOMETER A5

int selectedEffect = CLEAN;
int oldEffect = 0;
int flag = 0;

void setup() {

  pinMode(TOGGLE, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(POTENTIOMETER, INPUT);

  setupEffects();  // Inicialização de todos os efeitos

  Serial.begin(9600);
}

void loop() {

  if (!digitalRead(TOGGLE)) {
    digitalWrite(LED, HIGH);
    if(flag){
      selectedEffect = oldEffect;
      flag = 0;
    }
  }

  else {
    digitalWrite(LED, LOW);
    if(!flag){
      oldEffect = selectedEffect;
      selectedEffect = CLEAN;
      flag = 1;
    }
  }

  switchEffect(&selectedEffect);
}

ISR(TIMER1_CAPT_vect) {
  
    effects[selectedEffect].processEffect(); // Chama a função de processamento do efeito atual
}

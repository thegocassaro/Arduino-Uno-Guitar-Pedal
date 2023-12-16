#include "efeitos.h"
#include "clean.h"
#include "chorus.h"
#include "tremolo.h"
#include "UpDown.h"
#include "distortion.h"

Effect effects[NUM_EFFECTS] = {
  {processClean},
  {processChorus},
  {processDistortion},
  {processTremolo},
  {processUpDown}
};

void setupEffects() {
  
  // setup ADC
  ADMUX = 0x60; // left adjust, adc0, internal vcc
  ADCSRA = 0xe5; // turn on adc, ck/32, auto trigger
  ADCSRB = 0x07; // t1 capture for trigger
  DIDR0 = 0x01; // turn off digital inputs for adc0

  // setup PWM
  TCCR1A = (((PWM_QTY - 1) << 5) | 0x80 | (PWM_MODE << 1)); //
  TCCR1B = ((PWM_MODE << 3) | 0x11); // ck/1
  TIMSK1 = 0x20; // interrupt on capture interrupt
  ICR1H = (PWM_FREQ >> 8);
  ICR1L = (PWM_FREQ & 0xff);
  DDRB |= ((PWM_QTY << 1) | 0x02); // turn on outputs
  sei(); // turn on interrupts - not really necessary with arduino
}

RotaryEncoder encoder(A2, A3);
int valor = 0;
int newPos = 0;

void switchEffect(int *selectedEffect) {
  static int pos = 0;
  encoder.tick();
  long newPos = encoder.getPosition();

  if (pos != newPos) {
    // Funcoes de desalocacao
    delay(10);
    destroy_chorus();
    delay(10);
    destroy_UpDown();
    delay(10);

    // Seleciona novo efeito na ordem
    *selectedEffect = (newPos + NUM_EFFECTS) % NUM_EFFECTS;

    // Debug
    Serial.println(newPos);
    Serial.println(*selectedEffect);
    Serial.println();

    pos = newPos;
  }
}


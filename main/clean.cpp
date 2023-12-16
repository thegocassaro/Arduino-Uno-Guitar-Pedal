#include "clean.h"

#define LED 13
#define TOGGLE 2
#define POTENTIOMETER A5

#define PWM_FREQ 0x00FF
#define PWM_MODE 0
#define PWM_QTY 2

//other variables
int clean_input, clean_vol_variable=512;
byte clean_ADC_low, clean_ADC_high;

void processClean() {
  // get ADC data
    
  // Serial.println("CLEAN");

    clean_ADC_low = ADCL; // you need to fetch the low byte first
    clean_ADC_high = ADCH;
    //construct the input sumple summing the ADC low and high byte.
    clean_input = ((clean_ADC_high << 8) | clean_ADC_low) + 0x8000; // make a signed 16b value

    
    //write the PWM signal
    OCR1AL = ((clean_input + 0x8000) >> 8); // convert to unsigned, send out high byte
    OCR1BL = clean_input; // send out low byte

}
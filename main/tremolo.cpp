#include "tremolo.h"

//defining the output PWM parameters
#define PWM_FREQ 0x00FF // pwm frequency - 31.3KHz
#define PWM_MODE 0 // Fast (1) or Phase Correct (0)
#define PWM_QTY 2 // 2 PWMs in parallel

const char * const sinewave[] PROGMEM = {
  // this file is stored in StompShield and is a 1024 value
  // sinewave lookup table of signed 16bit integers
  // you can replace it with your own waveform if you like
  
  #include "mult16x16.h"
  #include "sinetable.h"
  
};
unsigned int tremolo_location = 0; // incoming data buffer pointer

unsigned int tremolo_fractional = 0x00; // tremolo_fractional sample position
int tremolo_data_buffer; // temporary data storage to give a 1 sample buffer
int input, speed=20;
int counter=0;


void processTremolo() { // all processing happens here
    // Serial.println("TREMOLO");

  // output the last value calculated
  OCR1AL = ((tremolo_data_buffer + 0x8000) >> 8); // convert to unsigned, send out high byte
  OCR1BL = tremolo_data_buffer; // send out low byte
  
  // get ADC data
  byte temp1 = ADCL; // you need to fetch the low byte first
  byte temp2 = ADCH; // yes it needs to be done this way
  int input = ((temp2 << 8) | temp1) + 0x8000; // make a signed 16b value
    
  
  tremolo_fractional += speed; // increment sinewave lookup counter
  if (tremolo_fractional >= 0x0100) { // if its large enough to go to next sample
    tremolo_fractional &= 0x00ff; // round off
    tremolo_location += 1; // go to next tremolo_location
    tremolo_location &= 0x03ff; // fast boundary wrap for 2^n boundaries
  }
  // fetch current sinewave value
  int amplitude = pgm_read_word_near(sinewave + tremolo_location);
  amplitude += 0x8000; // convert to unsigned
  int output;
  MultiSU16X16toH16(output, input, amplitude);
  // save value for playback next interrupt
  tremolo_data_buffer = output; 
}

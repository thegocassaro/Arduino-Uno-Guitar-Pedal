#include "chorus.h"

/* Based on the "CHORUS for Stompshield" adapted from pedal shield, and reconverted for pedalSHIELD UNO*/
 
//defining harware resources.
#define LED 13
#define FOOTSWITCH 12
#define TOGGLE 2
// #define PUSHBUTTON_1 A5
// #define PUSHBUTTON_2 A4

//defining the output PWM parameters
#define PWM_FREQ 0x00FF // pwm frequency - 31.3KHz
#define PWM_MODE 0 // Fast (1) or Phase Correct (0)
#define PWM_QTY 2 // 2 PWMs in parallel

//defining buttons parameters
#define B_MAX 100 // max value for rotary encoder input (min freq)
#define B_MIN 1 // min value for rotary encoder input (max freq)

//defining FX parameters
#define MAX_DELAY 240
#define MIN_DELAY 200

uint16_t* sDelayBuffer0 = NULL;
// uint16_t* sDelayBuffer1 = NULL;
unsigned int DelayCounter = 5;
unsigned int Delay_Depth = 25;
unsigned int count_up=1;
int p;
int POT0 = 1;

unsigned int chorus_location = 0; // incoming data buffer pointer
byte button; // button checking timer
byte last_state; // last rotary encoder state
byte chorus_counter = 3; // rotary encoder rotation chorus_counter (and start value)
unsigned int chorus_fractional = 0x00; // chorus_fractional sample position
int chorus_data_buffer; // temporary data storage to give a 1 sample buffer
int counter2=0; //buttons timer chorus_counter

void destroy_chorus() {
  if (sDelayBuffer0 != NULL) {
    free(sDelayBuffer0);
    sDelayBuffer0 = NULL;
    Serial.println("Destroy Chorus");
  }
  
  // if (sDelayBuffer1 != NULL) {
  //   free(sDelayBuffer1);
  //   sDelayBuffer1 = NULL;
  //   Serial.println("CABELUDO");
  // }
}

// void checkButton()
// {
// counter2++; //to save resources, the pushbuttons are checked every 2000 times.
// if(counter2==2000)
// { 
// counter2=0;
//   if (!digitalRead(PUSHBUTTON_1)) {
//      if (chorus_counter > B_MIN) chorus_counter-=1; // if not at min, decrement
//       }
//       if (!digitalRead(PUSHBUTTON_2)) {
//         if (chorus_counter < B_MAX) chorus_counter+=1; // if not at max, increment
//       }
//     }
// }

void processChorus(){

  if(sDelayBuffer0 == NULL){
    sDelayBuffer0 = (uint16_t*)malloc(sizeof(uint16_t)*(MAX_DELAY*2));
  }
  // if(sDelayBuffer1 == NULL){
  //   sDelayBuffer1 = (uint16_t*)malloc(sizeof(uint16_t)*(MAX_DELAY*2));
  // }

  // output the last value calculated
  OCR1AL = ((chorus_data_buffer + 0x8000) >> 8); // convert to unsigned, send out high byte
  OCR1BL = chorus_data_buffer; // send out low byte
  
  // get ADC data
  byte temp1 = ADCL; // you need to fetch the low byte first
  byte temp2 = ADCH; // yes it needs to be done this way
  int input = ((temp2 << 8) | temp1) + 0x8000; // make a signed 16b value
  
  // checkButton();
//  Delay_Depth=chorus_counter;
 
  sDelayBuffer0[DelayCounter] = input;
  POT0 = chorus_counter;
  DelayCounter++;
  if(DelayCounter >= Delay_Depth) 
  {
    DelayCounter = 0; 
    if(count_up)
    {
       for(p=0;p<POT0+1;p++) 
       sDelayBuffer0[Delay_Depth+p]=sDelayBuffer0[Delay_Depth-1]; 
       Delay_Depth=Delay_Depth+POT0;
       if (Delay_Depth>=MAX_DELAY)count_up=0;
    }
    else
    {
       Delay_Depth=Delay_Depth-POT0;
       if (Delay_Depth<=MIN_DELAY)count_up=1;
    }
  }
  
  // save value for playback next interrupt
  chorus_data_buffer = sDelayBuffer0[DelayCounter];
}



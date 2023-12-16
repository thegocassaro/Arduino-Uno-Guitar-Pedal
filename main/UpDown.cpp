#include "UpDown.h"


#define LED 13
#define FOOTSWITCH 12
#define TOGGLE 2

//defining the output PWM parameters
#define PWM_FREQ 0x00FF  // pwm frequency - 31.3KHz
#define PWM_MODE 0       // Fast (1) or Phase Correct (0)
#define PWM_QTY 2        // 2 PWMs in parallel


#define SIZE 600  // buffer size, make lower if it clicks
int *updown_buffer = NULL;


unsigned int location = 0;  // current buffer location
unsigned int offset = 0;    // distance to current location
byte dir = 0;               // direction of travel in buffer
int data_buffer = 0x8000;

void destroy_UpDown() {
  if (updown_buffer != NULL) {
    free(updown_buffer);
    updown_buffer = NULL;
    Serial.println("Destroy UpDown");
  }
}

void processUpDown() {
    // Serial.println("UPDOWN");

  if (updown_buffer == NULL) {
    updown_buffer = (int *)malloc(sizeof(int) * SIZE);
    delay(10);
  }

  // output the last value calculated
  OCR1AL = ((data_buffer + 0x8000) >> 8);  // convert to unsigned, send out high byte
  OCR1BL = data_buffer;                    // send out low byte

  // get ADC data
  byte temp1 = ADCL;                            // you need to fetch the low byte first
  byte temp2 = ADCH;                            // yes it needs to be done this way
  int input = ((temp2 << 8) | temp1) + 0x8000;  // make a signed 16b value

  updown_buffer[location] = input;        // store current sample
  location++;                             // go to next location
  if (location >= SIZE) location = 0;     // deal with boundary
  unsigned int temp = location + offset;  // find playback location
  if (temp >= SIZE) temp -= SIZE;         // boundary wrap
  data_buffer = updown_buffer[temp];      // fetch sample
  if (dir) {                              // increment until at buffer boundary
    if (offset >= (SIZE - 4)) {
      dir = 0;
      offset--;
    } else offset++;
  } else {  // decrement till reaching boundary from other side
    if (offset <= 4) {
      dir = 1;
      offset--;
    } else offset -= 2;
  }
}
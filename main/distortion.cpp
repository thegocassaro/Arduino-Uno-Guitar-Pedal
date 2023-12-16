#include "distortion.h"

//defining the output PWM parameters
#define PWM_FREQ 0x00FF // pwm frequency - 31.3KHz
#define PWM_MODE 0 // Fast (1) or Phase Correct (0)
#define PWM_QTY 2 // 2 PWMs in parallel

//other variables
int distortion_input, distortionAmount=18000; //initial value adjusted by try and error.
unsigned int ADC_low, ADC_high;


void processDistortion()
{
  // get ADC data
  ADC_low = ADCL; // you need to fetch the low byte first
  ADC_high = ADCH;
  //construct the distortion_input sumple summing the ADC low and high byte.
  distortion_input = ((ADC_high << 8) | ADC_low) + 0x8000; // make a signed 16b value

  distortion_input = distortion_input + distortionAmount * sin(distortion_input * 0.001);

  //write the PWM signal
  OCR1AL = ((distortion_input + 0x8000) >> 8); // convert to unsigned, send out high byte
  OCR1BL = distortion_input; // send out low byte
}

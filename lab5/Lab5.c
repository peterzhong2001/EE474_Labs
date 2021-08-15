// Peter Zhong
// 1936889
// 08/15/2021
// This is the file containing the main function for Lab 5. This is a resistance visualizer
// that gradually changes the brightness of 3 LEDs with PWMs to indicate the current resistance
// of the potentiometer. Green LED indicates low resistance, Yellow LED indicates medium resistance,
// and red LED indicates high resistance.

#include <stdint.h>
#include <math.h>
#include "Lab5_Inits.h"
#include "Lab5.h"

uint32_t ADC_value;

int main(void) {
  // Select system clock frequency preset
  enum frequency freq = PRESET2; // 60 MHz
  PLL_Init(freq);        // Set system clock frequency to 60 MHz
  ADCReadPot_Init();     // Initialize ADC0 to read from the potentiometer
  TimerADCTriger_Init(); // Initialize Timer0A to trigger ADC0
  PWM_Init();            // Initialize PWM0
  float resistance;
  while(1) {
    resistance = (double) ADC_value / 4095.0 * 10.0;
    if (resistance < 3.33) { // green linear increase, yellow and red off
      PWM0CMPB = (unsigned long) floor(599 - (599 / 3.33 * resistance) - 1 + 0.5);
      PWM1CMPA = 0x256;
      PWM1CMPB = 0x256;
    } else if (resistance < 6.67) { // green linear decrease, yellow linear increase, red off
      PWM0CMPB = (unsigned long) floor(599 / 3.33 * (resistance - 3.33) - 1 + 0.5);
      PWM1CMPA = (unsigned long) floor(599 - (599 / 3.33 * (resistance - 3.33)) - 1 + 0.5);
      PWM1CMPB = 0x256;
    } else { // green off, yellow linear decrease, red linear increase
      PWM0CMPB = 0x256;
      PWM1CMPA = (unsigned long) floor(599 / 3.33 * (resistance - 6.67) - 1 + 0.5);
      PWM1CMPB = (unsigned long) floor(599 - (599 / 3.33 * (resistance - 6.67)) - 1 + 0.5);
    }
  }
  return 0;
}

void ADC0SS3_Handler(void) {
   ADCISC_0 |= SS3_INTERRUPT_CLEAR; // clear the interrupt flag
   ADC_value = ADCSSFIFO3_0 & 0xFFF; // save adc value to global variable
}

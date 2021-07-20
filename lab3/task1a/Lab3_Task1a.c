// Peter Zhong
// 1936889
// 07/20/2021
// This is the file containing the main function for Lab 3 task 1 part a.
// It defines the LED patterns under different circumstances, and defines the
// ISR for ADC SS3.

#include <stdint.h>
#include "Lab3_Inits.h"
#include "Lab3_Task1a.h"

uint32_t ADC_value;

int main(void) {
  // Select system clock frequency preset
  enum frequency freq = PRESET2; // 60 MHz
  PLL_Init(freq);        // Set system clock frequency to 60 MHz
  LED_Init();            // Initialize the 4 onboard LEDs (GPIO)
  ADCReadPot_Init();     // Initialize ADC0 to read from the potentiometer
  TimerADCTriger_Init(); // Initialize Timer0A to trigger ADC0
  float resistance;
  while(1) {
    resistance = (double) ADC_value / 4095.0 * 10.0;
    if (resistance < 2.5) {
      GPIODATA_N = LED1MASK; // turn on only LED1
      GPIODATA_F = 0x0; // turn off everything else
    } else if (resistance < 5.0) {
      GPIODATA_N = LED1MASK | LED2MASK; // turn on LED1 and LED2
      GPIODATA_F = 0x0; // turn off everything else
    } else if (resistance < 7.5) {
      GPIODATA_N = LED1MASK | LED2MASK; // turn on LED1 and LED2
      GPIODATA_F = LED3MASK; // turn on only LED3
    } else {
      GPIODATA_N = LED1MASK | LED2MASK; // turn on LED1 and LED2
      GPIODATA_F = LED3MASK | LED4MASK; // turn on LED3 and LED4
    }
  }
  return 0;
}

void ADC0SS3_Handler(void) {
   ADCISC_0 |= SS3_INTERRUPT_CLEAR; // clear the interrupt flag
   ADC_value = ADCSSFIFO3_0 & 0xFFF; // save adc value to global variable
}

// Peter Zhong
// 1936889
// 07/20/2021
// This is the file containing the main function for Lab 3 task 1 part b.
// It defines the formula used to calculate board temperature. It also defines the
// ISR for both ADC SS3 and Port J (on-board switches).

#include <stdio.h>
#include <stdint.h>
#include "Lab3_Inits.h"
#include "Lab3_Task1b.h"

uint32_t ADC_value;

int main(void) {
  // Select system clock frequency preset
  enum frequency freq = PRESET2; // 60 MHz
  PLL_Init(freq);        // Set system clock frequency to 60 MHz
  SW_Init();             // Initialize the 2 onboard SWs (GPIO)
  ADCReadPot_Init();     // Initialize ADC0 to read from temperature sensor
  TimerADCTriger_Init(); // Initialize Timer0A to trigger ADC0
  float temperature;
  while (1) {
    temperature = (float) 147.5 - ((247.5 * ADC_value) / 4096);
    printf("%f\n", temperature);
  }
  return 0;
}

void ADC0SS3_Handler(void) {
   ADCISC_0 |= SS3_INTERRUPT_CLEAR; // clear the interrupt flag
   ADC_value = ADCSSFIFO3_0 & 0xFFF; // save adc value to global variable
}

void PortJ_Handler(void) {
   GPIOICR_J |= 0x3; // Clear Port J interruption
  enum frequency freq = PRESET2;
  if (GPIODATA_J == SW2MASK) {
    freq = PRESET3; // 12 MHz
  } else if (GPIODATA_J == SW1MASK) {
    freq = PRESET1; // 120 MHz
  }
  PLL_Init(freq);
}
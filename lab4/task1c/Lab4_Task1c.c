// Peter Zhong
// 1936889
// 07/28/2021
// This is the file containing the main function for Lab 4 task 1 part b.
// It updates the current temperature and clock speed of the TIVA microcontroller
// to the LCD display whenever the ADC updates its temperature reading.
// Some of the initialization files and the ISRs are copied from my lab 3 files.

#include "Lab3_Inits.h"
#include "Lab3_Task1b.h"
#include "SSD2119_Display.h"
#include <stdint.h>

uint32_t ADC_value;
enum frequency freq;
int updateTemp = 0;

int main(void) {
  // Select system clock frequency preset
  freq = PRESET2; // 60 MHz
  PLL_Init(freq);        // Set system clock frequency to 60 MHz
  SW_Init();             // Initialize the 2 onboard SWs (GPIO)
  ADCReadPot_Init();     // Initialize ADC0 to read from temperature sensor
  TimerADCTriger_Init(); // Initialize Timer0A to trigger ADC0
  LCD_Init();            // Initialize the LCD display
  float temperature_c;
  float temperature_f;
  int currentFreq;
  while (1) {
    // clear flag
    updateTemp = 0;
    // convert ADC values into floating point values
    temperature_c = (float) 147.5 - ((247.5 * ADC_value) / 4096);
    temperature_f = temperature_c * 9 / 5 + 32;
    // determing frequency
    if (freq == PRESET1) {
      currentFreq = 120;
    } else if (freq == PRESET2) {
      currentFreq = 60;
    } else if (freq == PRESET3) {
      currentFreq = 12;
    } else { // error
      currentFreq = 0;
    }
    // print on LCD display
    LCD_SetCursor(0, 0);
    LCD_PrintString("The current temperature is ");
    LCD_PrintFloat(temperature_c);
    LCD_PrintString(" C, ");
    LCD_PrintFloat(temperature_f);
    LCD_PrintString(" F.\r\n");
    LCD_PrintString("The current clock frequency is ");
    LCD_PrintInteger(currentFreq);
    LCD_PrintString(" MHz.\r\n");
    // wait for flag
    while (updateTemp != 1);
  }
  return 0;
}

void ADC0SS3_Handler(void) {
   ADCISC_0 |= SS3_INTERRUPT_CLEAR; // clear the interrupt flag
   ADC_value = ADCSSFIFO3_0 & 0xFFF; // save adc value to global variable
   updateTemp = 1; // raise flag
}

void PortJ_Handler(void) {
  GPIOICR_J |= 0x3; // Clear Port J interruption
  freq = PRESET2;
  if (GPIODATA_J == SW1MASK) {
    freq = PRESET3; // 12 MHz
  } else if (GPIODATA_J == SW2MASK) {
    freq = PRESET1; // 120 MHz
  }
  PLL_Init(freq);
}
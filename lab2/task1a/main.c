// Peter Zhong
// 1936889
// 07/06/2021
// This is the main program for Lab 2 task 1 part a. Utilizing the timer module
// on the board, This program cycles the 4 LED lights on the TIVA microcontroller
// with an interval of 1s.

#include <stdint.h>
#include "task1a.h"

// defining masks to access each LED
#define LED1MASK 0x02
#define LED2MASK 0x01
#define LED3MASK 0x10
#define LED4MASK 0x01

int main (void) {
  volatile unsigned short delay = 0 ;
  RCGCGPIO |= RCGCGPIO_F_EN; // Enable PortF GPIO
  RCGCGPIO |= RCGCGPIO_N_EN; // Enable PortN GPIO
  delay++; // Delay 2 more cycles before access Timer registers
  delay++;
  
  GPIOInit();
  TimerInit();
  
  while (1) {
    while (!(GPTMRIS_0 & 0x1)); // interval
    GPTMICR_0 = 0x01; // clear the TATORIS bit;
    GPTMICR_0 = 0x00;
    GPIODATA_N = 0x0; // Turn off port N
    GPIODATA_F = LED4MASK; // Turn on LED4
    
    while (!(GPTMRIS_0 & 0x1)); // interval
    GPIODATA_F = LED3MASK; // Turn on LED3
    GPTMICR_0 = 0x01; // clear the TATORIS bit;
    GPTMICR_0 = 0x00;
    
    while (!(GPTMRIS_0 & 0x1)); // interval
    GPIODATA_F = 0x0; // Turn off port F
    GPIODATA_N = LED2MASK; // Turn on LED2
    GPTMICR_0 = 0x01; // clear the TATORIS bit;
    GPTMICR_0 = 0x00;
    
    while (!(GPTMRIS_0 & 0x1)); // interval
    GPIODATA_N = LED1MASK; // Turn on LED1
    GPTMICR_0 = 0x01; // clear the TATORIS bit;
    GPTMICR_0 = 0x00;
  }
  return 0;
}

void GPIOInit() {
  GPIODIR_F = (LED4MASK | LED3MASK); // Set PF0 and PF4 to output
  GPIODIR_N = (LED2MASK | LED1MASK); // Set PN0 and PN1 to output
  
  GPIODEN_F = (LED4MASK | LED3MASK); // Set PF0 and PF4 to digital
  GPIODEN_N = (LED2MASK | LED1MASK); // Set PN0 and PN1 to digital
}

void TimerInit() {
  RCGCTIMER |= RCGCTIMER_0_EN; // Enable timer 0
  GPTMCTL_0 = 0x0; // Disable timer 0
  GPTMCFG_0 = CFG_32BIT; // Set 32-bit mode
  GPTMTAMR_0 |= (TAMR_PERIODIC | TAMR_COUNT_DOWN); // Set timer to periodic and countdown
  GPTMTAILR_0 = FREQ_1HZ; // Set frequency to 1Hz
  GPTMCTL_0 = 0x1; // Enable timer 0;
}
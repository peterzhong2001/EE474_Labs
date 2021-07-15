// Peter Zhong
// 1936889
// 07/13/2021
// This is the main program for Lab 2 task 2 part b. This program blinks LED1 at
// a rate of 1Hz. If SW1 is pressed, LED1 stays at its current state and LED2 lights
// up; if SW2 is pressed, LED1 continues cycling and LED2 is turned off.

#include <stdint.h>
#include "task2b.h"

// defining masks to access LEDs and SWs
#define LED1MASK 0x02
#define LED2MASK 0x01
#define SW1MASK 0x02
#define SW2MASK 0x01

int main (void) {
  volatile unsigned short delay = 0 ;
  RCGCGPIO |= RCGCGPIO_J_EN; // Enable PortF GPIO
  RCGCGPIO |= RCGCGPIO_N_EN; // Enable PortN GPIO
  delay++; // Delay 2 more cycles before access Timer registers
  delay++;
  
  GPIOInit();
  TimerInit();
  
  while (1) {
    CycleLight();
  }
  return 0;
}

void CycleLight() {
  GPTMICR_0 = 0x01; // clear the TATORIS bit;
  GPTMICR_0 = 0x00;
  GPIODATA_N |= LED1MASK; // Turn on LED1
  while (!(GPTMRIS_0 & 0x1)); // interval
  GPTMICR_0 = 0x01; // clear the TATORIS bit;
  GPTMICR_0 = 0x00;
  GPIODATA_N &= !LED1MASK; // Turn off LED1
  while (!(GPTMRIS_0 & 0x1)); // interval
}

void GPIOInit() {
  GPIODIR_J = 0x0; // Set PJ0 and PJ1 to input
  GPIODIR_N = (LED2MASK | LED1MASK); // Set PN0 and PN1 to output
  GPIODEN_J = (SW2MASK | SW1MASK); // Set PJ0 and PJ1 to digital
  GPIODEN_N = (LED2MASK | LED1MASK); // Set PN0 and PN1 to digital
  GPIOPUR_J = (SW2MASK | SW1MASK); // Set up pullup resistors
  
  EN1 = PORTJ_INTERRUPT_EN; // Enable Port J Interruption
  GPIOIM_J = 0x0; // Mask all interrupts
  GPIOIS_J &= ~0x3; // Make bits 1 and 0 edge sensitive
  GPIOIBE_J &= ~0x3; // Deactivate both edge detection
  GPIOIEV_J &= ~0x3; // Falling edge
  GPIOICR_J |= 0x3; // Clear Port J interruption
  GPIOIM_J |= 0x3; // Unmask bits 1 and 0
}

void TimerInit() {
  RCGCTIMER |= RCGCTIMER_0_EN; // Enable timer 0
  GPTMCTL_0 = 0x0; // Disable timer 0
  GPTMCFG_0 = CFG_32BIT; // Set 32-bit mode
  GPTMTAMR_0 |= (TAMR_PERIODIC | TAMR_COUNT_DOWN); // Set timer to periodic and countdown
  GPTMTAILR_0 = FREQ_1HZ; // Set frequency to 1Hz
  GPTMCTL_0 = 0x1; // Enable timer 0;
}

void PortJ_Handler() {
  if (GPIODATA_J == SW2MASK) {
    GPTMCTL_0 = 0x1; // Enable timer 0;
    GPIODATA_N &= !LED2MASK; // Turn off LED2
  } else if (GPIODATA_J == SW1MASK) {
    GPTMCTL_0 = 0x0; // Disable timer 0;
    GPIODATA_N |= LED2MASK; // Turn on LED2
  }
  GPIOICR_J |= 0x3; // Clear Port J interruption
}
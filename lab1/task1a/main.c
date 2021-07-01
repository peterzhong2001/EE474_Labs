// Peter Zhong
// 1936889
// 06/30/2021
// This is the main program for Lab 1 task 1 part a. This program cycles the
// 4 LED lights on the TIVA microcontroller with an interval of ~0.3s.

#include <stdint.h>
#include "task1a.h"

// defining masks to access each LED
#define LED1MASK 0x02
#define LED2MASK 0x01
#define LED3MASK 0x10
#define LED4MASK 0x01

// Helper function that adds a ~0.3s delay interval to the program
void interval();

int main (void) {
  volatile unsigned short delay = 0 ;
  RCGCGPIO |= RCGCGPIO_F_EN; // Enable PortF GPIO
  RCGCGPIO |= RCGCGPIO_N_EN; // Enable PortN GPIO
  delay++; // Delay 2 more cycles before access Timer registers
  delay++;
  
  GPIODIR_F = (LED4MASK | LED3MASK); // Set PF0 and PF4 to output
  GPIODIR_N = (LED2MASK | LED1MASK); // Set PN0 and PN1 to output
  
  GPIODEN_F = (LED4MASK | LED3MASK); // Set PF0 and PF4 to digital
  GPIODEN_N = (LED2MASK | LED1MASK); // Set PN0 and PN1 to digital
  
  while (1) {
    GPIODATA_N = 0x0; // Turn off port N
    GPIODATA_F = LED4MASK; // Turn on LED4
    interval();
    GPIODATA_F = LED3MASK; // Turn on LED3
    interval();
    GPIODATA_F = 0x0; // Turn off port F
    GPIODATA_N = LED2MASK; // Turn on LED2
    interval();
    GPIODATA_N = LED1MASK; // Turn on LED1
    interval();
  }
  return 0;
}

void interval() {
  for (int j = 0; j < 1000000; j++) {}
}
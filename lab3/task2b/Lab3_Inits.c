// Peter Zhong
// 1936889
// 07/27/2021
// This is the file containing all initialization functions for Lab 3 task 2 part b.
// I deleted everything from "task2a.h" except for PLL and UART initialization.

#include "PLL_Header.h"
#include "Lab3_Inits.h"
#include "Lab3_Task2b.h"

int PLL_Init(enum frequency freq) {
    // Do NOT modify this function.
    MOSCCTL &= ~(0x4);                      // Power up MOSC
    MOSCCTL &= ~(0x8);                      // Enable MOSC
    while ((RIS & 0x100) == 0) {};          // Wait for MOSC to be ready
    RSCLKCFG |= (0x3 << 20);                // Select MOSC as system clock source
    RSCLKCFG |= (0x3 << 24);                // Select MOSC as PLL clock source

    PLLFREQ0 |= 0x60;                       // Set MINT field
    PLLFREQ1 |= 0x4;                        // Set N field

    MEMTIM0 &= ~((0xF << 22) | (0xF << 6));     // Reset FBCHT and EBCHT
    MEMTIM0 &= ~((0xF << 16) | (0xF << 0));     // Reset EWS and FWS
    MEMTIM0 &= ~((0x1 << 21) | (0x1 << 5));     // Reset FBCE and EBCE

    RSCLKCFG &= ~(0x1 << 28);                   // Temporarilly bypass PLL

    switch (freq) {
        case 120:
            MEMTIM0 |= (0x6 << 22) | (0x6 << 6);    // Set FBCHT and EBCHT
            MEMTIM0 |= (0x5 << 16) | (0x5 << 0);    // Set EWS and FWS
            RSCLKCFG |= 0x3;                        // Set PSYSDIV to use 120 MHZ clock
            RSCLKCFG &= ~0x3FC;                     // Update PSYSDIV field
            break;
        case 60:
            MEMTIM0 |= (0x3 << 22) | (0x3 << 6);    // Set FBCHT and EBCHT
            MEMTIM0 |= (0x2 << 16) | (0x2 << 0);    // Set EWS and FWS
            RSCLKCFG |= 0x7;                        // Set PSYSDIV to use 60 MHZ clock
            RSCLKCFG &= ~0x3F8;                     // Update PSYSDIV field
            break;
        case 12:
            MEMTIM0 |= (0x1 << 21) | (0x1 << 5);    // Set FBCE and EBCE
            RSCLKCFG |= 0x27;                       // Set PSYSDIV to use 12 MHZ clock
            RSCLKCFG &= ~0x3D8;                     // Update PSYSDIV field
            break;
        default:
            return -1;
    }

    RSCLKCFG |= (0x1 << 30);                // Enable new PLL settings
    PLLFREQ0 |= (0x1 << 23);                // Power up PLL
    while ((PLLSTAT & 0x1) == 0) {};        // Wait for PLL to lock and stabilize

    RSCLKCFG |= (0x1u << 31) | (0x1 << 28);  // Use PLL and update Memory Timing Register
    return 1;
}

void UART_Init(void) {
  volatile unsigned short delay = 0;
  RCGCUART |= RCGCUART_3_EN; // enable clock to UART0
  RCGCGPIO |= RCGCGPIO_A_EN; // enable clock to GPIO port A
  delay++;
  delay++;
  delay++; // delay for three clock cycles before further configuration
  
  GPIOAFSEL_A |= 0x30; // enable alternate function for PA4 and PA5
  GPIODEN_A |= 0x30; // enable digital function for PA4 and PA5
  GPIODR2R_A |= 0x30; // setting PA4 and PA5 to 2mA drive
  GPIOPCTL_A |= 0x110000; // enable U3Tx and U3Rx function for PA4 and PA5
  
  UARTCTL_3 &= ~0x1; // disable UART3
  // Baud rate is 9600, SysClk is 16M
  UARTIBRD_3 = 0x68; // decimal 104 in hex for Baud rate
  UARTFBRD_3 = 0xB; // decimal 11 in hex for Baud rate
  UARTLCRH_3 = UARTLCRH_8BIT; // set word length to 8 bit
  UARTCC_3 |= 0x5; // set clock source to alternate clock
  UARTCTL_3 |= 0x1; // enable UART3, transmission, and receive
}
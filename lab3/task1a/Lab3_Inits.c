// Peter Zhong
// 1936889
// 07/20/2021
// This is the file containing all initialization functions for Lab 3 task 1 part a.
// The PLL initialization is provided, and I followed the instructions throughout
// the starter code to complete this file.

#include "PLL_Header.h"
#include "Lab3_Inits.h"
#include "Lab3_Task1a.h"

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

void LED_Init(void) {
  volatile unsigned short delay = 0 ;
  RCGCGPIO |= RCGCGPIO_F_EN; // Enable PortF GPIO
  RCGCGPIO |= RCGCGPIO_N_EN; // Enable PortN GPIO
  delay++; // Delay 2 more cycles before access Timer registers
  delay++;
  GPIODIR_F = (LED4MASK | LED3MASK); // Set PF0 and PF4 to output
  GPIODIR_N = (LED2MASK | LED1MASK); // Set PN0 and PN1 to output
  GPIODEN_F = (LED4MASK | LED3MASK); // Set PF0 and PF4 to digital
  GPIODEN_N = (LED2MASK | LED1MASK); // Set PN0 and PN1 to digital
}

void ADCReadPot_Init(void) {
  volatile unsigned short delay = 0;
  RCGCADC |= RCGCADC_0_EN; // Enable the ADC0 clock
  delay++;
  delay++;
  delay++; // 3 clock cycles of delay before further configuration
  
  PLLFREQ0 |= 0x00800000; // Power up the PLL
  while (PLLSTAT != 0x1); // Wait for PLL to lock
  
  ADCCC_0 = ADCCC_ALTCLKCFG; // Configure ADCCC to use ALTCLKCFG
  
  RCGCGPIO |= RCGCGPIO_E_EN; // Enable GPIO Port E
  delay++;
  delay++; // 2 clock cycles of delay before further configuration
  
  GPIODIR_E &= ~PE0; // Set PE0 to input port
  GPIOAFSEL_E |= PE0; // Set PE0 to be controlled by alternate peripheral
  GPIODEN_E &= ~PE0; // Disable PE0 digital mode
  GPIOAMSEL_E |= PE0; // Enable PE0 analog input

  ADCACTSS_0 &= ~ASEN3; // Disable SS3
  ADCEMUX_0 = EM3_TIMER; // Select timer as the trigger for SS3
  ADCSSEMUX3 = 0x0; // Select from AIN[15:0]
  ADCSSMUX3 = AIN3; // Select AIN3 (PE0)
  ADCSSCTL3 = SS3_INTERRUPT_EN; // Enable interrupt at ADCSSCTL3 register
  ADCIM_0 = SS3_IM; // Set the SS3 interrupt mask
  EN0 |= ADCSS3_INTERRUPT_EN; // Set the corresponding bit for ADC0 SS3 in NVIC
  ADCACTSS_0 |= ASEN3; // Enable ADC0 SS3
}

void TimerADCTriger_Init(void) {
  RCGCTIMER |= RCGCTIMER_0_EN; // Enable timer 0 clock
  GPTMCTL_0 = 0x0; // Disable timer 0
  GPTMCTL_0 |= TAOTE; // enable output trigger
  GPTMCFG_0 = CFG_32BIT; // Set 32-bit mode and enable output trigger
  GPTMTAMR_0 |= (TAMR_PERIODIC | TAMR_COUNT_DOWN); // Set timer to periodic and countdown
  GPTMTAILR_0 = FREQ_1HZ; // Set frequency to 1Hz
  GPTMADCEV_0 |= TATOADCEN; // enable trigger on timeout event
  GPTMCTL_0 |= 0x1; // Enable timer 0
}

// NEXT STEP: Go to Lab3_Task1a.c and finish implementing ADC0SS3_Handler

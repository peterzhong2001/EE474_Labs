// Peter Zhong
// 1936889
// 07/22/2021
// This is the file containing all initialization functions for Lab 3 task 2 part a.
// I copied the file from task 1 part b and added a function to initialize the UART.

#include "PLL_Header.h"
#include "Lab3_Inits.h"
#include "Lab3_Task2a.h"

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

void SW_Init(void) {
  volatile unsigned short delay = 0;
  RCGCGPIO |= RCGCGPIO_J_EN; // Enable GPIO Port J
  delay++;
  delay++; // 2 clock cycles of delay before further configuration
  
  GPIODIR_J = 0x0; // Set PJ0 and PJ1 to input
  GPIODEN_J = (SW2MASK | SW1MASK); // Set PJ0 and PJ1 to digital
  GPIOPUR_J = (SW2MASK | SW1MASK); // Set up pullup resistors
  
  EN1 = PORTJ_INTERRUPT_EN; // Enable Port J Interruption
  GPIOIM_J = 0x0; // Mask all interrupts
  GPIOIS_J &= ~0x3; // Make bits 1 and 0 edge sensitive
  GPIOIBE_J &= ~0x3; // Deactivate both edge detection
  GPIOIEV_J &= ~0x3; // Falling edge
  GPIOICR_J |= 0x3; // Clear Port J interruption
  GPIOIM_J |= 0x3; // Unmask bits 1 and 0
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
  
  ADCACTSS_0 &= ~ASEN3; // Disable SS3
  ADCEMUX_0 = EM3_TIMER; // Select timer as the trigger for SS3
  ADCSSCTL3 = (SS3_INTERRUPT_EN | TEMP_SENSOR_EN); // Enable temp sensor and interrupt at ADCSSCTL3 register
  ADCIM_0 = SS3_IM; // Set the SS3 interrupt mask
  EN0 |= ADCSS3_INTERRUPT_EN; // Set the corresponding bit for ADC0 SS3 in NVIC
  ADCACTSS_0 |= ASEN3; // Enable ADC0 SS3
}

void TimerADCTriger_Init(void) {
  RCGCTIMER |= RCGCTIMER_0_EN; // Enable timer 0 clock
  GPTMCTL_0 = 0x0; // Disable timer 0
  GPTMCTL_0 |= TAOTE; // enable output trigger
  GPTMCFG_0 = CFG_32BIT; // Set 32-bit mode
  GPTMTAMR_0 |= (TAMR_PERIODIC | TAMR_COUNT_DOWN); // Set timer to periodic and countdown
  GPTMTAILR_0 = FREQ_1HZ; // Set frequency to 1Hz
  GPTMADCEV_0 |= TATOADCEN; // enable trigger on timeout event
  GPTMCC_0 = 0x1; // set the clock source to alternate clock
  GPTMCTL_0 |= 0x1; // Enable timer 0
}

void UART_Init(void) {
  volatile unsigned short delay = 0;
  RCGCUART |= RCGCUART_0_EN; // enable clock to UART0
  RCGCGPIO |= RCGCGPIO_A_EN; // enable clock to GPIO port A
  delay++;
  delay++;
  delay++; // delay for three clock cycles before further configuration
  GPIOAFSEL_A |= 0x3; // enable alternate function for PA0 and PA1
  GPIODEN_A |= 0x3; // enable digital function for PA0 and PA1
  GPIODR2R_A |= 0x3; // setting PA0 and PA1 to 2mA drive
  GPIOPCTL_A |= 0x11; // enable U0Tx and U0Rx function for PA0 and PA1
  
  UARTCTL_0 &= ~0x1; // disable UART0
  // Baud rate is 9600, SysClk is 16M
  UARTIBRD_0 = 0x68; // decimal 104 in hex for Baud rate
  UARTFBRD_0 = 0xB; // decimal 11 in hex for Baud rate
  UARTLCRH_0 = UARTLCRH_8BIT; // set word length to 8 bit, keep everything else default
  UARTCC_0 = 0x5; // set clock source to alternate clock
  UARTCTL_0 |= 0x1; // enable UART0
}
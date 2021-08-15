// Peter Zhong
// 1936889
// 08/15/2021
// This is the file containing all initialization functions for Lab 5. 
// The majority of this file is copied from Lab 3 Task 1A. I deleted the orginal
// LED_Init function, and implemented a new PWM_Init function.

#include "PLL_Header.h"
#include "Lab5_Inits.h"
#include "Lab5.h"

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
  GPTMTAILR_0 = FREQ_20HZ; // Set frequency to 20Hz
  GPTMADCEV_0 |= TATOADCEN; // enable trigger on timeout event
  GPTMCTL_0 |= 0x1; // Enable timer 0
}

void PWM_Init(void) {
  volatile unsigned short delay = 0;
  RCGCPWM = 0x1; // Enable PWM0 clock
  RCGCGPIO |= RCGCGPIO_F_EN; // Enable GPIO Port F
  delay++;
  delay++; // Delay for two clock cycles before further configuration
  
  GPIODIR_F |= (PF1 | PF2 | PF3); // Set PF1-3 to output
  GPIOAFSEL_F |= (PF1 | PF2 | PF3); // Set PF1-3 to be controlled by alternate function
  GPIODEN_F |= (PF1 | PF2 | PF3); // Enable PF1-3 digital mode
  GPIOPCTL_F |= 0x6660; // Set PF1-3 to PWM
  PWMCC |= 0x101; // Use clock divider and divide by 4 (15MHz clock)
  
  PWM0CTL = 0x0;
  PWM1CTL = 0x0;
  PWM0GENB |= 0x80C;
  PWM1GENA |= 0x8C;
  PWM1GENB |= 0x80C; // Configure generator to countdown mode
  
  PWM0LOAD = 0x257;
  PWM1LOAD = 0x257; // 600 ticks per period for 25kHz frequency
  
  PWM0CMPB = 0x0;
  PWM1CMPA = 0x0;
  PWM1CMPB = 0x0; // default to 100% duty cycle. Changed during first reading of resistance
  
  PWM0CTL |= 0x1; // start the timers in PWM0
  PWM1CTL |= 0x1; // start the timers in PWM1
  PWMENABLE |= 0xE; // Enable 0B, 1A, and 1B outputs
}

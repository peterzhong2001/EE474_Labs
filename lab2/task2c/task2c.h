// Peter Zhong
// 1936889
// 07/13/2021
// This is the header file for Lab 2 task 2 part c. It includes macros to memory
// addresses for GPIO port E and definitions of helper methods for task 2 part c.
// It also includes macros to enable interrupt for timer 0A.

#ifndef __TASK2C_H__
#define __TASK2C_H__

// macros for accessing GPIO ports
#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))
#define RCGCGPIO_E_EN 0x00000010

#define GPIOAMSEL_E (*((volatile uint32_t *)0x4005C528))
#define GPIOAFSEL_E (*((volatile uint32_t *)0x4005C420))
#define GPIODIR_E (*((volatile uint32_t *)0x4005C400))
#define GPIODEN_E (*((volatile uint32_t *)0x4005C51C))
#define GPIODATA_E (*((volatile uint32_t *)0x4005C3FC))

// macros for accessing Timer 0A and Timer 1A
#define RCGCTIMER (*((volatile uint32_t *)0x400FE604))
#define RCGCTIMER_0_EN 0x01
#define RCGCTIMER_1_EN 0x02

#define GPTMCTL_0 (*((volatile uint32_t *)0x4003000C))
#define GPTMCTL_1 (*((volatile uint32_t *)0x4003100C))

#define GPTMCFG_0 (*((volatile uint32_t *)0x40030000))
#define GPTMCFG_1 (*((volatile uint32_t *)0x40031000))
#define CFG_32BIT 0x0

#define GPTMTAMR_0 (*((volatile uint32_t *)0x40030004))
#define GPTMTAMR_1 (*((volatile uint32_t *)0x40031004))
#define TAMR_ONESHOT 0x0001
#define TAMR_COUNT_DOWN 0x0000

#define GPTMTAILR_0 (*((volatile uint32_t *)0x40030028))
#define GPTMTAILR_1 (*((volatile uint32_t *)0x40031028))
#define FREQ_2S 0x1E84800 // 32M in hex
#define FREQ_5S 0x4C4B400 // 80M in hex

#define GPTMRIS_0 (*((volatile uint32_t *)0x4003001C))
#define GPTMRIS_1 (*((volatile uint32_t *)0x4003101C))

#define GPTMICR_0 (*((volatile uint32_t *)0x40030024))
#define GPTMICR_1 (*((volatile uint32_t *)0x40031024))

#define GPTMIMR_0 (*((volatile uint32_t *)0x40030018))
#define IMR_ENABLE 0x01

#define EN0 (*((volatile uint32_t *)0xE000E100))
#define TIMER0A_INTERRUPT_EN 0x00080000

// Helper function for resetting timer
void ResetTimer0();
void ResetTimer1();

// Helper function for starting timer
void StartTimer0();
void StartTimer1();

// Helper function for clearing flags
void ClearFlagTimer0();
void ClearFlagTimer1();

// Helper functions for initializing Sys Button at PE1
void SysInit();
// Helper functions for initializing Ped Button at PE0
void PedInit();

// Helper function for timer initialization
void TimerInit();

// Takes the switch mask as parameter and return the state of that switch.
// This function only returns 1 when the switch has been pressed for at least
// 2 seconds.
unsigned char GetButton(unsigned char sw);

// Helper functions for red LED at PE2: initialize, on, and off
void RedInit();
void RedOn();
void RedOff();

// Helper functions for yellow LED at PE3: initialize, on, and off
void YellowInit();
void YellowOn();
void YellowOff();

// Helper functions for green LED at PE5: initialize, on, and off
void GreenInit();
void GreenOn();
void GreenOff();

// Overall initialize function
void Init();

// 5-second interval between states. Can only be prematurely ended when either
// the system button is pressed for at least 2 seconds or the pedestrian button
// is pressed for at least 2 seconds in the go state
void Interval();

// ISR for timer 0A
void Timer0A_Handler();

#endif //__TASK2C_H__
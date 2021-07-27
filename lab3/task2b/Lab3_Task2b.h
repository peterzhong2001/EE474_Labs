// Peter Zhong
// 1936889
// 07/27/2021
// This is the custom header file for Lab 3 task 2 part b. This header file defines
// the macros needed to access the GPIO port A and the macros to initialize and access UART3.

#ifndef __LAB3_TASK2B_H__
#define __LAB3_TASK2B_H__

//------------------GPIO Macros----------------------
#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))

#define RCGCGPIO_A_EN 0x00000001

#define GPIOAFSEL_A (*((volatile uint32_t *)0x40058420))
#define GPIODEN_A (*((volatile uint32_t *)0x4005851C))
#define GPIOPCTL_A (*((volatile uint32_t *)0x4005852C))
#define GPIODR2R_A (*((volatile uint32_t *)0x40058500))


//------------------UART Macros----------------------
#define RCGCUART (*((volatile uint32_t *)0x400FE618))
#define RCGCUART_3_EN 0x8

#define UARTCTL_3 (*((volatile uint32_t *)0x4000F030))
#define UARTIBRD_3 (*((volatile uint32_t *)0x4000F024))
#define UARTFBRD_3 (*((volatile uint32_t *)0x4000F028))
#define UARTLCRH_3 (*((volatile uint32_t *)0x4000F02C))
#define UARTLCRH_8BIT 0x60
#define UARTCC_3 (*((volatile uint32_t *)0x4000FFC8))
#define UARTDR_3 (*((volatile uint32_t *)0x4000F000))
#define UARTFR_3 (*((volatile uint32_t *)0x4000F018))

// return the most recently entered character to the sender
void ReturnToSender();

#endif //__LAB3_TASK2B_H__
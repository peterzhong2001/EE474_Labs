// Peter Zhong
// 1936889
// 07/27/2021
// This is the header file for initialization functions in Lab 3 task 2 part b.
// It is provided in the starter code. I removed everything except for the PLL
// and UART initiation.

#ifndef _Lab3_Inits_H_
#define _Lab3_Inits_H_

// Preset frequency for 120 MHZ, 60 MHZ, and 12 MHZ clock
// Pass in the preset to Timer1_Init and PLL_Init to configure
// the system clock to specified frequency
enum frequency {PRESET1 = 120, PRESET2 = 60, PRESET3 = 12};

// Initializes the PLL module and generates a system clock frequency
// that equal to the frequency preset.
// Returns 1 if configured successfully, -1 if you select a non-exist preset.
int PLL_Init(enum frequency freq);

// Initializes UART0
void UART_Init(void);

#endif  // _Lab3_Inits_H_

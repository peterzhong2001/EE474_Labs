// Peter Zhong
// 1936889
// 08/03/2021
// This is the header file for PLL initialization in Lab 4 task 2.
// I copied the file from lab 3 and removed other unnecessary functions.

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

#endif  // _Lab3_Inits_H_

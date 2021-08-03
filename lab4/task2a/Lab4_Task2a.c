// Peter Zhong
// 1936889
// 08/03/2021
// This is the main program for Lab 4 task 2 part a. This program implements the traffic
// light state machine with 1 system on/off button, 1 pedestrian button, and 3 LEDs.
// All buttons are virtually displayed on the LCD screen.
// The buttons only respond if they are held down for at least 2 seconds. Regular interval
// between lights is 5 seconds, but that interval can be prematurely ended by either
// the system button (at all time) or the pedestrian button (when in "go" state).

// Most of the code is copied from lab 2. Some functions are modified for the program
// so that it can be run on a LCD display.

#include "SSD2119_Display.h"
#include "SSD2119_Touch.h"
#include "Lab4_Task2a.h"
#include "Lab3_Inits.h"
#include "PLL_Header.h"

// macros for masks for buttons and LEDs
#define SYS 's' 
#define PED 'p'

enum TL_States {TL_SMStart, TL_off, TL_go, TL_warn, TL_stop} TL_State;

int main() {
   volatile unsigned short delay = 0;
   PLL_Init(PRESET2); // 60M clock
   PLLFREQ0 |= 0x00800000; // Power up the PLL
   while (PLLSTAT != 0x1); // Wait for PLL to lock
   RCGCTIMER |= (RCGCTIMER_0_EN | RCGCTIMER_1_EN); // Enable timer 0 and timer 1
   delay++;
   delay++; // delay for two clock cycles before accessing registers
   LCD_Init();
   Touch_Init();
   DrawInterface();
   ResetTimer0();
   ResetTimer1();
   while (1) {
     // Implementing the state machine
     switch(TL_State) { // next state logic
        case TL_SMStart:
          TL_State = TL_off;
          break;
        
        case TL_off:
          if (GetButton(SYS)) {
            TL_State = TL_stop;
          } else {
            TL_State = TL_off;
          }
          break;
          
        case TL_go:
          if (GetButton(SYS)) {
            TL_State = TL_off;
          } else if (GetButton(PED)) {
            TL_State = TL_warn;
          } else {
            TL_State = TL_stop;
          }
          break;
       
        case TL_warn:
          if (GetButton(SYS)) {
            TL_State = TL_off;
          } else {
            TL_State = TL_stop;
          }
          break;
          
        case TL_stop:
          if (GetButton(SYS)) {
            TL_State = TL_off;
          } else if (GetButton(PED)){
            TL_State = TL_stop;
          } else {
            TL_State = TL_go;
          }
          break;
          
        default:
           TL_State = TL_SMStart;
           break;
     }
     
     switch(TL_State) { // output logic
        case TL_off:
          RedOff();
          YellowOff();
          GreenOff();
          Interval();
          break;
        
        case TL_go:
          RedOff();
          YellowOff();
          GreenOn();
          Interval();
          break;
        
         case TL_warn:
           RedOff();
           YellowOn();
           GreenOff();
           Interval();
           break;
         
         case TL_stop:
           RedOn();
           YellowOff();
           GreenOff();
           Interval();
           break;
         
         default: break;
      }
    }
  return 0;
}

void DrawInterface() {
  LCD_DrawCircle(60, 50, 20, Color4[12]); // red light border
  LCD_DrawCircle(60, 120, 20, Color4[14]); // yellow light border
  LCD_DrawCircle(60, 190, 20, Color4[10]); // green light border
  
  LCD_DrawFilledRect(150, 30, 100, 70, Color4[9]); // blue pedestrian button
  LCD_DrawFilledRect(150, 140, 100, 70, Color4[15]); // white system button
}

unsigned char GetButton(char sw) {
  // reset timer when both buttons are unpressed
  if (!SysPressed() && !PedPressed()) { 
    ResetTimer0();
    ClearFlagTimer0();
  // start timer when button is first pressed
  } else if ((GPTMCTL_0 == 0x0) && !(GPTMRIS_0 & 0x01)) {
    StartTimer0();
  }
  // return when both button is pressed and timer is up
  if (sw == SYS) {
    return (SysPressed() && (GPTMRIS_0 & 0x1));
  } else if (sw == PED) {
    return (PedPressed() && (GPTMRIS_0 & 0x1));
  } else {
    return 0;
  }
}

bool SysPressed() {
  unsigned long X = Touch_ReadX();
  unsigned long Y = Touch_ReadY();
  return ((X >= 1550) && (X <= 1960) && (Y >= 695) && (Y <= 1080));
}

bool PedPressed() {
  unsigned long X = Touch_ReadX();
  unsigned long Y = Touch_ReadY();
  return ((X >= 1550) && (X <= 1960) && (Y >= 1200) && (Y <= 1500));
}

void Interval() {
  ResetTimer1();
  ClearFlagTimer1();
  ClearFlagTimer0();
  StartTimer1();
  while (!(GPTMRIS_1 & 0x1)) { // wait for either timeout or interruption
    if ((GetButton(SYS)) || (GetButton(PED) && (TL_State == TL_go))) {
      ResetTimer1();
      ClearFlagTimer1();
      return;
    }
  }
  ResetTimer1();
  ClearFlagTimer1();
}

void ResetTimer0() {
  GPTMCTL_0 = 0x0; // Disable timer 0
  GPTMCFG_0 = CFG_32BIT; // Set 32-bit mode
  GPTMTAMR_0 |= (TAMR_ONESHOT | TAMR_COUNT_DOWN); // Set timer to oneshot and countdown
  GPTMTAILR_0 = FREQ_2S; // Set interval to 2s
}
void ResetTimer1() {
  GPTMCTL_1 = 0x0; // Disable timer 1
  GPTMCFG_1 = CFG_32BIT; // Set 32-bit mode
  GPTMTAMR_1 |= (TAMR_ONESHOT | TAMR_COUNT_DOWN); // Set timer to oneshot and countdown
  GPTMTAILR_1 = FREQ_5S; // Set interval to 5s
}

void StartTimer0() {
  GPTMCTL_0 = 0x1; // Enable timer 0
}
void StartTimer1() {
  GPTMCTL_1 = 0x1; // Enable timer 1
}

void ClearFlagTimer0() {
  GPTMICR_0 = 0x01; // clear the TATORIS bit;
}
void ClearFlagTimer1() {
  GPTMICR_1 = 0x01; // clear the TATORIS bit;
}

void RedOn() {
  LCD_DrawFilledCircle(60, 50, 20, Color4[12]);
}
void RedOff() {
  LCD_DrawFilledCircle(60, 50, 19, Color4[0]);
}

void YellowOn() {
  LCD_DrawFilledCircle(60, 120, 20, Color4[14]);
}
void YellowOff() {
  LCD_DrawFilledCircle(60, 120, 19, Color4[0]);
}

void GreenOn() {
  LCD_DrawFilledCircle(60, 190, 20, Color4[10]);
}
void GreenOff() {
  LCD_DrawFilledCircle(60, 190, 19, Color4[0]);
}
// Peter Zhong
// 1936889
// 06/30/2021
// This is the main program for Lab 1 task 2. This program implements the traffic
// light state machine with 1 system on/off button, 1 pedestrian button, and 3 LEDs.

#include <stdint.h>
#include "task1b.h"

// macros for masks
#define SYS 0x01
#define PED 0x02
#define RED 0x04
#define YELLOW 0x08
#define GREEN 0x20

enum TL_States {TL_SMStart, TL_off, TL_go, TL_warn, TL_stop} TL_State;

int main() {
   volatile unsigned short delay = 0;
   RCGCGPIO |= RCGCGPIO_E_EN;
   delay++;
   delay++; // delay for two clock cycles before accessing registers
   Init();
   
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

void Init() {
  SysInit();
  PedInit();
  RedInit();
  YellowInit();
  GreenInit();
  TimerInit();
}

void TimerInit() {
  RCGCTIMER |= RCGCTIMER_0_EN; // Enable timer 0
  GPTMCTL_0 = 0x0; // Disable timer 0
  GPTMCFG_0 = CFG_32BIT; // Set 32-bit mode
  GPTMTAMR_0 |= (TAMR_PERIODIC | TAMR_COUNT_DOWN); // Set timer to periodic and countdown
  GPTMTAILR_0 = FREQ_1HZ; // Set frequency to 1Hz
}

void StartTimer() {
  GPTMCTL_0 = 0x1; // Enable timer 0
}

void StopAndClearTimer() {
  GPTMCTL_0 = 0x0; // disable timer 0
  GPTMICR_0 = 0x01; // clear the TATORIS bit;
  GPTMICR_0 = 0x00;
}

void SysInit() {
  GPIOAMSEL_E &= ~SYS; // disable analog function
  GPIOAFSEL_E &= ~SYS; // select regular port function
  GPIODIR_E &= ~SYS;   // set input direction
  GPIODEN_E |= SYS;    // enable digital function
}

void PedInit() {
  GPIOAMSEL_E &= ~PED; // disable analog function
  GPIOAFSEL_E &= ~PED; // select regular port function
  GPIODIR_E &= ~PED;   // set input direction
  GPIODEN_E |= PED;    // enable digital function
}
unsigned char GetButton(unsigned char sw) {
  return ((GPIODATA_E & sw) != 0);
  /*
  // stop timer when button is unpressed
  if ((GPIODATA_E & sw) == 0) { 
    StopAndClearTimer();
    return 0;
  }
  // start timer when button is first pressed
  if (((GPIODATA_E & sw) != 0) && (GPTMCTL_0 == 0x0)) {
    StartTimer();
  }
  // return 1 when at least 2 seconds have passed
  if (((GPTMRIS_0 & 0x1) != 0) && ((GPIODATA_E & sw) != 0)) { 
    StopAndClearTimer();
    return 1;
  }
  return 0;
  */
}

void RedInit() {
  GPIOAMSEL_E &= ~RED; // disable analog function
  GPIOAFSEL_E &= ~RED; // select regular port function
  GPIODIR_E |= RED;   // set output direction
  GPIODEN_E |= RED;    // enable digital function
}
void RedOn() {
  GPIODATA_E |= RED;
}
void RedOff() {
  GPIODATA_E &= ~RED;
}

void YellowInit() {
  GPIOAMSEL_E &= ~YELLOW; // disable analog function
  GPIOAFSEL_E &= ~YELLOW; // select regular port function
  GPIODIR_E |= YELLOW;   // set output direction
  GPIODEN_E |= YELLOW;    // enable digital function
}
void YellowOn() {
  GPIODATA_E |= YELLOW;
}
void YellowOff() {
  GPIODATA_E &= ~YELLOW;
}

void GreenInit() {
  GPIOAMSEL_E &= ~GREEN; // disable analog function
  GPIOAFSEL_E &= ~GREEN; // select regular port function
  GPIODIR_E |= GREEN;   // set output direction
  GPIODEN_E |= GREEN;    // enable digital function
}
void GreenOn() {
  GPIODATA_E |= GREEN;
}
void GreenOff() {
  GPIODATA_E &= ~GREEN;
}

void Interval() {
  for (int i = 0; i <= 80000000; i++) {
    if ((GetButton(SYS)) || (GetButton(PED))) { 
      if (i > 16000000) {
        return; 
      }
    }
  }
  /*
  StartTimer();
  // ends interval when button is pressed in go state
  while (!(GPTMRIS_0 & 0x1)) {
    if ((GetButton(SYS)) || ((GetButton(PED)) && (TL_State == TL_go))) { 
      StopAndClearTimer();
      break; 
    }
  }
  StopAndClearTimer();
*/
}
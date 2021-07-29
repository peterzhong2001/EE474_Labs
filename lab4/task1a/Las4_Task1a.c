// Peter Zhong
// 1936889
// 07/28/2021
// This is the main function for lab 4 task 1 part a. It fills the LCD display
// with the color red (as specified in the Color4 array).

#include "SSD2119_Display.h"

int main() {
  LCD_Init();
  LCD_ColorFill(Color4[9]);
  return 0;
}
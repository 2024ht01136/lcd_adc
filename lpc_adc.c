#include <stdint.h>
#include "lcd.h"
#include <lpc23xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void delay(int time)
{
  int i, j;
  for(i = 0; i < time; i++)
    for(j = 0; j < 1000; j++);
}

int main()
{
  char voltage_str[20] = "";
  uint32_t adc_data = 0;
  float volt = 0;

  // ADC Initialization
  PCONP |= (1<<12);      // ADC power
  PINSEL1 |= (1<<14);    // P0.23 as AD0.0
  PINSEL1 &= ~(1<<15);

  AD0INTEN |= (1<<0);    // Interrupt for AD0.0

  // ADC configuration: Select AD0.0, clock = 4 MHz, 10 bits resolution, operational
  AD0CR &= ~(1<<0);      // Clear channel select bits
  AD0CR |= (1<<9);       // Clock divider for 4 MHz
  AD0CR |= (1<<16);      // BURST mode off (software control)
  AD0CR |= (1<<21);      // ADC operational

  lcd_init();
  lcd_clear();

  /*Display BITS ID on first line */	
  set_cursor(0,0);
  lcd_print((unsigned char *)"BITS 2024HT01136");
	
  while(1)
  {
    set_cursor(0,1);
		
    while(!(AD0STAT & (1<<0))){};  // Wait for conversion to complete
		
    adc_data = (AD0DR0 & 0x0000FFC0);  // Get the result from ADC
    adc_data = adc_data >> 6;
    volt = ((adc_data / 1023.0) * 3.3);  // Convert ADC value to voltage

    // Convert float voltage to string for display
    sprintf(voltage_str, "%.2fV ", volt);
		
    lcd_print((const unsigned char *)voltage_str);
		lcd_print((const unsigned char *)"Date:11/10");
    delay(1);
  }
}

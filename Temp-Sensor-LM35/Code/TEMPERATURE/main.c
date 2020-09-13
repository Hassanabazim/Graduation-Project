/*
 *  								HIGHER TECHNOGICAL INSTITUTE
 *  				TEMPERATURE SENSOR LM35 using AVR Micro controller project
 *      						Author: HassaN AbdElazeam
 * 
 */

#define F_CPU8000000UL 			//telling the controller that's Crystal frequency attached
#include <avr/io.h>   			// enable flow data over Ports Pins
#include <stdlib.h>				// include the C function libraries
#include <util/delay.h>			// include the delay function into the program
#include <string.h>				// include the string function that's used in LCD Display
#include "LCD.h"

					/* Analog to Digital Converter Driver */
void ADC_setup()
{
	ADMUX = (1<<REFS0);										// AREF = AVcc
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	// ADC Enable and prescaler of 128
}

uint16_t ADC_readChannel(uint8_t ch)
{
  /* select the corresponding channel 0~7 */

  ch &= 0b00000111;  			// AND operation with 7
  ADMUX = (ADMUX & 0xF8)|ch; 	// clears the bottom 3 bits before ORing
  ADCSRA |= (1<<ADSC);			// start single convention

  /* wait for conversion to complete , till then, run loop continuously */

  while(ADCSRA & (1<<ADSC));
  return (ADC);
}

#define high_temp   30 
#define low_temp	12

/* Variable declarations */
uint16_t ADC_value;
char LCD_string[10];

int main()
{
	DDRA  = 0x00;			/* PORTA is declared as input */
	DDRB  = 0xFF;			/* PORTB is declared as Output */
	DDRC = 0b00111000;		/* PD0,1,2 of PortD is declared as output */
	PORTC = 0x00;

	ADC_setup();
	LCD_Init();
	LCD_Clear();

	while(1)
	{
		ADC_value = ADC_readChannel(0);
		float temp  = ADC_value* (5.0/1023.0);
		double degree = temp  * 100 ;
		dtostrf(degree,2,0,LCD_string);
		
		LCD_String_xy(1,0,"TEMP=");
		LCD_String_xy(1,6,LCD_string);
		LCD_Char(0xDF);
		LCD_String_xy(1,9,"C");
		
		/* Conditions that's indicate the weather based on the average temperature */
		
		if (degree >= high_temp)
		{
			PORTC = 0b00100000;
			LCD_String_xy(2,0,"HOT WEATHER");
			_delay_ms(20);
		}
		 
		else if (degree < low_temp)
		{
		  PORTC = 0b000001000;
		  LCD_String_xy(2,0,"COLD WEATHER");
		  _delay_ms(20);
		}
		
		else
		 {
			PORTC = 0b00010000;
			LCD_String_xy(2,0,"NICE WEATHER");
			_delay_ms(20);
		 }
	}
	return 0;
}
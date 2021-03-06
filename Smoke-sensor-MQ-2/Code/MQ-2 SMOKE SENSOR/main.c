/*
 *  								HIGHER TECHNOGICAL INSTITUTE
 *  				SMOKE Detection Alert using AVR Micro controller project
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

#define clean   300 
#define danger  700

/* Variable declarations */
uint16_t smoke;
char LCD_string[10];

int main()
{
	DDRA  = 0x00;			/* PORTA is declared as input */
	DDRB  = 0xFF;			/* PORTB is declared as Output */
	DDRC  = 0b00001000;		/* PD0 of PortD is declared as output */
	PORTC = 0x00 ;			/* PORTD is set to low logic mode */

	ADC_setup();
	LCD_Init();
	LCD_Clear();

	while(1)
	{
		smoke = ADC_readChannel(0);
		double percentage = (smoke / 1023.00 ) * 100 ;
		dtostrf(percentage,2,0,LCD_string);
		
		LCD_String_xy(1,0,"SCOPE=");
		LCD_String_xy(1,6,LCD_string);
		LCD_String_xy(1,10,"% PPM");
		_delay_ms(50);
		
		 if ((smoke > clean) && ( smoke < danger))
		 {
				PORTC = 0b000001000;
				LCD_String_xy(2,0,"SMOKE ZONE");
				_delay_ms(100);
		 }
		else if (smoke >= danger)
		 {
				PORTC = 0b000001000;
				LCD_String_xy(2,0,"DANGER ZONE");
				_delay_ms(100);
		 }
		else
		 {
				PORTC = 0x00;
				LCD_String_xy(2,0,"CLEAN ZONE");
				_delay_ms(100);
		 }
	}
	return 0;
}

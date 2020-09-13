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
#include "LCD_20x4_H_file.h"

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
#define clean_air   350
#define safe_place  450

/* Variable declarations */
int temp , smoke, flame ;
float temp_volt ;
double degree , smoke_scope, flame_scope ; 
char temp_string[10] , smoke_string[10], flame_string[10] ;

int main()
{
	DDRA  = 0x00;			/* PORTA is declared as input */
	DDRB  = 0xFF;			/* PORTB is declared as Output */
	DDRC = 0b11111000;		/* PD0,1,2 of PortD is declared as output */
	PORTC = 0x00;

	ADC_setup();
	LCD_Init();
	LCD_Clear();

	while(1)
	{
		temp = ADC_readChannel(0);
		smoke= ADC_readChannel(1);
		flame= ADC_readChannel(2);
		
		smoke_scope = (smoke / 1023.0)*100 ;
		flame_scope = (flame / 1023.0)*100 ; 
		temp_volt  = temp * (5.00/1023.00);
		degree = temp_volt  * 100 ;
		
		dtostrf(degree,2,0,temp_string);
		dtostrf(smoke_scope,2,0,smoke_string);
		dtostrf(flame_scope,2,0,flame_string);
		
		LCD_String_xy(1,0,"ANALOG   SENSORS ");
		LCD_String_xy(2,0,"TEMP= ");
		LCD_String_xy(2,7,temp_string);
		LCD_Char(0xDF);
		LCD_String_xy(2,10,"C");
		LCD_String_xy(3,0,"SMOKE= ");
		LCD_String_xy(3,7,smoke_string);
		LCD_String_xy(3,10,"%");
		LCD_String_xy(4,0,"FLAME= ");
		LCD_String_xy(4,7,flame_string);
		LCD_String_xy(4,10,"%");
		_delay_ms(100) ;
		
		if ((smoke >= clean_air) && (flame <= safe_place) && (degree >= high_temp))
		{
			PORTC = 0b11100000;
			_delay_ms(20);
		}
		else if ((smoke >= clean_air) && (flame > safe_place) && (degree >= high_temp))
		{
			PORTC = 0b01100000;
			_delay_ms(20);
		}
		else if ((smoke < clean_air) && (flame <= safe_place) && (degree >= high_temp))
		{
			PORTC = 0b10100000;
			_delay_ms(20);
		}
		else if ((smoke < clean_air) && (flame > safe_place) && (degree >= high_temp)) 	
		{
			PORTC = 0b00100000;
			_delay_ms(20);
		}
		else if ((smoke >= clean_air) && (flame <= safe_place) && (degree <= low_temp))
		{
			PORTC = 0b11001000;
			_delay_ms(20);
		}
		else if ((smoke >= clean_air) && (flame > safe_place) && (degree <= low_temp))
		{
			PORTC = 0b01001000;
			_delay_ms(20);			
		}
		else if ((smoke < clean_air) && (flame <= safe_place) && (degree <= low_temp))
		{
			PORTC = 0b10001000;
			_delay_ms(20);
		}
		else if ((smoke < clean_air) && (flame > safe_place) && (degree <= low_temp))
		{
			PORTC = 0b00001000;
			_delay_ms(20);
		}
		else if ((smoke >= clean_air) && (flame <= safe_place) && ( low_temp < degree < high_temp))
		{
			PORTC = 0b11010000;
			_delay_ms(20);
		}
		else if ((smoke >= clean_air) && (flame > safe_place) && (low_temp < degree < high_temp))
		{
			PORTC = 0b01010000;
			_delay_ms(20);
		}
		else if ((smoke < clean_air) && (flame <= safe_place) && (low_temp < degree < high_temp))
		{
			PORTC = 0b10010000;
			_delay_ms(20);
		}
		else if((smoke < clean_air) && (flame > safe_place) && (low_temp < degree < high_temp))
		{
			PORTC = 0b00010000;
			_delay_ms(20);
		}
		
		 
	}
return(0);
}

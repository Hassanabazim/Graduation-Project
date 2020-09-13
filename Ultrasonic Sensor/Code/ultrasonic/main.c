/*
 *  								HIGHER TECHNOGICAL INSTITUTE
 *  						Ultrasonic using AVR Micro controller project
 *      							Author: HassaN AbdElazeam
 * 
 */

#define F_CPU8000000UL 			// telling the controller that's Crystal frequency attached
#include <avr/io.h>   			// enable flow data over Ports Pins
#include <stdlib.h>				// include the C function libraries
#include <util/delay.h>			// include the delay function into the program
#include <string.h>				// include the string function that's used in LCD Display
#include "LCD.h"


int main(void)
{
	DDRB = 0;					 // PORTB As Output 
	
   LCD_Init();
   unsigned short a,b,high;
   double distance;
   char object_string[10];
   DIO_vsetPINDir('D',7,1);
   
   while(1)
   {
	   TCCR1A = 0;
	   TIFR = (1<<ICF1);  				/* Clear ICF (Input Capture flag)  */
	   DIO_write('D',7,1);
	   _delay_us(50);
	   DIO_write('D',7,0);
	   
	   TCCR1B = 0xc1;  					/* Rising edge, no prescaler , noise canceler*/
	   while ((TIFR&(1<<ICF1)) == 0);
	   a = ICR1;  						/* Take value of capture register */
	   TIFR = (1<<ICF1);  				/* Clear ICF flag */
	   TCCR1B = 0x81;					/* Falling edge, no prescaler ,noise canceler*/
	   while ((TIFR&(1<<ICF1)) == 0);
	   b = ICR1;  						/* Take value of capture register */
	   TIFR = (1<<ICF1);				/* Clear ICF flag */
	   TCNT1=0;
	   TCCR1B = 0;  					/* Stop the timer */
	   
	   LCD_String_xy(1,0,"ULTRASONIC SEN.");  
	   high=b-a;
	   distance =((high*34600)/(F_CPU*2)) ;
	   dtostrf(distance,2,0,object_string);
	  	_delay_ms(100);
	
	   if (distance<=20)
	   {
			LCD_String_xy(2,0,"TURN RIGHT");
		   _delay_ms(100);
	   }
	   
	   else
	   {
		   LCD_String_xy(2,0,"D  =  ");
		   LCD_String_xy(2,5, object_string);
		   LCD_String_xy(2,7,"Cm.");
		   _delay_ms(100);
	   }
   }
   }

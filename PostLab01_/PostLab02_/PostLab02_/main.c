/*
 * Pantalla4_.c
 *
 * Created: 7/21/2026 4:03:01 PM
 * Author : ayala
 */ 

// PINES
// RS	| 0 comando		| 1 dato 
// R/W	| 0 escritura	| 1 lectura

// COMANDOS BASICOS
// DL	| 0 - 4 bits	| 1 - 8 bits
// N	| 0 una linea	| 1 dos lineas
// F	| 0 - 5x8 p		| 1 5x11 p
// R/L	| 0 izquierda	| 1 derecha

// LIBRERIAS
#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>

#include "LCD/LCD8_.h"

// LOOP PRINCIPAL
int main(void)
{
	DDRB |= (1<<PB0)|(1<<PB1);
	PORTB = 0x00;   // apagado
	
	
	Init_LCD8();
	LCD_Set_Cursor(1,1);
	LCD_Write_Char('P');
	LCD_Write_Char('A');
	LCD_Write_Char('U');
	LCD_Write_Char('L');
	LCD_Write_Char('A');
	LCD_Set_Cursor(1,2);
	LCD_Write_String("HEY!");
	   
    while (1) 
    {
    }
}


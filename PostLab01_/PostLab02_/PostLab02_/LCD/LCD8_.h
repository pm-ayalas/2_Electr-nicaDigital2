
#ifndef LCD8__H_
#define LCD8__H_

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

//#define E(1<<PORTB1)
//#define RS(1<<PORTB0)

void Init_LCD4(void);					// inicializar 4bits
void Init_LCD8(void);					// inicializar 8bits
void LCD_PORT(char a);					// colocar valor en puerto
void LCD_CMD(char a);					// enviar comando
void LCD_Write_Char(char c);			// enviar caracter
void LCD_Write_String(char *a);			// enviar cadena
void LCD_Shift_Right(void);				// desp. derecha
void LCD_Shift_Left(void);				// desp. izquierda
//void LCD_Set_Cursor(char c, char f);	// Establecer Cursor

void LCD_Set_Cursor(unsigned char col, unsigned char fila);
#endif /* LCD8__H_ */
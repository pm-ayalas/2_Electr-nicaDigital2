
#include "LCD8_.h"

void Init_LCD4(void)					// inicializar 8bits
{
}

void Init_LCD8(void)					// inicializar 8bits
{
		DDRD |= (1<<DDD0)|(1<<DDD1)|(1<<DDD2)|(1<<DDD3)|(1<<DDD4)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7);
		PORTD = 0;
		
		LCD_PORT(0x00);
		_delay_ms(20);
		LCD_CMD(0x30);
		_delay_ms(5);
		LCD_CMD(0x30);
		_delay_ms(11);
		LCD_CMD(0x30);
		
		LCD_CMD(0x30);	
		
		// Function Set
		LCD_CMD(0x38);
		
		// Display ON/OFF
		LCD_CMD(0x0C);	
		
		// Entry Mode
		LCD_CMD(0x06);
		
		// Clear Display
		LCD_CMD(0x01);
}

void LCD_PORT(char a)					// colocar valor en puerto
{
	if (a & (1<<0))
	PORTD |= (1<<PORTD0);	// D0=1
	else
	PORTD &= ~(1<<PORTD0);	// D0=0
	
	if (a & (1<<1))
	PORTD |= (1<<PORTD1);	// D1=1
	else
	PORTD &= ~(1<<PORTD1);	// D1=0
	
	if (a & (1<<2))
	PORTD |= (1<<PORTD2);	// D2=1
	else
	PORTD &= ~(1<<PORTD2);	// D2=0
	
	if (a & (1<<3))
	PORTD |= (1<<PORTD3);	// D3=1
	else
	PORTD &= ~(1<<PORTD3);	// D3=0
	
	if (a & (1<<4))
	PORTD |= (1<<PORTD4);	// D4=1
	else
	PORTD &= ~(1<<PORTD4);	// D4=0
	
	if (a & (1<<5))
	PORTD |= (1<<PORTD5);	// D5=1
	else
	PORTD &= ~(1<<PORTD5);	// D5=0
	
	if (a & (1<<6))
	PORTD |= (1<<PORTD6);	// D6=1
	else
	PORTD &= ~(1<<PORTD6);	// D6=0
	
	if (a & (1<<7))
	PORTD |= (1<<PORTD7);	// D7=1
	else
	PORTD &= ~(1<<PORTD7);	// D7=0
	
}

void LCD_CMD(char a)					// enviar comando
{
	// RS
	PORTB &= ~(1<<PORTB0);
	LCD_PORT(a);
	
	// EN transicion
	PORTB |= (1<<PORTB1);
	_delay_ms(4);
	PORTB &= ~(1<<PORTB1);
}

void LCD_Write_Char(char c)				// enviar caracter
{
	PORTB |= (1<<PORTB0);		// RS - 1 Dato en el puerto (CARACTER - DATO)
	LCD_PORT(c);
	
	// EN transicion
	PORTB |= (1<<PORTB1);
	_delay_ms(4);
	PORTB &= ~(1<<PORTB1);
}

void LCD_Write_String(char *a)			// enviar cadena
{
	int i;
	for (i=0; a[i]!='\0'; i++)
	{ LCD_Write_Char(a[i]); }
	
}

void LCD_Shift_Right(void)				// desp. derecha
{
	LCD_CMD(0x01);
	LCD_CMD(0x0C);
}

void LCD_Shift_Left(void)				// desp. izquierda
{
	LCD_CMD(0x01);
	LCD_CMD(0x08);
}

//void LCD_Set_Cursor(char c, char f)		// Establecer Cursor
void LCD_Set_Cursor(unsigned char col, unsigned char fila) {
	unsigned char addr;
	if (fila == 1) {
		addr = 0x80 + (col - 1);   // base 0x00
		} else if (fila == 2) {
		addr = 0xC0 + (col - 1);   // base 0x40 => 0x80 + 0x40 = 0xC0
		} else {
		return;                    // fila no válida
	}
	LCD_CMD(addr);   // ¡en 8 bits se envía el byte completo!
}
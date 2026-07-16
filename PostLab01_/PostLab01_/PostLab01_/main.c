/*
 * Laboratorio01_Digital2.c
 *
 * Created: 7/9/2026 5:41:58 PM
 * Author : ayala
 */ 

// LIBRERÍAS
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Display_seg/Display_seg.h"

// VARIABLES
#define		T1Valor 0x004E				// CTC 5ms
#define		Umbral_r 10					// 10 muestras * 5ms = 50ms

volatile	uint8_t Conteo_1S = 0;		// 200 vueltas = 1Segundo
volatile	uint8_t	Valor_Conteo = 5;
volatile	uint8_t Cuenta_Regresiva = 0;

volatile	uint8_t	Conteo_J1 = 0;
volatile	uint8_t	Conteo_J2 = 0;

volatile	uint8_t	Bandera_BTN_inicio = 0;
volatile	uint8_t	Bandera_BTN_J1 = 0;
volatile	uint8_t	Bandera_BTN_J2 = 0;

volatile	uint8_t	prev_BTN_inicio = 0;
volatile	uint8_t	prev_BTN_J1 = 0;
volatile	uint8_t	prev_BTN_J2 = 0;

// FUNCTION PROTOTYPES
void	setup(void);
void	init_PinChange(void);
void	init_TMR1(void);

// MAIN LOOP
int main(void)
{
	cli();
	setup();
	init_PinChange();
	init_TMR1();
	sei();
	
    while (1) 
    {
		// Verificar Bandera BTNS
		if (Bandera_BTN_inicio){
			Bandera_BTN_inicio = 0;		// limpiar bandera
			Cuenta_Regresiva = 1;
		}
    }
}

// NON-INTERRUPT SUBROUTINES
void	setup(void){
	// Frecuencia Reloj 1MHz
	CLKPR	= (1<<CLKPCE);
	CLKPR	= (1<<CLKPS2);
	
	UCSR0B	= 0x00;	// Apagar pines - UART
	
	// PORTD -> Salida - Display
	DDRD	= 0xFF;
	PORTD	= 0xFF;			// Apagados
	
	// PORTB -> Salida - Contador J1
	DDRB	|= (1 << PINB1) | (1 << PINB2) | (1 << PINB3) | (1 << PINB0);
	PORTB	&= ~((1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB0));
	
	// PORTC -> Salida - Contador J2
	DDRC	|= (1 << PINC1) | (1 << PINC2) | (1 << PINC3) | (1 << PINC0);
	PORTC	&= ~((1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC0));
	
	// PC4,PC5,PC6 -> Entrada [Botones] pull-up 
	DDRC	&= ~((1 << PC4) | (1 << PC5) | (1 << PC6));
	PORTC	|= (1 << PC4) | (1 << PC5) | (1 << PC6);
}

void	init_PinChange(void){
	// Habilitar interrupciones para PCINT1 (PORTC)
	PCICR	|= (1<<PCIE1);
	// PC4 (PCINT12) PC5 (PCINT13) PC6 (PCINT14)
	PCMSK1 |= (1<<PCINT12) | (1<<PCINT13) | (1<<PCINT14);
}

void	init_TMR1(void){
	TCCR1B = 0;
	
	// Modo CTC
	TCCR1B |= (1<<WGM12);
	// Prescaler 64
	TCCR1B |= (1<<CS11)|(1<<CS10);
	// Valor para comparación
	OCR1A = T1Valor;
	// Habilitar interrupcion
	TIMSK1 |= (1<<OCIE1A);
	
	TCNT1 = 0; // Iniciar contador en 0
}

// INTERRUPT ROUTINES

ISR(PCINT1_vect){
	// Leer PORTC
	uint8_t estado_actual = PINC;
	
	// Selección pines específicos --- TEMPORALMENTE SOLO PC4
	uint8_t actual_PC4 = (estado_actual >> PC4) & 1;
	
	if ((prev_BTN_inicio == 0) && (actual_PC4 == 1)){
		Bandera_BTN_inicio = 1;
	}
	prev_BTN_inicio = actual_PC4;
}

ISR(TIMER1_COMPA_vect){
	
	if (Cuenta_Regresiva){
		
		Conteo_1S++;
		if (Conteo_1S == 200){
			if (Valor_Conteo < 6){
				Valor_Conteo--;
				}else{
				Valor_Conteo = 0;
				Cuenta_Regresiva = 5;
			}
		}

		Mostrar_Valor(Valor_Conteo);
		
	}


}
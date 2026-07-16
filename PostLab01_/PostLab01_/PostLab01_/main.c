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
#define		Umbral_btn 11				// 10 muestras * 5ms = 50ms
#define		META 9

volatile	uint8_t Conteo_btn1 = 0;
volatile	uint8_t Conteo_btn2 = 0;
volatile	uint8_t Conteo_btn3 = 0;
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

volatile	uint8_t	Bandera_Carrera_Activa = 0;	// 1 = jugadores pueden avanzar
volatile	uint8_t	Bandera_Ganador = 0;			// 1 = carrera terminada

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
		// Verificar Bandera BTN inicio
		if (Bandera_BTN_inicio){
			Bandera_BTN_inicio = 0;		// limpiar bandera
			Valor_Conteo = 5;
			Conteo_1S = 0;
			Bandera_Carrera_Activa = 0;
			Bandera_Ganador = 0;
			Conteo_J1 = 0;
			Conteo_J2 = 0;
			PORTB &= ~0x2F;		// apagar leds J1 (bits 0-3) y bit4 J2 (PB5)
			PORTC &= ~0x07;		// apagar leds J2 (bits 0-2)
			Cuenta_Regresiva = 1;
		}
		
		// Verificar Bandera BTN J1
		if (Bandera_BTN_J1){
			Bandera_BTN_J1 = 0;
			Conteo_J1++;
			PORTB = (PORTB & 0xF0) | (Conteo_J1 & 0x0F);
		}
		
		// Verificar Bandera BTN J2
		if (Bandera_BTN_J2){
			Bandera_BTN_J2 = 0;
			Conteo_J2++;
			PORTC = (PORTC & ~0x07) | (Conteo_J2 & 0x07);
			if (Conteo_J2 & 0x08){
				PORTB |= (1 << PB5);
			}else{
				PORTB &= ~(1 << PB5);
			}
		}
		
		// Verificar ganador
		if (Bandera_Carrera_Activa && !Bandera_Ganador){
			if (Conteo_J1 >= META){
				Bandera_Ganador = 1;
				Bandera_Carrera_Activa = 0;
				PORTB |= 0x0F;
				PORTC &= ~0x07;
				PORTB &= ~(1 << PB5);
				Mostrar_Valor(1);
			}else if (Conteo_J2 >= META){
				Bandera_Ganador = 1;
				Bandera_Carrera_Activa = 0;
				PORTC |= 0x07;
				PORTB |= (1 << PB5);
				PORTB &= ~0x0F;
				Mostrar_Valor(2);
			}
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
	
	// PORTB -> Salida - Contador J1 (bits 0-3) y 4to led J2 (PB5)
	DDRB	|= (1 << PINB1) | (1 << PINB2) | (1 << PINB3) | (1 << PINB0) | (1 << PINB5);
	PORTB	&= ~((1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB0) | (1 << PB5));
	
	// PORTC -> Salida - Contador J2 (bits 0-2)
	DDRC	|= (1 << PINC1) | (1 << PINC2) | (1 << PINC0);
	PORTC	&= ~((1 << PC1) | (1 << PC2) | (1 << PC0));
	
	// PC3,PC4,PC5 -> Entrada [Botones Inicio,J1,J2] pull-up 
	DDRC	&= ~((1 << PC3) | (1 << PC4) | (1 << PC5));
	PORTC	|= (1 << PC3) | (1 << PC4) | (1 << PC5);
}
void	init_PinChange(void){
	// Habilitar interrupciones para PCINT1 (PORTC)
	PCICR	|= (1<<PCIE1);
	// PC3 (PCINT11) Inicio, PC4 (PCINT12) J1, PC5 (PCINT13) J2
	PCMSK1 |= (1<<PCINT11) | (1<<PCINT12) | (1<<PCINT13);
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
	// Leer PORTC - Botones Inicio (PC3), J1 (PC4), J2 (PC5)
	uint8_t estado_actual = PINC;
	
	uint8_t actual_PC3 = (estado_actual >> PC3) & 1;
	uint8_t actual_PC4 = (estado_actual >> PC4) & 1;
	uint8_t actual_PC5 = (estado_actual >> PC5) & 1;
	
	if ((Conteo_btn1 == 0) && (actual_PC3 != prev_BTN_inicio)){
		Conteo_btn1 = 1;
	}
	if ((Conteo_btn2 == 0) && (actual_PC4 != prev_BTN_J1)){
		Conteo_btn2 = 1;
	}
	if ((Conteo_btn3 == 0) && (actual_PC5 != prev_BTN_J2)){
		Conteo_btn3 = 1;
	}
}
ISR(TIMER1_COMPA_vect){
	
	if (Conteo_btn1>0){
		Conteo_btn1++;
		
		if (Conteo_btn1 == Umbral_btn){
				uint8_t actual_PC3 = (PINC >> PC3) & 1;
				
				// Pull-up: reposo = 1, presionado = 0
				if ((prev_BTN_inicio == 1) && (actual_PC3 == 0) && (Cuenta_Regresiva == 0)){
					Bandera_BTN_inicio = 1;
				}
				prev_BTN_inicio = actual_PC3;
				Conteo_btn1 = 0;
		}
	}
	
	if (Conteo_btn2>0){
		Conteo_btn2++;
		
		if (Conteo_btn2 == Umbral_btn){
				uint8_t actual_PC4 = (PINC >> PC4) & 1;
				
				if ((prev_BTN_J1 == 1) && (actual_PC4 == 0) && Bandera_Carrera_Activa && !Bandera_Ganador){
					Bandera_BTN_J1 = 1;
				}
				prev_BTN_J1 = actual_PC4;
				Conteo_btn2 = 0;
		}
	}
	
	if (Conteo_btn3>0){
		Conteo_btn3++;
		
		if (Conteo_btn3 == Umbral_btn){
				uint8_t actual_PC5 = (PINC >> PC5) & 1;
				
				if ((prev_BTN_J2 == 1) && (actual_PC5 == 0) && Bandera_Carrera_Activa && !Bandera_Ganador){
					Bandera_BTN_J2 = 1;
				}
				prev_BTN_J2 = actual_PC5;
				Conteo_btn3 = 0;
		}
	}
	
	if (Cuenta_Regresiva){
		
		Mostrar_Valor(Valor_Conteo);
		Conteo_1S++;
		if (Conteo_1S >= 200){
			Conteo_1S = 0;
			if (Valor_Conteo > 0){
				Valor_Conteo--;
			}else{
				Cuenta_Regresiva = 0;
				Bandera_Carrera_Activa = 1;	// fin conteo, habilitar botones
			}
		}
	}
}
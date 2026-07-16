/*
 * Display_seg.c
 *
 * Created: 7/9/2026 5:56:49 PM
 *  Author: ayala
 */ 

#define F_CPU 16000000
#include "Display_seg.h"

const uint8_t SEGMENTOS[16] = {
	0x40,    // 0
	0x75,    // 1
	0x22,    // 2
	0x24,    // 3
	0x15,    // 4
	0x0C,    // 5
	0x08,    // 6
	0x65,    // 7
	0x00,    // 8
	0x05,    // 9
	0x01,    // A
	0x18,    // b
	0x4A,    // C
	0x30,    // d
	0x0A,    // E
	0x0B	 // F
};

void Mostrar_Valor(uint8_t Valor){
	PORTD = SEGMENTOS[Valor];
}



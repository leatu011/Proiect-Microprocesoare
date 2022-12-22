#include "gpio.h"
#include "Uart.h"
#define Vibration_PIN (4) // PORT D
int counter=0;

void Vibration_Init(void) {
	
	// Activarea semnalului de ceas pentru a putea folosi GPIO cu ajutorul pinului 1 de pe portul C
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

	
	// Utilizarea GPIO impune selectarea variantei de multiplexare cu valorea 1
	PORTD->PCR[Vibration_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[Vibration_PIN] |= PORT_PCR_MUX(1);
	
	// Activare întreruperi pe rising edge
	PORTD->PCR[Vibration_PIN] |= PORT_PCR_IRQC(0x09) | PORT_PCR_PE_MASK;
	
	// Activare întrerupere pentru a folosi vibration
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_SetPriority(PORTD_IRQn, 128);
	NVIC_EnableIRQ(PORTD_IRQn);
	
}

void PORTD_IRQHandler() {
	
		
	if((GPIOD_PDIR & (1<<4))){
		counter++;
		}
	
	PORTD_ISFR = (1<<Vibration_PIN);
}






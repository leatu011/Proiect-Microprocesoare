#include "Pit.h"
#include "Uart.h"
#include "ADC.h"
#include "gpio.h"
#include "Pwm.h"
int seconds=20;
int count_sec=21;
void UTILS_PrintCounter(uint16_t count){ 
	uint8_t Digit1=count /1000; 
	uint8_t Digit2=count %1000 /100; 
	uint8_t Digit3=count %100 /10; 
	uint8_t Digit4=count %10;  
	UART0_Transmit(Digit1+48);
	UART0_Transmit(Digit2+48);
	UART0_Transmit(Digit3+48);
	UART0_Transmit(Digit4+48);
	UART0_Transmit(0x0D);
	UART0_Transmit(0x0A);
	
}

void PIT_Init(void) {
	
	// Activarea semnalului de ceas pentru perifericul PIT
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	// Utilizarea semnalului de ceas pentru tabloul de timere
	PIT_MCR &= ~PIT_MCR_MDIS_MASK;
	// Oprirea decrementarii valorilor numaratoarelor in modul debug
	//PIT->MCR |= PIT_MCR_FRZ_MASK;
	// Setarea valoarea numaratorului de pe canalul 0 la o perioada de 1 secunda
	PIT->CHANNEL[0].LDVAL = 0x9FFFFF;
	
  // Activarea întreruperilor pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	// Activarea timerului de pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	
	// Setarea valoarea numaratorului de pe canalul 0 la o perioada de 0.1 secunde
	PIT->CHANNEL[1].LDVAL = 0xFFFFF;
	
  // Activarea întreruperilor pe canalul 0
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK;
	// Activarea timerului de pe canalul 0
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
	
	// Activarea întreruperii mascabile si setarea prioritatiis
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_SetPriority(PIT_IRQn,5);
	NVIC_EnableIRQ(PIT_IRQn);
}

void PIT_IRQHandler(void) {
	
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		//Brightness_Control(); 
		if(state==1){
			adcRead(adcSelect());
			seconds=ADC0->R[0]/100;
			state++;
			Vibration_Init();
			count_sec=0;
		}	
		if(count_sec==seconds)
		{
			UTILS_PrintCounter(counter);
			count_sec++;
			counter=0;
		}
		else{
			count_sec++;
		}
		
		
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
	}
	
	if(PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) {
		Brightness_Control(); 
		if(state<2){
			adcRead(adcSelect());
			UTILS_PrintCounter(ADC0->R[0]/100);
		}	
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
	}
}


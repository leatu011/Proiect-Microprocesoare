#include "Uart.h"
#include "gpio.h"
#include "ADC.h"
#include "Pit.h"
#include "Pwm.h"

int main()
{
	UART0_Init(115200);
	adcInit();
	TPM2_Init();
	PIT_Init();
	for(;;){}
}

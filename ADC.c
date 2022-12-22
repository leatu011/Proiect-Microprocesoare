#include "ADC.h"
#include "Uart.h"

#define PIN_Rotation (0) //PORT B

int channel_pin;

void adcInit(){
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK; // enable CLK ADC0

    ADC0->CFG1 |= ADC_CFG1_ADLSMP_MASK;        
    ADC0->CFG2 = ADC_CFG2_ADLSTS(0x12&3);
    

    ADC0->CFG1 |=ADC_CFG1_ADIV(1)    // Clock Divide Select: (Input Clock)/2 : 24MHz/2=12MHz
               | ADC_CFG1_MODE(2)               // 10 bits Resolution
               | ADC_CFG1_ADICLK(0);    // Input Clock: (Bus Clock=24MHz)

  /* ADC0_CFG2: ADACKEN=1,ADHSC=0,ADLSTS=0 for speed up */
  ADC0->CFG2 = (uint32_t)((ADC0->CFG2 & (uint32_t)~(uint32_t)(
               ADC_CFG2_ADHSC_MASK |
               ADC_CFG2_ADLSTS(0x03)
              )) | (uint32_t)(
               ADC_CFG2_ADACKEN_MASK
              ));

    ADC0->SC3 = ADC_SC3_AVGE_MASK       // Hardware Average Enable
              | ADC_SC3_AVGS(0x10&3);   // which number of avg samples
}


int adcSelect(){

    channel_pin=-1;
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;// enable clock
    PORTB->PCR[PIN_Rotation] &= ~PORT_PCR_MUX_MASK;// analog Input
    channel_pin=8;  
    return channel_pin;
}

void adcRead(int channel) {
    // start conversion
    if(channel>=0){
    ADC0->SC1[0] = ADC_SC1_ADCH(channel & ~(1 << 5));
    }
}

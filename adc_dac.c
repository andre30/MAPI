
#include <stm32f30x.h>
int tensione;
int tensione2;
int calibrazione=0;
int  dac;
int dac1;
void main(void){

RCC->AHBENR |= 1<<28;// ABilito BUS CLK degli ADC 1 e 2 perchË sono in coppia
RCC->AHBENR |= 1<<17;
RCC->CFGR2 |=1<<8;//clock diviso per 2
ADC1_2->CCR |= 1<<17;
GPIOA->MODER &= ~ ((unsigned int)3); // INPUT PORTA A

ADC1->CR &= 0xCFFFFFFF; // pongo a 00 gli ultimi bit fi ADVREGEN
ADC1->CR |= 1<<28; //pomgo 01 in ADVREGEN
for(int i=0;i<721;i++);// aspetto 10 micro secondi

ADC1->CR |= 1<<31; //abilito ADCAL
while( (ADC1->CR & 1<<31) == 1<<31);// attesa fine calibrazione
ADC1->CR |= 1;//pongo aden a 1
while( !(ADC1->ISR & 1)== 1);// il bit ADRDY si Ë alzato ed Ë pronto per la conversione
ADC1->SQR1 |=0<<0; // CAMPO L
ADC1->SQR1 |= 3<<6; 
ADC1->SMPR1 |= 7<<9;


RCC->APB1ENR |= 1<<29; // DAC
DAC->CR |= 1;
GPIOA->MODER|= 3<<8; // abilito PA4 che è associata a DAC1_OUT1  e PA4 sta nel cmpo MODER4  non si sa perchè
DAC->DHR12R1 = 1234*4096/3000;
while(1){
    ADC1->CR |= 1<<2;
    while( (ADC1->ISR & 1<<2)==0);// attendo si alzi End Of Conversion
    dac=DAC->DOR1;
    dac1= 3000*dac/4095;
    calibrazione=ADC1->CALFACT;
    tensione =ADC1->DR;
    tensione2=( (tensione*3000)/4096);
  }

}

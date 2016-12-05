/* (1) MEMORY TO MEMORY vettori

#include <stm32f30x.h>
#define N 32
int sorgente[N];
int destinazione[N];
void main(void){

  RCC->AHBENR |= 1<<0;          // clock al DMA
  DMA1_Channel1->CCR |= 1<<14; //mem to mem mode
  DMA1_Channel1->CCR |=1<<11; //mem size 32 bit 10
  DMA1_Channel1->CCR |=0<<4; // direzione da periferica a memoria, con 1 è il contrario
  DMA1_Channel1->CCR |= 1<<9; //psize (nel nostro caso è sempre memoria) 32 bit 10
  DMA1_Channel1->CCR |= 1<<7; //increment mode abilitata- minc a 1
  DMA1_Channel1->CCR |= 1<<6; //increment mode abilitata- pinc a 1
 // DMA1_Channel1->CCR |= 1<<5; //buffer circolare abilitato 1
  DMA1_Channel1->CNDTR |= 1<<5; //32 transazioni
  
  for(int i=0; i<N; i++){
    sorgente[i] = 100 + i;
  }
  DMA1_Channel1->CPAR |= (uint32_t)&sorgente;
  
  
   for(int i=0; i<N; i++){
    destinazione[i] = 0;
  }
 
  DMA1_Channel1->CMAR |= (uint32_t)&destinazione;
  
 DMA1_Channel1->CCR |= 1<<0; // faccio partire il DMA
 
 
 while((DMA1->ISR & 1) == 1);
  DMA1->ISR &=0<<1;
  
}

*/

// (2) PERIPHERIC TO MEMORY (ADC)

#include <stm32f30x.h>
void main(void){

  RCC->APB1ENR |= 1<<0; //TIM2
  RCC->AHBENR |= 1<<28; //abilito ADC1 e 2  noi useremo ADC1
  
  //configurazione ADC con trigger su TIM2 upd
  ADC1->CFGR |=3<<10; //EXTEN a 11 : abilito trigger esterno
  ADC1->CFGR |=1011<<0; // EXTSEL metto il numero di evento del tim 2
  
  RCC->AHBENR |= 1<<28;// ABilito BUS CLK degli ADC 1 e 2 perchË sono in coppia
  RCC->AHBENR |= 1<<17;
  RCC->CFGR2 |=1<<8;//clock diviso per 2
  ADC1_2->CCR |= 1<<17;
  GPIOA->MODER &= ~ ((unsigned int)3); // INPUT PORTA A
  
ADC1->CR &= 0xCFFFFFFF; // pongo a 00 gli ultimi bit di ADVREGEN
ADC1->CR |= 1<<28; //pongo 01 in ADVREGEN
for(int i=0;i<721;i++);// aspetto 10 micro secondi
ADC1->CR |= 1<<31; //abilito ADCAL
while( (ADC1->CR & 1<<31) == 1<<31);// attesa fine calibrazione
ADC1->CR |= 1;//pongo aden a 1
while( !(ADC1->ISR & 1)== 1);// il bit ADRDY si è alzato ed e' pronto per la conversione
ADC1->SQR1 |=0<<0; // CAMPO L
ADC1->SQR1 |= 3<<6; 
ADC1->SMPR1 |= 7<<9;

//configurazione TIM2
TIM2->PSC |=0;
TIM2->ARR |=1000;


  
//Configurazione DMA
  RCC->AHBENR |= 1<<0;          // clock al DMA
  DMA1_Channel1->CCR |= 1<<14; //per to mem mode
  DMA1_Channel1->CCR |=1<<11; //mem size 32 bit 10
  DMA1_Channel1->CCR |=0<<4; // direzione da periferica a memoria, con 1 è il contrario
  DMA1_Channel1->CCR |= 1<<9; //psize (nel nostro caso è sempre memoria) 32 bit 10
  DMA1_Channel1->CCR |= 1<<7; //increment mode abilitata- minc a 1
  DMA1_Channel1->CCR |= 1<<6; //increment mode abilitata- pinc a 1
  DMA1_Channel1->CCR |= 1<<5; //buffer circolare abilitato 1
  DMA1_Channel1->CNDTR |= 1<<5; //32 transazioni

  while(1){
    ADC1->CR |= 1<<2;
    while( (ADC1->ISR & 1<<2)==0);// attendo si alzi End Of Conversion
    tensione =ADC1->DR;
    tensione2=( (tensione*3000)/4096);
  }

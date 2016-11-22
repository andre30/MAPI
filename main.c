#include <stm32f30x.h>
#define TIMINT (uint32_t)1
#define TIM2EN (uint32_t)0x1
#define FCK 72000000
#define FCKMIL 72000
#define FCKMIC 72
#define UIF (uint32_t)0x1
int secondi=0;
int milli=0;
int micro;
unsigned int count=0;
double countmil=0;
double countmic=0;


void main(void){
  
  RCC->AHBENR |= 1<<21;
  RCC->AHBENR |=1<<17; //abilito porta A
  GPIOE->MODER |= 0x55550000;
  GPIOA->MODER &= ~ ((unsigned int)3); //00 nei due bit meno significativi
  RCC->APB1ENR |= 0x1; //abilito TIM2
  TIM2->CR1 = 0; //Registro di controllo in cui inizialmente disabilito tutte le opzioni su TIM6
  TIM2->CR2 =0;
  TIM2->PSC = 0;
  TIM2->CNT=0; //imposto conteggio a 0
  TIM2->ARR = (uint32_t) TIMINT*FCKMIC-1; //fisso il registro di arr al massimo
  
  while(1){
    //questo if serve a capire se il tasto è stato premuto. Nel registro IDR il bit 0 è collegato al pin PA0
    while(GPIOA->IDR & 0x1 == 0x1){ 
      if((TIM2->CR1 &0x1) != 0x1){
        TIM2->CR1 |= (uint32_t)0x1; //CEN abilito conteggio
        GPIOE->ODR |= (uint32_t)0x0000FF00;
      }
      if((TIM2->SR & UIF)==UIF){ //ogni secondo//microsecondo scatta l' interrupt e si alza il flag in uif => incremento il conteggio
        TIM2->SR &= ~ UIF;
        (count)++;
        secondi=count/1000000;
        milli=count/1000;
        micro=count;
        
        
      }
    }
    
    TIM2->CR1 &=~((unsigned int)1); //disabilito conteggio
    GPIOE->ODR &= 0x0;
     
      
    }
}
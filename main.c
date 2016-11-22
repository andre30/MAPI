#include <stm32f30x.h>
#define TIMINT 1
#define FCK  (uint32_t)72000000
#define TIM2EN (uint32_t)0x00000001   
#define CEN (uint32_t)0x0001
#define UIF (uint32_t)0x00000001
#define PE815OUT (uint32_t)0x55550000
#define FCKS 72000000
#define FCKMIL 72000
#define FCKMIC 72
   int count;
  int secondi=0;
  int milli=0;
  int micro=0;
void main(void)
{
  RCC->AHBENR |= 1<<21; //attivo porta E
  RCC->AHBENR |=1<<17; //abilito porta A
  RCC->APB1ENR |= TIM2EN;//ABILITO TIMER 2 HA COME MAX FREQ DI FUNZIONAMENTO META' DI QUELLA DEL CLOCK
  GPIOE ->MODER |= PE815OUT; 
   GPIOA->MODER &= ~ ((unsigned int)3);//abilito pin da 8 a 15
  //configurazione timer
  TIM2->PSC=0;
  TIM2->CNT =0; //TIMINT è il numero di secondi che voglio attivare, scrivo 1 secondo nell' arr
  TIM2->CR1=0;
  //attendo che un secondo sia trascorso
  while(1)
  {
    while(GPIOA->IDR & 0x1 == 0x1){ 
      if((TIM2->CR1 &0x1) != 0x1){
        TIM2->CR1 |= (uint32_t)0x1; //CEN abilito conteggio
        GPIOE->ODR |= (uint32_t)0x0000FF00;
      }
      count=TIM2->CNT;
      secondi=count/FCKS;
      micro=count/FCKMIC;
      milli=count/FCKMIL;
      TIM2->CR1 &=0x0;
    }
  }   
}
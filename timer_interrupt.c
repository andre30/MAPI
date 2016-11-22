/* abilito clock tim2 ,gpioa e syscfg
configuro pa0 come input aspetto presione user avvio tim2 (bit CEN)
abilito clock
configuro PA0 come input per EXTI0
NVIC->ISER[0] |= 0x1<<6;
Abilito fronte di salita e discesa EXTI0
while(1) [faccio qualcosa]

ISR EXTI0_IRQHandler
Azzero PR0
Pressione User ?
Avvio TIM2 (bit CEN di CR1)
Rilascio USER?
Fermo TIM2
Visualizzo contenuto registro CNT di TIM2
Azzero CNT

*/

#include <stm32f30x.h>


 long int s,ml, ms, count;
void main(void){

RCC->AHBENR |= 1<<21;
RCC->AHBENR |= 1<<17;
RCC->APB1ENR |= 0x1; //TIM2
RCC->APB1ENR |= 0x1<<1; //TIM3
RCC->APB2ENR |=1<<0; //abilito syscfg
GPIOE->MODER = 0x55550000;
GPIOA->MODER &= ~ ((unsigned int)3); // INPUT 

TIM2->ARR= 0xFFFFFFFF;
TIM2->PSC=0;
TIM2->CNT=0;

SYSCFG->EXTICR[0] |= 0000<<0; //PA0 come input per EXTI0
EXTI->IMR |= 0x00000001; //abilito linea interrupt per EXTI0
NVIC->ISER[0] |= 0x1<<6; //abilito la routine di EXTI0

//abilito fronte di salita e di discesa
EXTI->FTSR |= 0x1;
EXTI->RTSR |= 0x1;

while(1){
  for(int i=8; i<16; i++){ 
    GPIOE->ODR |= 1<<i;
    for(int j=0; j<2000000; j++);
   GPIOE->ODR = 0<<i;
   }
}
}

void EXTI0_IRQHandler(){
  
  EXTI->PR |= 0x1; //azzero PR0
  
 while(GPIOA->IDR & 0x1 == 0x1){ 
      if((TIM2->CR1 &0x1) != 0x1){
        TIM2->CR1 |= (uint32_t)0x1; //CEN abilito conteggio
        GPIOE->ODR |= (uint32_t)0x0000FF00;
      }
      count=TIM2->CNT;
      s=count/72000000;
      ms=count/72000;
      ml=count/72;
      TIM2->CR1 &=0x0;
    }
 GPIOE->ODR &=0;
  
  
  
  
  
  
}
  

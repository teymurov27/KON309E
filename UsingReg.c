#include "stm32f10x.h"


int main(void)
{
	
	RCC->APB2ENR |= (0x1 << 4); // Enable clock for PortA
	GPIOC->CRH &= ~(0xF << 20);  // Clear settings for PC13
	GPIOC->CRH |= ((uint32_t)0x6 << 28);  // CNF:MODE (Output|Open Drain|2MHz)

	
	
	GPIOC->CRH &= ~((uint32_t)0x8 << 24);  // Clear settings for PC14
	GPIOC->CRH |= ((uint32_t)0x8 << 24);   // CNF:MODE (Input|Pull-Up|Reserved)
	
	GPIOC->CRH &= ~((uint32_t)0x8 << 28);  // Clear settings for PC15
	GPIOC->CRH |= ((uint32_t)0x8 << 28);   // CNF:MODE (Input|Pull-Up|Reserved)
	
	
	delayInit();

	while(1)
	{
		
		
		
		if(!(GPIOC->IDR & (1<<14))) // InputDataRead on GPIO port C. This equals to if(!(GPIOA->IDR & ((0000 0000 0000 0001) << 4))) and from the bitwise & and Truth table operation it's if(!(0000 0000 0000 0000)) for button pressed and if(!(0000 0000 0001 0000)) for not pressed.
        {
					
					
					
					while(1){
					  loop_first:
					GPIOA->BRR |= (1 << 13); // Turn off LED on PA0
		      delayMs(1000);		   
		      GPIOA->BSRR |= (1 << 13); // Turn on LED on PA0
		      delayMs(1000);
					if(!(GPIOC->IDR & (1<<14)))
						goto loop_end;	
					if(!(GPIOC->IDR & (1<<15)))
						goto loop_second;
					}
					
					
					while(1){
			      loop_second:		   
		      GPIOA->BRR |= (1 << 13); // Turn off LED on PC13
		      delayMs(500);		   
		      GPIOA->BSRR |= (1 << 13); // Turn on LED on PC13
		      delayMs(500);
					if(!(GPIOC->IDR & (1<<14)))
						goto loop_end; // If button on PC14 pressed it goes loop_end which break out of loop, eventually LED Turns Off.
					if(!(GPIOC->IDR & (1<<15)))
						goto loop_first; // If button on PC15 pressed it goes to loop_first which is the only difference of 
					
					}
					
					
					  loop_end:
					GPIOA->BRR |= (1 << 13); // Turn off LED on PC13
					  break;
						
					         							
        }
				
				
		
				
	}
	
	
	
}




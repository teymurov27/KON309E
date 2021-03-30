/*
              Istanbul Technical University
           Faculty of Electical and Electronics
	  Department of Controlling and Automation Engineering 
	
	        SAMIR TEYMUROV           ID: 040190922
*/
	
	

/* I hereby declare that code below and all of its components are written by me only.
If you have seen any part of my code in some other student please contact me directly.
*/

#include "stm32f10x.h"
#include "delay.h"
	
GPIO_InitTypeDef GPIO_InitStructure;

int main (void)
{
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // enabling clock for port a.

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // pin 0 as push-pull output
GPIO_Init(GPIOA, &GPIO_InitStructure);
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // pin 1 as push-pull output
GPIO_Init(GPIOA, &GPIO_InitStructure);
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // pin 2 as push-pull output
GPIO_Init(GPIOA, &GPIO_InitStructure);
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // pin 3 as pull-up input
GPIO_Init(GPIOA, &GPIO_InitStructure);

delayInit();

while(1)
{			
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)) 
	{
		while(1) // while button is pressed for the first time all leds are on.
		{
			loop1:
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			GPIO_SetBits(GPIOA, GPIO_Pin_1);
			GPIO_SetBits(GPIOA, GPIO_Pin_2);
			if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)) // if button released for the first time it goes to second loop.
			{
				goto loop2;
			}
		}		
		while(1) // here at first we need to turn off all leds, then it startes blinking.
		{
			loop2:
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);
			GPIO_ResetBits(GPIOA, GPIO_Pin_2);
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
				for(int i = 0; i < 10; i++) // for reading instantly I divided the delays for 10 pieces of for loops which contains 50 ms each. And overall delay is 500 ms which is same as instructions.
					{
						if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)) // in all of these for loops it checks for if button is pressed or not, if pressed then it goes to third loop.
							{
								goto loop3;
							}
						delayMs(50);
					}		
				GPIO_ResetBits(GPIOA, GPIO_Pin_0);
				GPIO_SetBits(GPIOA, GPIO_Pin_1);
				for(int j = 0; j < 10; j++)
					{
						if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
							{
								goto loop3;
							}
						delayMs(50);
					}		
				GPIO_ResetBits(GPIOA, GPIO_Pin_1);
				GPIO_SetBits(GPIOA, GPIO_Pin_2);
				for(int k = 0; k < 10; k++)
					{
						if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
							{
								goto loop3;
							}
						delayMs(50);
					}		
				GPIO_ResetBits(GPIOA, GPIO_Pin_2);
		}
		
		while(1) // this loop is doing led on thing while button is pressed like loop 1, but unlike loop 1 it turns off leds immediately after button released.
		{
			loop3:
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			GPIO_SetBits(GPIOA, GPIO_Pin_1);
			GPIO_SetBits(GPIOA, GPIO_Pin_2);
			if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
			{
				goto loop4;
			}
		}
		
		while(1) // this loop is for turning leds off and if button pressed after turning off the leds it goes to directly loop 1 and it starts again.
		{
			loop4:
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);
			GPIO_ResetBits(GPIOA, GPIO_Pin_2);
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
			{
				goto loop1;
			}
	
		}	
	}					
}
}













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
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

int ledGreen;
int button;

int main (void)
{
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // enabling clock for port a.
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // enabling clock for afio port

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

GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);

EXTI_DeInit();
EXTI_InitStructure.EXTI_Line = EXTI_Line3;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);

NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

delayInit();



while(1)
{	
			button = 0;	
			GPIO_SetBits(GPIOA, GPIO_Pin_0); // red led on
			delayMs(10000);			
			GPIO_ResetBits(GPIOA, GPIO_Pin_0); // red led off
						
			GPIO_SetBits(GPIOA, GPIO_Pin_1); // yellow led on
			delayMs(2000);
			GPIO_ResetBits(GPIOA, GPIO_Pin_1); // yellow led off
			
			button = 1;
			GPIO_SetBits(GPIOA, GPIO_Pin_2); // green led on
			for(int i = 0; i < 1000; i++)
				{
					if(ledGreen == 1)
					{
						goto yellow;
					}
					delayMs(10);
				}
							
				yellow:				
			button = 0;
			ledGreen = 0;
			GPIO_ResetBits(GPIOA, GPIO_Pin_2); // green led off
			GPIO_SetBits(GPIOA, GPIO_Pin_1); // yellow led on
			delayMs(2000);
			GPIO_ResetBits(GPIOA, GPIO_Pin_1); // yellow led off
										
}
}















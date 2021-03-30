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
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

int state;
int timer_flag;
int timerCounter;


void delay_ms(uint16_t ms)
{
	TIM_SetCounter(TIM3, 0);    // make sure TIM3 Counter start from zero
	TIM_Cmd(TIM3, ENABLE);      
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
	while(TIM_GetCounter(TIM3) < ms);   // it is not exactly milliseconds, 1 second corresponds to 2000 time period
	TIM_Cmd(TIM3, DISABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
}

	

int main (void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // enabling clock for port a.
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // enabling clock for afio port
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // enabling  clock for timer2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // enabling  clock for timer2

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
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // pin 4 as push-pull output
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // pin 5 as push-pull output
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);

	EXTI_DeInit();
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 49999; // total of 24 seconds
	TIM_TimeBaseStructure.TIM_Prescaler = 17279; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 8333; // CC1 divided into 2 second pulses
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 3999; // 2 seconds
	TIM_TimeBaseStructure.TIM_Prescaler = 35999;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);	

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	


	
	while(1)
	{
		switch(state)
		{
			case 1:
				GPIO_ResetBits(GPIOA, GPIO_Pin_4); // red led off for pedestrians
				GPIO_SetBits(GPIOA, GPIO_Pin_5); // green led on for pedestrians
				GPIO_SetBits(GPIOA, GPIO_Pin_0); // red led on
				timer_flag = 1; // delay 10 second
				GPIO_ResetBits(GPIOA, GPIO_Pin_0); // red led off
			break;
			
			case 2:
				GPIO_ResetBits(GPIOA, GPIO_Pin_5); // green led off for pedestrians
				GPIO_SetBits(GPIOA, GPIO_Pin_4); // red led on for pedestrians
				GPIO_SetBits(GPIOA, GPIO_Pin_1); // yellow led on
				timer_flag = 2; // delay 2 second
				GPIO_ResetBits(GPIOA, GPIO_Pin_1); // yellow led off
			break;
			
			case 3:
				GPIO_SetBits(GPIOA, GPIO_Pin_2); // green led on for cars		 	
				timer_flag = 3; // delay 10 second
				GPIO_ResetBits(GPIOA, GPIO_Pin_2); // green led off for cars
			break;
			
			case 4:
				timerCounter = 1;
				GPIO_SetBits(GPIOA, GPIO_Pin_2); // yellow led on for cars
				delay_ms(3999);
				GPIO_ResetBits(GPIOA, GPIO_Pin_2); // yellow led off for cars
				state = 5;
				
			break;			
			
			case 5:				
				// timerCounter == 1 version is for button pressed event.				
				if(timerCounter == 1)
				{
					GPIO_SetBits(GPIOA, GPIO_Pin_1); // yellow led on for cars
					delay_ms(3999); // delay 2 second
					GPIO_ResetBits(GPIOA, GPIO_Pin_1); // yellow led off for cars
					state = 6;
				}
				
				// timerCounter == 0 version is for normal cycle. Button not pressed.
				else
				{
					GPIO_SetBits(GPIOA, GPIO_Pin_1); // yellow led on for cars
					timer_flag = 4; // delay 2 second
					GPIO_ResetBits(GPIOA, GPIO_Pin_1); // yellow led off for cars
				}
					
			break;
			
			default:
				timerCounter = 0;
				TIM_SetCounter(TIM2, 0);  // make sure TIM2 Counter start from zero before begin
				state = 1;
			break;

				
		}
		
	}

}
















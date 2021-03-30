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
#include "string.h" // for string operations
#include "stdbool.h" // for boolean usage

GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

int state;
uint16_t redPulse; // Pulse value variable for red LED
uint16_t yellowPulse; // Pulse value variable for yellow LED
uint16_t greenPulse; // Pulse value variable for geen LED
	
void redConfig(char pulseValue[])
{
	
	if (strcmp(pulseValue, "LOW") == 0) 
	{
		redPulse = 1000; // when red is low others will be off
		yellowPulse = 0;
		greenPulse = 0;
		TIM_Cmd(TIM2, ENABLE); 
  
	} 
	else if (strcmp(pulseValue, "MEDIUM") == 0)
	{
		redPulse = 6000; // when red is medium others will be off
		yellowPulse = 0;
		greenPulse = 0;
		TIM_Cmd(TIM2, ENABLE);
 
	}

	else if (strcmp(pulseValue, "HIGH") == 0)
	{
		redPulse = 36000; // when red is high others will be off
		yellowPulse = 0;
		greenPulse = 0;
		TIM_Cmd(TIM2, ENABLE);
		
	}
	else
	{
		TIM_Cmd(TIM2, DISABLE); // Off state for all LEDs
	}
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = redPulse;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = yellowPulse;  
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = greenPulse;  
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
}

void yellowConfig(char pulseValue[])
{
	if (strcmp(pulseValue, "LOW") == 0) 
	{
		redPulse = 0; // when yellow is low others will be off
		yellowPulse = 1000;
		greenPulse = 0;
		TIM_Cmd(TIM2, ENABLE); 
  
	} 
	else if (strcmp(pulseValue, "MEDIUM") == 0)
	{
		redPulse = 0; // when yellow is medium others will be off
		yellowPulse = 6000;
		greenPulse = 0;
		TIM_Cmd(TIM2, ENABLE);
 
	}

	else if (strcmp(pulseValue, "HIGH") == 0)
	{
		redPulse = 0; // when yellow is high others will be off
		yellowPulse = 36000;
		greenPulse = 0;
		TIM_Cmd(TIM2, ENABLE);
		
	}
	else
	{
		TIM_Cmd(TIM2, DISABLE); // Off state for all LEDs
	}
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = redPulse;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = yellowPulse;  
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = greenPulse;  
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
}

void greenConfig(char pulseValue[])
{
	if (strcmp(pulseValue, "LOW") == 0) 
	{
		redPulse = 0; // when green is low others will be off
		yellowPulse = 0;
		greenPulse = 1000;
		TIM_Cmd(TIM2, ENABLE); 
  
	} 
	else if (strcmp(pulseValue, "MEDIUM") == 0)
	{
		redPulse = 0; // when green is medium others will be off
		yellowPulse = 0;
		greenPulse = 6000;
		TIM_Cmd(TIM2, ENABLE);
 
	}

	else if (strcmp(pulseValue, "HIGH") == 0)
	{
		redPulse = 0; // when yellow is high others will be off
		yellowPulse = 0;
		greenPulse = 36000;
		TIM_Cmd(TIM2, ENABLE);
		
	}
	else
	{
		TIM_Cmd(TIM2, DISABLE); // Off state for all LEDs
	}
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = redPulse;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = yellowPulse;  
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = greenPulse;  
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
}

// Simple delay function with the help of timers.
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // pin 0 as afio push-pull
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // pin 1 as afio push-pull 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // pin 2 as afio push-pull 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // pin 3 as pull-down input
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // pin 4 as pull-down input
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 35999;
  TIM_TimeBaseStructure.TIM_Prescaler = 19;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	// my delay function works with TIM3
	TIM_TimeBaseStructure.TIM_Period = 3999; // 2 seconds
	TIM_TimeBaseStructure.TIM_Prescaler = 35999;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	// tim2 for pwm
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// tim3 for delay
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	

	
	while(1)
	{
		switch(state)
		{
			case 1:
				delay_ms(450);
				redConfig("LOW");
				if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
				{
					state = 4;
				}
				else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))
				{
					state = 2;
				}
			break;
				
			case 2:
				delay_ms(450);
				redConfig("MEDIUM");
				if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
				{
					state = 4;
				}
				else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))
				{
					state = 3;
				}
			break;
				
			case 3:
				delay_ms(450);
				redConfig("HIGH");
				if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
				{
					state = 4;
				}
				else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))
				{
					state = 1;
				}
			break;
							
			case 4:
				delay_ms(450);
				yellowConfig("LOW");
				if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
				{
					state = 7;
				}
				else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))
				{
					state = 5;
				}
			break;
				
			case 5:
				delay_ms(450);
				yellowConfig("MEDIUM");
				if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
				{
					state = 7;
				}
				else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))
				{
					state = 6;
				}
			break;
				
			case 6:
				delay_ms(450);
				yellowConfig("HIGH");
				if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
				{
					state = 7;
				}
				else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))
				{
					state = 4;
				}
			break;
							
			case 7:
				delay_ms(450);
				greenConfig("LOW");
				if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
				{
					state = 10;
				}
				else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))
				{
					state = 8;
				}
			break;
			
			case 8:
				delay_ms(450);
				greenConfig("MEDIUM");
				if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
				{
					state = 10;
				}
				else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))
				{
					state = 9;
				}
			break;
				
			case 9:
				delay_ms(450);
				greenConfig("HIGH");
				if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
				{
					state = 10;
				}
				else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))
				{
					state = 7;
				}
			break;
				
			default:
				delay_ms(450);
				TIM_Cmd(TIM2, DISABLE); // all LEDs off 
				if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
				{
					state = 1;
				}
			break;				
		}	
	}
}
















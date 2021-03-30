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
#include "stm32f10x_adc.h"
#include "string.h" // for string operations
#include "stdbool.h" // for boolean usage
#include "stdio.h"

GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
ADC_InitTypeDef ADC_InitStructure;
USART_InitTypeDef USART_InitStructure;

int counter = 0; // main counter ranges between 0 and 30.
int iteration_counter; // potentiometer value for speed.

static double data=0;
char adc[20];
int Sent_data=0;

int value1; // for checking green LED counter
int value2; // for checking yellow LED counter
int value3; // for checking red LED counter

unsigned long int led_speed; // Speed value changes with potentiometer
uint32_t redPulse; // Pulse value variable for red LED
uint32_t yellowPulse; // Pulse value variable for yellow LED
uint32_t greenPulse; // Pulse value variable for geen LED
  

// Simple delay function with the help of TIM3.
void delay_ms(uint16_t ms)
{
  TIM_SetCounter(TIM3, 0);    // make sure TIM3 Counter start from zero
  TIM_Cmd(TIM3, ENABLE);      
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
  while(TIM_GetCounter(TIM3) < ms);   // it is not exactly milliseconds, 1 second corresponds to 2000 ms.
  TIM_Cmd(TIM3, DISABLE);
  TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
}

void UART_Transmit(char *string)
{
  while(*string)
  {
    while(!(USART1->SR & 0x00000040));
    USART_SendData(USART1,*string);
    *string++;
  }
}
  

int main (void)
{
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // enabling clock for port a.
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // enabling clock for afio port.
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // enabling clock for adc1.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // enabling  clock for timer2.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // enabling  clock for timer3.
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // enabling clock for usart1.
  
  // Configue UART RX
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  // Configue UART TX
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
  // USART settings
  USART_InitStructure.USART_BaudRate = 9600; // as stated in midterm
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_Init(USART1, &USART_InitStructure);
  // Enabledatareceive interrupt & USART1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART1, ENABLE);
  
  // Configure input PA0 (Analog Input)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // pin 0 as potentiometer voltage input.
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  // Configure ADC mode(Independent | RegSimult | FastInterl | etc.)
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  // Enable/disable external conversion trigger (EXTI | TIM | etc.)
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  // Configure data alignment (Right | Left)
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  // Set the number of channels to be used and initialize ADC
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5);
  ADC_Cmd(ADC1, ENABLE);
  
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // red LED
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // pin 1 as afio push-pull
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // yellow LED
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // pin 2 as afio push-pull 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // green LED
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // pin 3 as afio push-pull 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // pin 4 as pull-down input
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // pin 5 as pull-down input
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //tim2 for pwm
  TIM_TimeBaseStructure.TIM_Period = 36000; // it is for PWM signal 
  TIM_TimeBaseStructure.TIM_Prescaler = 19;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
  
  // tim3 for delay
  TIM_TimeBaseStructure.TIM_Period = 3999; // 2 seconds
  TIM_TimeBaseStructure.TIM_Prescaler = 35999; // delay function prescaler values
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; // my delay function works with TIM3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; // it is for receive USART interrupt.
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  

  
  while(1)
  { 
    ADC_GetConversionValue(ADC1);
    data = ADC_GetConversionValue(ADC1); //Data read from ADC (potentiometer)
    Sent_data=USART_ReceiveData(USART1); //Data sent from PC terminal
     sprintf(adc,"%.2f\r",(data/40.95)); // ADC changes from 0 to 4095, so we need to convert to between 0 and 100.
     UART_Transmit(adc);
    //delay_ms(2000); // if it used for plotting purposes, having a delay may not slow the data flow. Due to the reason that my computer is weak, I can not
    // plot the date for 0 seconds delay. 
    

  
    adc_refresh:
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    iteration_counter = ADC_GetConversionValue(ADC1);
    iteration_counter = (30*iteration_counter/4080); // here I am using adc convertion value as counter ranges between 0 and 30.
    // It is divided into 4080 instead of 4095 due to the fact that contact between breadboard and potentiometer it can not exceed 4082.
    if(iteration_counter < 10)
    {
      value1 = iteration_counter; // value1 is for green LED thus it is same as iteration value if below 10.
    }
    else if(iteration_counter >= 10 && iteration_counter < 20)
    {
      value1 = 10; // if potentiometer value gives between 10 and 20, greed LED fulls with 100% as value1 is 10.
      value2 = iteration_counter; // value2 is for yellow LED, same logic as in green LED value1.
    }
    else if(iteration_counter >= 20 && iteration_counter <= 30)
    {
      value1 = 10; // with same logic, if potentiometer value is between 20 and 30, green fulls 100%
      value2 = 20; // then yellow fulls 100%
      value3 = iteration_counter; // same logic.
    }
    
    // green LED increase
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) && counter < value1 && counter < 10)
    {
      // it has been said that it is free to use GPIO read functions, I used delay divider in for loop.
      for(int i = 0; i < 50; i++)
      { 
        // every time the button released, it goes directly to the adc refresh to change value and check that if button pressed or not.
        if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))
        {
          goto adc_refresh; 
        }
        delay_ms(10); // this value corresponds to 0.02 seconds as 2000ms value is 1 second for my timer.
      }
        
    counter++; // counter adds 1 every time pulse increase with 10%.
    greenPulse += 3600; // every time its PWM increase as 10% because (3600/36000)x100 = 10.
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = greenPulse; 
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);
        
    TIM_Cmd(TIM2, ENABLE);
    }
    
    // green LED decrease
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) && counter <= 10 && counter > 0)
    {
    
      for(int i = 0; i < 50; i++)
      {         
        if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5))
        {
          goto adc_refresh;
        }
        delay_ms(10);
      }
  
    counter--;    
    greenPulse -= 3600; // same increase logic also applies to decrease. As counter decrease PWM signal decrease by 10%.
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = greenPulse;
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);
        
    TIM_Cmd(TIM2, ENABLE);
    }   
    
    // yellow LED increase
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) && counter <= value2 && counter >= 10 && counter < 20)
    {
    
      for(int i = 0; i < 50; i++)
      {         
        if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))
        {
          goto adc_refresh;
        }
        delay_ms(10);
      }
        
    counter++;    
    yellowPulse += 3600;
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = yellowPulse;
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
        
    TIM_Cmd(TIM2, ENABLE);
    }
    
    // yellow LED decrease
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) &&  counter > 10 && counter <= 20)
    {
    
      for(int i = 0; i < 50; i++)
      {         
        if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5))
        {
          goto adc_refresh;
        }
        delay_ms(10);
      }
        
    counter--;    
    yellowPulse -= 3600;
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = yellowPulse;
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
        
    TIM_Cmd(TIM2, ENABLE);
    }   
    
    // red LED increase
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) && counter <= value3 && counter >= 20 && counter < 30)
    {
    
      for(int i = 0; i < 50; i++)
      {         
        if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))
        {
          goto adc_refresh;
        }
        delay_ms(10);
      }
      if(redPulse == 36000)
      {
        redPulse -= 3600;
      }   
    counter++;    
    redPulse += 3600;
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = redPulse;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
        
    TIM_Cmd(TIM2, ENABLE);
    }
    
    // red LED decrease
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) && counter > 20 && counter <= 30)
    {
    
      for(int i = 0; i < 50; i++)
      {         
        if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5))
        {
          goto adc_refresh;
        }
        delay_ms(10);
      }
        
    counter--;    
    redPulse -= 3600;
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = redPulse;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
        
    TIM_Cmd(TIM2, ENABLE);
    }         
  }
}



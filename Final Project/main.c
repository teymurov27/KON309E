/*
              Istanbul Technical University
           Faculty of Electical and Electronics
    Department of Controlling and Automation Engineering 
  
          SAMIR TEYMUROV           ID: 040190922
*/
  
  

/* I hereby declare that code below and all of its components are written by me only.
If you have seen any part of my code in some other student please contact me directly.
*/


#define LM75A_ADDR				 0x90
#define LM75A_REG_TEMP		 0x00
#define LM75A_REG_CONF     0x01
#define LM75A_REG_THYS     0x02
#define LM75A_REG_TOS      0x03


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

static double sensor_temp; // sensor temperature
char adc[20];
int Sent_data=0;
double referance_temp; // adc value from potentiometer

float readTemperature(void)
{
	uint8_t LM75A_DataBuffer[2] = {0,0};
	uint16_t temperature;

	// Wait if busy
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));		
	// Enable ACK
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
	// Generate start condition
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB)); 

	// Send device address for write
	I2C_Send7bitAddress(I2C1, LM75A_ADDR, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) ;

	// Write to device (temperature request)
	I2C_SendData(I2C1, LM75A_REG_TEMP);  
  while ((!I2C_GetFlagStatus(I2C1,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(I2C1,I2C_FLAG_BTF)));

	// Generate start condition again (Re-start)
	I2C_GenerateSTART(I2C1, ENABLE);   
  while (!I2C_GetFlagStatus(I2C1,I2C_FLAG_SB));

	// Send device address for read this time
	I2C_Send7bitAddress(I2C1, LM75A_ADDR, I2C_Direction_Receiver);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));
	LM75A_DataBuffer[0] = I2C_ReceiveData(I2C1);
	
	// No ACK by master & Stop condition
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_GenerateSTOP(I2C1, ENABLE); 
	
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));
	LM75A_DataBuffer[1] = I2C_ReceiveData(I2C1);
	 
	// Calculate temp data
	temperature = (uint16_t)((LM75A_DataBuffer[0] << 8) | (LM75A_DataBuffer[1]));
	
	return ((float)(temperature >> 7) * 0.5);
}

// Simple delay function with the help of TIM3.
void delay_ms(uint16_t ms)
{
  TIM_SetCounter(TIM3, 0);    // make sure TIM3 Counter start from zero
  TIM_Cmd(TIM3, ENABLE);      
  while(TIM_GetCounter(TIM3) < ms);   // it is not exactly milliseconds, 1 second corresponds to 2000 ms.
  TIM_Cmd(TIM3, DISABLE);
}

void UART_Transmit(char *string)
{
	while(*string)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_SendData(USART1,*string++);
	}
}

void pwm_state(char system_state[]) // Basic ON/OFF control system
{

  if (strcmp(system_state, "ON") == 0) // due to the fact that system has neative gain when pulse is 0 system is ON
	 {
		
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0; 
		TIM_OC2Init(TIM2, &TIM_OCInitStructure);
		 
		TIM_Cmd(TIM2, ENABLE);
	 }
	else if (strcmp(system_state, "OFF") == 0) // due to the fact that system has neative gain when pulse is 35000 system is OFF
	 {
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 35000;
		TIM_OC2Init(TIM2, &TIM_OCInitStructure);
		 
		TIM_Cmd(TIM2, ENABLE);

	 }
}

  

int main (void)
{
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // enabling clock for port a.
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // enabling clock for port b.
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // enabling clock for afio port.
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // enabling clock for adc1.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // enabling  clock for timer2.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // enabling  clock for timer3.
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // enabling clock for usart1.
  
	I2C_InitTypeDef I2C_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 100000;
  I2C_Init(I2C1, &I2C_InitStructure);

  I2C_Cmd(I2C1, ENABLE);
	
	// Configure pins (SDA->PB7, SCL->PB6)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
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
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // pin 0 as potentiometer voltage input for referance_temp
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
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // pwm signal
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // yellow LED
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // green LED
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // blue LED on RGB
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // red LED on RGB
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // green LED on RGB
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // red LED
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
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
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    referance_temp = ADC_GetConversionValue(ADC1); // ADC value changes between 0 and 4095, so getting 60 degree it has to be  divided to 68.2
		referance_temp = referance_temp/68.2; // referance temperature changes between 22 and 60. 60 is my system max for sensor
		
		sensor_temp = readTemperature(); // sensor value
		sprintf(adc,"%.1f\r",sensor_temp); 
		UART_Transmit(adc);
		delay_ms(500); // 0.25 ms delay, because of the high data flow
		
		if(sensor_temp > referance_temp)
		{
			pwm_state("OFF"); // if sensor value exceeds referance temperature, it is OFF
		}
		else if(sensor_temp < referance_temp)
		{
			pwm_state("ON"); // if sensor value below the referance temperature, it is ON
		}	
		
		if(referance_temp < 30) // low temperature: below 30
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_3); // green LED
			GPIO_ResetBits(GPIOA, GPIO_Pin_7); //
			GPIO_ResetBits(GPIOA, GPIO_Pin_2);

		}
		else if(referance_temp >=30 && referance_temp < 45) // medium temperature between 30 and 45
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_2); // yellow LED
			GPIO_ResetBits(GPIOA, GPIO_Pin_7);
			GPIO_ResetBits(GPIOA, GPIO_Pin_3);
		}
		else // high temperature between 45 and 60
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_7); // red LED
			GPIO_ResetBits(GPIOA, GPIO_Pin_3);
			GPIO_ResetBits(GPIOA, GPIO_Pin_2);
		}
		
		if (((sensor_temp-referance_temp) > 0) && ((sensor_temp-referance_temp) > (0.1*referance_temp))) // 10% overshoot
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_6); // green LED
			GPIO_ResetBits(GPIOA, GPIO_Pin_4);
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		}
		else if (((sensor_temp-referance_temp) > 0) && ((sensor_temp-referance_temp) < (0.1*referance_temp)) && ((sensor_temp-referance_temp) > (0.02*referance_temp)))
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_4); // blue LED. I did not have any LEDs left thats why i used RGB LED
			GPIO_ResetBits(GPIOA, GPIO_Pin_6);
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		}
		else
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_5); // red LED
			GPIO_ResetBits(GPIOA, GPIO_Pin_6);
			GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		}  		
  }
}

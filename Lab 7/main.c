/*
              Istanbul Technical University
           Faculty of Electical and Electronics
    Department of Controlling and Automation Engineering 
  
          SAMIR TEYMUROV           ID: 040190922
*/
  
  

#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "string.h" // for string operations
#include "stdbool.h" // for boolean usage
#include "stdio.h"

#if _I2C_PORT == 1
  #define I2C_PORT         I2C1
  #define I2C_SCL_PIN      GPIO_Pin_6     // PB6
  #define I2C_SDA_PIN      GPIO_Pin_7     // PB7
  #define I2C_GPIO_PORT    GPIOB
  #define I2C_CLOCK        RCC_APB1Periph_I2C1
#elif _I2C_PORT == 2
  #define I2C_PORT         I2C2
  #define I2C_SCL_PIN      GPIO_Pin_10    // PB10
  #define I2C_SDA_PIN      GPIO_Pin_11    // PB11
  #define I2C_GPIO_PORT    GPIOB
  #define I2C_CLOCK        RCC_APB1Periph_I2C2
#endif

/* LM75 defines */
#define LM75_ADDR                     0x90 // LM75 address

/* LM75 registers */
#define LM75_REG_TEMP                 0x00 // Temperature
#define LM75_REG_CONF                 0x01 // Configuration
#define LM75_REG_THYS                 0x02 // Hysteresis
#define LM75_REG_TOS                  0x03 // Overtemperature shutdown

GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
ADC_InitTypeDef ADC_InitStructure;
USART_InitTypeDef USART_InitStructure;
I2C_InitTypeDef I2C_InitStructure;

int counter = 0; // main counter ranges between 0 and 30.
int iteration_counter; // potentiometer value for speed.

static double temp_data;

char adc[20];
int Sent_data=0;

uint8_t LM75_Init(uint32_t SPI_Clock_Speed);

void LM75_WriteReg(uint8_t reg, uint16_t value);
uint16_t LM75_ReadReg(uint8_t reg);
uint8_t LM75_ReadConf(void);
void LM75_WriteConf(uint8_t value);

void LM75_Shutdown(FunctionalState newstate);
int16_t LM75_Temperature(void);

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

// Read 16-bit LM75 register
uint16_t LM75_ReadReg(uint8_t reg) {
  uint16_t value;

  I2C_AcknowledgeConfig(I2C1,ENABLE); // Enable I2C acknowledgment
  I2C_GenerateSTART(I2C1,ENABLE);
  while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); // Wait for EV5
  I2C_Send7bitAddress(I2C1,LM75_ADDR,I2C_Direction_Transmitter); // Send slave address
  while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); // Wait for EV6
  I2C_SendData(I2C1,reg); // Send register address
  while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); // Wait for EV8
  I2C_GenerateSTART(I2C1,ENABLE); // Send repeated START condition (aka Re-START)
  while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); // Wait for EV5
  I2C_Send7bitAddress(I2C1,LM75_ADDR,I2C_Direction_Receiver); // Send slave address for READ
  while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); // Wait for EV6
  while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)); // Wait for EV7 (Byte received from slave)
  value = (I2C_ReceiveData(I2C1) << 8); // Receive high byte
  I2C_AcknowledgeConfig(I2C1,DISABLE); // Disable I2C acknowledgment
  I2C_GenerateSTOP(I2C1,ENABLE); // Send STOP condition
  while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)); // Wait for EV7 (Byte received from slave)
  value |= I2C_ReceiveData(I2C1); // Receive low byte

  return value;
}

int16_t LM75_Temperature(void) {
  uint16_t raw;
  int16_t temp;

  raw = LM75_ReadReg(LM75_REG_TEMP) >> 7;
  if (raw & 0x0100) {
    // Negative temperature
    temp = -10 * (((~(uint8_t)(raw & 0xFE) + 1) & 0x7F) >> 1) - (raw & 0x01) * 5;
  } else {
    // Positive temperature
    temp = ((raw & 0xFE) >> 1) * 10 + (raw & 0x01) * 5;
  }

  return temp;
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
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE); // enabling clocl for i2c1.
  
  // Configure pins (SDA, SCL)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  // I2Cconfiguration
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 100000;
  I2C_Init(I2C1, &I2C_InitStructure);
  I2C_Cmd(I2C1, ENABLE);
  
  // Configue UART RX
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  // Configue UART TX
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
//  USART_ClockInitTypeDef USART_ClockInitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_Init(USART1, &USART_InitStructure);
  
//  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART1, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // red LED
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // pin 1 as afio push-pull
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  // tim3 for delay
  TIM_TimeBaseStructure.TIM_Period = 3999; // 2 seconds
  TIM_TimeBaseStructure.TIM_Prescaler = 35999; // delay function prescaler values
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
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
      
      temp_data = LM75_ReadReg(0x00);
      
      Sent_data=USART_ReceiveData(USART1); //Data sent from PC terminal
      if(Sent_data==49 && temp_data > 50) // ASCII 49 = 1
        
        GPIO_SetBits(GPIOA,GPIO_Pin_1); 
      if(Sent_data==48 && temp_data < 50) // ASCII 48 = 0
        GPIO_ResetBits(GPIOA,GPIO_Pin_1);
      
      sprintf(adc,"%.1f\r",temp_data);
      UART_Transmit(adc);
      delay_ms(2000);   

        
   
  }
}


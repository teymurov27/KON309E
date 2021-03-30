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
#include "stdbool.h"
#include "stdio.h"
#include "delay.h"

	
GPIO_InitTypeDef GPIO_InitStruct;



int main (void)
{
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // Enable clock for PortA
 
// Configure PA0 as open-drain output
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz; 
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD; // CNF:MODE (Output|Open Drain|2MHz)
GPIO_Init(GPIOA, &GPIO_InitStruct);
	
// Configure PA1 as input with internal pull-up resistor
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // CNF:MODE (Input|Pull-Up|2MHz)
GPIO_Init(GPIOA, &GPIO_InitStruct);
	
// Configure PA2 as input with internal pull-up resistor
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // CNF:MODE (Input|Pull-Up|2MHz)
GPIO_Init(GPIOA, &GPIO_InitStruct);
	


delayInit();

while (1)
{

	
	
        if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))// If on PortA of GPIO pin reads input which literally means if the button on pin 1 pressed.
        {
					
					bool button = true;		
					
					
					while(button){ // Here I wrote blink in while(button) which means this loop always works when button 1 not pressed after turning on LED first time.
					   loop_first:
           GPIO_ResetBits(GPIOA, GPIO_Pin_0);
				 	 delayMs(1000); // Delay in milliseconds
					if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 1){
						button = false;
					}
					 GPIO_SetBits(GPIOA, GPIO_Pin_0);
					 delayMs(1000); // Delay in milliseconds
					if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 1){
						goto loop_end; // If button on PA1 pressed it goes loop_end which break out of loop, eventually LED Turns Off.
					}
					if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 1){
						goto loop_second; // If button on PA2 pressed it goes to loop_second which reduces delay to 500 milliseconds 
					}
					}
					
					
					while(button){
				          loop_second:
					 GPIO_ResetBits(GPIOA, GPIO_Pin_0);
					 delayMs(500);
					if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 1){
						button = false;
					}
					 GPIO_SetBits(GPIOA, GPIO_Pin_0);
					 delayMs(500);
					if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 1){
						goto loop_end;
					}
					if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 1){
						goto loop_first; // If button on PA2 pressed it goes to loop_first which reduces delay to 500 milliseconds 
					}
					
					}
					
					
					  loop_end:
				        GPIO_SetBits(GPIOA, GPIO_Pin_0); // Turn off LED on PA0
					break;
					
					  
             							
        }
				
				
				
				else
				{
           GPIO_SetBits(GPIOA, GPIO_Pin_0); // Turn off LED on PA0
				}
				    			

}

}



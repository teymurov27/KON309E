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

// tim3 for delay
  TIM_TimeBaseStructure.TIM_Period = 3999; // 2 seconds
  TIM_TimeBaseStructure.TIM_Prescaler = 35999; // delay function prescaler values
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

// setup SPI2 pins PB12/SS, PB13/SCK, PB14/MISO, PB15/MOSI
void PeripheralInit_SPI2_Slave(){
GPIO_InitTypeDef GPIO_InitDef;
SPI_InitTypeDef SPI_InitDef;
// initialize init structs
GPIO_StructInit(&GPIO_InitDef);
SPI_StructInit(&SPI_InitDef);
// initialize clocks
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
// initialize PB12/SS alternate function open-drain (10 MHz)
GPIO_InitDef.GPIO_Pin = GPIO_Pin_12;
GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_OD;
GPIO_InitDef.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_Init(GPIOB, &GPIO_InitDef);
// initialize PB13/SCK alternate function open-drain (10 MHz)
GPIO_InitDef.GPIO_Pin = GPIO_Pin_13;
GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_OD;
GPIO_InitDef.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_Init(GPIOB, &GPIO_InitDef);
// initialize PB14/MISO alternate function push-pull (10 MHz)
GPIO_InitDef.GPIO_Pin = GPIO_Pin_14;
GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_InitDef.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_Init(GPIOB, &GPIO_InitDef);
// initialize PB15/MOSI alternate function open-drain (10 MHz)
GPIO_InitDef.GPIO_Pin = GPIO_Pin_15;
GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_OD;
GPIO_InitDef.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_Init(GPIOB, &GPIO_InitDef);
// initialize SPI slave
// for slave, no need to define SPI_BaudRatePrescaler
SPI_InitDef.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
SPI_InitDef.SPI_Mode = SPI_Mode_Slave;
SPI_InitDef.SPI_DataSize = SPI_DataSize_8b; // 8-bit transactionsSPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB; // MSB first
SPI_InitDef.SPI_CPOL = SPI_CPOL_Low; // CPOL = 0, clock idle low
SPI_InitDef.SPI_CPHA = SPI_CPHA_2Edge; // CPHA = 1
SPI_InitDef.SPI_NSS = SPI_NSS_Hard; // use hardware SS
SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; // APB1 36/32
= 1.125 MHz
// SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; // APB1
36/128 = 0.28 MHz
// SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // APB1
36/8 = 4.5 MHz
SPI_InitDef.SPI_CRCPolynomial = 7;
SPI_Init(SPI2, &SPI_InitDef);
SPI_Cmd(SPI2, ENABLE);
}
// Here is a function to transfer a byte over SPI_CR2:
// transfer a byte over SPI2 PB12/SS, PB13/SCK, PB14/MISO, PB15/MOSI
uint8_t transfer_8b_SPI2_Slave(uint8_t outByte){
while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE));
SPI_I2S_SendData(SPI2, outByte);
// send
while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE));
return SPI_I2S_ReceiveData(SPI2);
// read recieived
}

#include "includes.h"

void ADC1_Test(void);
void MosTest(void);
void I2CSoftwareTest(void);
void ClearProtocol(void);

unsigned char temp_readbyte, readbyte,sucess;

unsigned char i_testloop = 0, j_testloop = 0;
unsigned char StartTimes = 0;

int main(void)
{
	SysTick_Init();	
	GPIO_Config();//
	SoftwareI2CConfig();
	SoftwareI2CGPIOConfig(I2CValue1, I2CValue2);
	USART2_Config(); 
	ADC1_Config();
	//CAN_Config();
	TIM2_NVIC_Configuration();
	TIM2_Configuration();
	TIM3_NVIC_Configuration();
	TIM3_Configuration();
	printf("\r\n get started \r\n");
	IWDG_Configuration();
	IWDG_Enable();
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{ 
		printf("WatchDog Reset\r\n"); 
		RCC_ClearFlag();//清除标志位
	}
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)) 
		{
			IWDG_ReloadCounter();
			Delay_us(1000);
		}
	}
	
}

void ADC1_Test(void)
{
		for(i=0;i<11;i++)
		{
			ADC_ConvertedValueLocal = (float)ADC_ConvertedValue[i]/4096*2.5; // 读取转换的AD
			Delay_us(100);                             // 延时 
			//USART_printf( USART2,"0x%04X ", ADC_ConvertedValue[i]);  
			USART_printf( USART2,"-->%fV", ADC_ConvertedValueLocal); ;
		}
		USART_printf( USART2,"\r \n");
}

void MosTest(void)
{	
	GPIO_SetBits(GPIOA, GPIO_Pin_All & (~(GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 \
																				| GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_13 | GPIO_Pin_14)));	
	GPIO_SetBits(GPIOB, GPIO_Pin_All & ( ~ (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7) ) );
	GPIO_SetBits(GPIOC, GPIO_Pin_All & ( ~ (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5) )); 
	GPIO_SetBits(GPIOD, GPIO_Pin_All & ( ~ (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3) ) );
	GPIO_SetBits(GPIOE, GPIO_Pin_All & ( ~ (GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15 ) ) );	
	
	Delay_us(500);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_All & (~(GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 \
																				| GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_13 | GPIO_Pin_14)));	
	GPIO_ResetBits(GPIOB, GPIO_Pin_All & ( ~ (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7) ) );
	GPIO_ResetBits(GPIOC, GPIO_Pin_All & ( ~ (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5) )); 
	GPIO_ResetBits(GPIOD, GPIO_Pin_All & ( ~ (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3) ) );
	GPIO_ResetBits(GPIOE, GPIO_Pin_All & ( ~ (GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15 ) ) );
	
	Delay_us(1000);
}

void I2CSoftwareTest(void)
{
		temp_readbyte = readbyte;
		sucess = I2C_PCF8574_BufferRead( &readbyte, 0x40 );
		
		Delay_us(100);
		
		if (temp_readbyte != readbyte)
		{	
			sucess = I2C_PCF8574_BufferWrite( 0x00, 0x42 );
			Delay_us(500);		
			sucess = I2C_PCF8574_BufferWrite( 0xFF, 0X42 );
			Delay_us(500);
		}
}

/*
void I2C_Test(void)
{
	uint8_t I2CInput;
	uint8_t Write_Buffer = 0xFF;
	uint8_t PCF8574Address = 0x40;
	I2C_PCF8574_Init();
	I2C_PCF8574_ByteWrite(Write_Buffer , PCF8574Address);
	I2CInput = I2C_PCF8574_ByteRead(PCF8574Address);
	USART_printf( USART2,"%d", I2CInput);
}
*/			

void ClearProtocol(void)
{
	unsigned char i;
	gU2RecvBuff.header = 0;
	gU2RecvBuff.command = 0;
	for(i=0; i<4; i++) gU2RecvBuff.data[i] = 0;
	gU2RecvBuff.checksum = 0;
}

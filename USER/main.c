#include "includes.h"

void ADC1_Test(void);
void MosTest(void);
void I2CSoftwareTest(void);

unsigned char temp_readbyte, readbyte,sucess;
	
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
	
	printf("\r\n串口测试\r\n\r\n");
	
// 	if ( I2C_PCF8574_BufferRead( &IDOfPCB, 0x40 ) ) 	
// 	{
// 		IDOfPCB &= 0x0F;
// 		printf("%d" , IDOfPCB) ;
// 	}
// 	else printf("an error occurred while reading IDOfPCB");
// 	
// 	if ( I2C_PCF8574_BufferRead( &RunMode, 0x40 ) ) 	
// 	{
// 		RunMode = ( RunMode & 0xF0 ) >> 4;
// 		printf("%d" , RunMode) ;
// 	}
// 	else printf("an error occurred while reading RunMode"); 	
	
	PtoEtcSWCheckResult = ELSCheck();
	
	while (1)
	{
		if (gU2RecvBuff.protocol_ok)
		{
			gU2RecvBuff.protocol_ok = 0;
			MotorStopAll();
			motor_row =( (gU2RecvBuff.data[0]<<8) | gU2RecvBuff.data[1] );
			motor_col =( (gU2RecvBuff.data[2]<<8) | gU2RecvBuff.data[3] );			
			for(i=0;i<11;i++)
			{
				if (( (motor_row >>i) & 0x01 ) == 0x01) 
				{
					for (j = 0; j<11;j++)
					{
						if (( (motor_col >>j) & 0x01 ) == 0x01) MotorDrive( 4-gU2RecvBuff.command, i+1, j+1);
					}
				}
			}
			Delay_us(500);
		}
		
		//check photoelectric switch and current
		if (GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13) == 1) 
		{
			MotorStopAll();
			printf("Medicine_ok");
		}
		for (j = 0; j<3; j++)
		{
			MeanRunningCurrent += ADC_ConvertedValue[0];
			Delay_us(1);
		}
		if (MeanRunningCurrent > 3234) //if anything wrong during running(current feedback), stop all the motors;
		{
			MotorStopAll();
			printf("Medicine_wrong");
		}
		MeanRunningCurrent = 0;
	}
}

void ADC1_Test(void)
{
		for(i=0;i<11;i++)
		{
			ADC_ConvertedValueLocal = (float)ADC_ConvertedValue[i]/4096*2.5; // 读取转换的AD
			Delay_us(100);                             // 延时 
			//printf("0x%04X ", ADC_ConvertedValue[i]);  
			printf("-->%fV", ADC_ConvertedValueLocal); ;
		}
		printf("\r \n");
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

// void I2C_Test(void)
// {
// 	uint8_t I2CInput;
// 	uint8_t Write_Buffer = 0xFF;
// 	uint8_t PCF8574Address = 0x40;
// 	I2C_PCF8574_Init();
// 	I2C_PCF8574_ByteWrite(Write_Buffer , PCF8574Address);
// 	I2CInput = I2C_PCF8574_ByteRead(PCF8574Address);
// 	printf("%d", I2CInput);
// }

// 			TIM_SetCompare1(TIM3,TIM_GetCapture1(TIM3)+100);
// 			if(TIM_GetCapture1(TIM3)==900)TIM_SetCompare1(TIM3,600);	
			
			//for(i=0;i<=399;i++)
			//{
				//CCR1_Val = 600 + i;
				//TIM3_PWM_Init ();
				//for(j=0;j<10000;j++);
			//}
			//for(i=0;i<=399;i++)
			//{
				//CCR1_Val = 999 - i;
				//TIM3_PWM_Init ();
				//for(j=0;j<10000;j++);
			//}		

			//printf("\r\n this is a printf demo \r\n");
			//USART1_printf(USART1, "\r\n This is a USART1_printf demo \r\n");
			//USART1_printf(USART1, "\r\n ("__DATE__ " - " __TIME__ ") \r\n");
			

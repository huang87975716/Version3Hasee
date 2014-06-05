#include "includes.h"

void ADC1_Test(void);
void MosTest(void);
void I2CSoftwareTest(void);
void EchoToMaster(unsigned char *ptr);
void ClearProtocol(void);

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
	TIM2_NVIC_Configuration();e
	TIM2_Configuration();
	
	printf("\r\n串口测试\r\n\r\n");
	
	if ( I2C_PCF8574_BufferRead( &IDOfPCB, 0x40 ) ) 	
	{
		IDOfPCB &= 0x0F;
		printf("IDOfPCB is %d \r \n" , IDOfPCB) ;
	}
	else printf("an error occurred while reading IDOfPCB\r \n");
	
	if ( I2C_PCF8574_BufferRead( &RunMode, 0x40 ) ) 	
	{
		RunMode = ( RunMode & 0xF0 ) >> 4;
		printf("RunMode is %d \r \n" , RunMode) ;
	}
	else printf("an error occurred while reading RunMode\r \n"); 	
		
	while (1)
	{
		if(keypushed)
		{
			start motor to drive the door down;
			DownLimitSWFlag = 1;
		}
		if(DownLimitSWFlag == 1)
		{
			DownLimitSWFlag = 0;
			check down limitSW, if 1 then DownLimitSWFlag = 0 and also InfraredCheckFlag = 1;
		}
		if(InfraredCheckFlag == 1)
		{
			check infrared sensor
			if 1
			start timerofinfrard like 10s
		}
		if(timer2 up)
		{
		start motor to drive the motor down;
		CheckLimitSW Flag ==1
		}
		
		if (gU2RecvBuff.protocol_ok)
		{
			gU2RecvBuff.protocol_ok = 0;
			EchoToMaster(&HandShakeToMaster[0]);//Echo to master to claim that slave have received the infomatino correctly;
			
			
			switch (gU2RecvBuff.command)
			{	
				case MotorForward:
					MotorStopAll();
					motor_row =( (gU2RecvBuff.data[0]<<8) | gU2RecvBuff.data[1] );
					motor_col =( (gU2RecvBuff.data[2]<<8) | gU2RecvBuff.data[3] );			
					for(i=0;i<11;i++)
					{
						if (( (motor_row >>i) & 0x01 ) == 0x01) 
						{
							for (j = 0; j<11;j++)
							{
								if (( (motor_col >>j) & 0x01 ) == 0x01) 
									MotorDrive( 1, i+1, j+1);//should tell master maybe the motor could not start																													
							}
						}
					}
					START_TIME2;//Start timer 2 for PtoEtcSWCheck WaitPtoEtcSW period
					WaitPtoEtcSW = 1;
					Delay_us(500);
					break;
				case MotorBackward:
					MotorStopAll();
					motor_row =( (gU2RecvBuff.data[0]<<8) | gU2RecvBuff.data[1] );
					motor_col =( (gU2RecvBuff.data[2]<<8) | gU2RecvBuff.data[3] );			
					for(i=0;i<11;i++)
					{
						if (( (motor_row >>i) & 0x01 ) == 0x01) 
						{
							for (j = 0; j<11;j++)
							{
								if (( (motor_col >>j) & 0x01 ) == 0x01) 
									MotorDrive( 0, i+1, j+1);//should tell master maybe the motor could not start																													
							}
						}
					}
					Delay_us(400);
					break;
				case StopAllMotor:
					MotorStopAll();
					break;
				case ReverseRowMotor:
					MotorDriveResverse(gU2RecvBuff.data[0]);
					break;
				case CheckPtoEtcSW:
					PtoEtcSWCheckResult = ELSCheck();
					USART_SendData(USART2, PtoEtcSWCheckResult);
					while (!(USART2->SR & USART_FLAG_TXE));
					USART_SendData(USART2, PtoEtcSWCheckResult >> 8);
					while (!(USART2->SR & USART_FLAG_TXE));
					PtoEtcSWCheckResult = 0;
					//EchoToMaster(&HandShakeToMaster[0]);
					break;
				case StartPtoEtcSW:
					StartAllPtoEtcSW();
					break;
				case StopPtoEtcSW:
					StopAllPtoEtcSW();
					break;
				case StartLED:
					MotorColDrive(0,10);					
					break;
				case StopLED:
					MotorColStop(10);
					break;
				default:
					break;
			}
			ClearProtocol();		
}
		if(1)
		{
			//check photoelectric switch
			if ( WaitPtoEtcSW  )
			{
				if ((GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13) == 1)) 
				{
					MotorStopAll();
					WaitPtoEtcSW = 0;	
					STOP_TIME2;
					printf("Medicine_ok\r\n");
				}							
			}
			//11 Col current check
			for(i=0;i<11;i++)
			{
				for (j = 0; j<3; j++)
				{
					MeanRunningCurrent += ADC_ConvertedValue[i];
				}
				if (MeanRunningCurrent > 3234) //if anything wrong during running(current feedback), stop all the motors;
				{
					MotorColStop(i+1);
					WaitPtoEtcSW = 0;	
					STOP_TIME2;
					printf("Col:%d current over range, stopped \r \n", i+1);
				}
				MeanRunningCurrent = 0;
				for(j=0;j<250;j++);//just for time Delay_us;
			}			
			//PtoEtcSW wait time terminated
			if (step_timer2 > 1000) 
			{
				WaitPtoEtcSW = 0;
				step_timer2 = 0;
				printf("failed to get the medicine");
				STOP_TIME2;
				MotorStopAll();
			}
		}
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
			
void EchoToMaster(unsigned char *ptr)
{
	unsigned char i;
	for(i=0; i<7; i++)	
	{
		USART_SendData(USART2, *ptr++);	
		while (!(USART2->SR & USART_FLAG_TXE));
	}
}

void ClearProtocol(void)
{
	unsigned char i;
	gU2RecvBuff.header = 0;
	gU2RecvBuff.command = 0;
	for(i=0; i<4; i++) gU2RecvBuff.data[i] = 0;
	gU2RecvBuff.checksum = 0;
}

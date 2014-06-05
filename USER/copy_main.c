#include "includes.h"

void ADC1_Test(void);
void MosTest(void);
void I2CSoftwareTest(void);
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
	TIM2_NVIC_Configuration();
	TIM2_Configuration();
		
	if ( I2C_PCF8574_BufferRead( &IDOfPCB, 0x40 ) ) 	
	{
		PCBID[2] = IDOfPCB & 0x0F;
		PCBID[3] = ( (IDOfPCB & 0xF0)>>4 );
		PCBID[6] = ( 0xAB+PCBID[2]+PCBID[3] );
		EchoToMaster(&PCBID[0]);
	}
	else 
	{
		PCBID[2] = 0xFF;
		PCBID[3] = 0xFF;
		PCBID[6] = 0xA9;
		EchoToMaster(&PCBID[0]);
	}
		
	while (1)
	{
		switch (TchScrSltStatus)
		{
			case MotorStoppedTop:
				//check key
				break;
			case KeyPushed:
				break;
			case MotorStartDown:
				break;
			case DownLimSW:
				break;
			case MotorStoppedBottom:
				break;		
			case InfraredSensorStatus:
				break;
			case TimerStarted:
				break;
			case TimerTerminated:
				break;
			case MotorstartUp:
				break;
			case UpLimSW:
				break;
			default:
				break;			
		}
		if (gU2RecvBuff.protocol_ok)
		{
			gU2RecvBuff.protocol_ok = 0;
			EchoToMaster(&HandShakeToMaster[0]);//Echo to master to claim 
												//that slave have received the information correctly;
			
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
								{
									MotorStartInfo[2] = (i+1);
									MotorStartInfo[3] = (j+1);
									MotorStartInfo[4] = MotorDrive( 1, i+1, j+1) ;
									MotorStartInfo[6] = (0xB7+i+j+MotorStartInfo[4]);									
									EchoToMaster(&MotorStartInfo[0]);
								}
							}
						}
					}
					START_TIME2;
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
								{
									MotorStartInfo[2] = (i+1);
									MotorStartInfo[3] = (j+1);
									MotorStartInfo[4] = MotorDrive( 0, i+1, j+1) ;
									MotorStartInfo[6] = (0xB7+i+j+MotorStartInfo[4]);									
									EchoToMaster(&MotorStartInfo[0]);
								}
							}
						}
					}
					Delay_us(400);
					break;
				case StopAllMotor:
					MotorStopAll();
					if(! (((GPIO_ReadOutputData(GPIOA)&0x8A10)<0x8A10) | ((GPIO_ReadOutputData(GPIOB)&0xAC00)<0xAC00)\
					| ((GPIO_ReadOutputData(GPIOC)&0x1680)<0x1680) | ((GPIO_ReadOutputData(GPIOD)&0xA000)<0xA000)\
					| ((GPIO_ReadOutputData(GPIOE)&0x780)<0x780)) )
					{
						EchoToMaster(&AllMotorStopped[0]);
					}
					break;
				case ReverseRowMotor:
					MotorDriveResverse(gU2RecvBuff.data[0]);//**************************echo back to master not finished
					break;
				case CheckPtoEtcSW:
					PtoEtcSWCheckResult = ELSCheck();
					PtoEtcSWCheckResInfo[3] = (unsigned char)(PtoEtcSWCheckResult);
					PtoEtcSWCheckResInfo[2] = (unsigned char)(PtoEtcSWCheckResult>>8); 
					PtoEtcSWCheckResInfo[6] = 0xAA+0x08+PtoEtcSWCheckResInfo[2]+PtoEtcSWCheckResInfo[3];
					EchoToMaster(&PtoEtcSWCheckResInfo[0]);
					PtoEtcSWCheckResult = 0;
					PtoEtcSWCheckResInfo[3] = 0;
					PtoEtcSWCheckResInfo[2] = 0; 
					PtoEtcSWCheckResInfo[6] = 0xB2;
					break;
				case StartPtoEtcSW:
					StartAllPtoEtcSW();
					EchoToMaster(&StartPtoEtcSWSuccedded[0]);
					break;
				case StopPtoEtcSW:
					StopAllPtoEtcSW();
					EchoToMaster(&StopPtoEtcSWSuccedded[0]);
					break;
				case StartLED:
					MotorColDrive(0,10);
					EchoToMaster(&StarLEDSuccedded[0]);				
					break;
				case StopLED:
					MotorColStop(10);
					EchoToMaster(&StopLEDSuccedded[0]);	
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
					EchoToMaster(&MedecineSuccedded[0]);
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
					ColCurrentOverRange[2] = (i+1);
					ColCurrentOverRange[6] = (i+1)+0xB4;
					EchoToMaster(&ColCurrentOverRange[0]);
					ColCurrentOverRange[2] = 0;
					ColCurrentOverRange[6] = 0xB4;
				}
				MeanRunningCurrent = 0;
				for(j=0;j<250;j++);//just for time Delay_us;
			}			
			//PtoEtcSW wait time terminated
			if (step_timer2 > 1000) 
			{
				WaitPtoEtcSW = 0;
				step_timer2 = 0;
				EchoToMaster(&MedecineFailed[0]);
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

/*
void I2C_Test(void)
{
	uint8_t I2CInput;
	uint8_t Write_Buffer = 0xFF;
	uint8_t PCF8574Address = 0x40;
	I2C_PCF8574_Init();
	I2C_PCF8574_ByteWrite(Write_Buffer , PCF8574Address);
	I2CInput = I2C_PCF8574_ByteRead(PCF8574Address);
	printf("%d", I2CInput);
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

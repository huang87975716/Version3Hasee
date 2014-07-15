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
	//IWDG_Configuration();
	
// 	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
// 	{ 
// 		printf("WatchDog Reset\r\n"); 
// 		RCC_ClearFlag();
// 	}
	
	if ( I2C_PCF8574_BufferRead( &IDOfPCB, 0x40 ) ) 	
	{
		for(i=0; i<4;i++)
		{
			RunMode |= (IDOfPCB & (0x01<<(i+4))) >> (2*i+1);
		}
		USART_printf( USART2,"RunMode is %d.\r\n", RunMode);
	}
	
	if ( RunMode == RunModeTest )
	{
		for (j_testloop = 1; j_testloop<10; j_testloop++ )
		{
			for (i_testloop =1 ; i_testloop<10;i_testloop++ )
			{
				MotorStopAll();
				StartTimes = MotorDrive( 0, j_testloop, i_testloop, 2 );
				USART_printf( USART2,"\r\n Row %d, Col %d, StartTimes %d, Backward\r\n",j_testloop,i_testloop,StartTimes);
				if (StartTimes < 2) MotorStatus = Backward;			
				else MotorStatus = Stopped;
				Delay_us(500);			
				while(MotorStatus == Backward)
				{
					for (j = 0; j<3; j++)
					{
						MeanRunningCurrent += ADC_ConvertedValue[i_testloop-1];
					}
					if (MeanRunningCurrent > LimitCurrent) 
					{
						MotorColStop(i_testloop);
						MotorStatus = Stopped;
						USART_printf( USART2,"Motor Stopped\r\n");
					}
					MeanRunningCurrent = 0;
					Delay_us(1);
				}
				Delay_us(500);
				
				//MotorStopAll();
				// StartTimes = MotorDrive( 1, j_testloop, i_testloop );
				// USART_printf( USART2,"Row %d, Col %d, StartTimes %d, Forward\r\n",j_testloop,i_testloop,StartTimes);
				// if (StartTimes < 7) MotorStatus = Forward;			
				// else MotorStatus = Stopped;		
				// Delay_us(500);
				// while(MotorStatus == Forward)
				// {
					// for (j = 0; j<3; j++)
					// {
						// MeanRunningCurrent += ADC_ConvertedValue[i_testloop-1];
					// }
					// if (MeanRunningCurrent > LimitCurrent) 
					// {
						// MotorColStop(i_testloop);
						// MotorStatus = Stopped;
						// USART_printf( USART2,"Motor Stopped\r\n");
					// }
					// MeanRunningCurrent = 0;
					// Delay_us(1);
				// }
				// Delay_us(1000);

				// MotorStopAll();
				// StartTimes = MotorDrive( 0, j_testloop, i_testloop );
				// USART_printf( USART2,"Row %d, Col %d, StartTimes %d, Backward\r\n",j_testloop,i_testloop,StartTimes);
				// if (StartTimes < 7) MotorStatus = Backward;			
				// else MotorStatus = Stopped;		
				// Delay_us(500);			
				// while(MotorStatus == Backward)
				// {
					// for (j = 0; j<3; j++)
					// {
						// MeanRunningCurrent += ADC_ConvertedValue[i_testloop-1];
					// }
					// if (MeanRunningCurrent > LimitCurrent) 
					// {
						// MotorColStop(i_testloop);
						// MotorStatus = Stopped;
						// USART_printf( USART2,"Motor Stopped\r\n");
					// }
					// MeanRunningCurrent = 0;
					// Delay_us(1);
				// }
				// Delay_us(1000);
			}
		}	
	}
	
	if ( RunMode == 0x05 )
	{
		for (j_testloop = 1; j_testloop<10; j_testloop++ )
		{
			for (i_testloop =1 ; i_testloop<10;i_testloop++ )
			{
				MotorStopAll();
				StartTimes = MotorDrive( 1, j_testloop, i_testloop, 7 );
				USART_printf( USART2,"\r\n Row %d, Col %d, StartTimes %d, Forward\r\n",j_testloop,i_testloop,StartTimes);
				if (StartTimes < 7) MotorStatus = Forward;			
				else MotorStatus = Stopped;		
				Delay_us(500);
				while(MotorStatus == Forward)
				{
					for (j = 0; j<3; j++)
					{
						MeanRunningCurrent += ADC_ConvertedValue[i_testloop-1];
					}
					if (MeanRunningCurrent > LimitCurrent) 
					{
						MotorColStop(i_testloop);
						MotorStatus = Stopped;
						USART_printf( USART2,"Motor Stopped\r\n");
					}
					MeanRunningCurrent = 0;
					Delay_us(1);
				}
				Delay_us(500);
				
				// MotorStopAll();
				// StartTimes = MotorDrive( 0, j_testloop, i_testloop );
				// USART_printf( USART2,"Row %d, Col %d, StartTimes %d, Backward\r\n",j_testloop,i_testloop,StartTimes);
				// if (StartTimes < 7) MotorStatus = Backward;			
				// else MotorStatus = Stopped;
				// Delay_us(500);			
				// while(MotorStatus == Backward)
				// {
					// for (j = 0; j<3; j++)
					// {
						// MeanRunningCurrent += ADC_ConvertedValue[i_testloop-1];
					// }
					// if (MeanRunningCurrent > LimitCurrent) 
					// {
						// MotorColStop(i_testloop);
						// MotorStatus = Stopped;
						// USART_printf( USART2,"Motor Stopped\r\n");
					// }
					// MeanRunningCurrent = 0;
					// Delay_us(1);
				// }
				// Delay_us(1000);
				
				// MotorStopAll();
				// StartTimes = MotorDrive( 1, j_testloop, i_testloop );
				// USART_printf( USART2,"Row %d, Col %d, StartTimes %d, Forward\r\n",j_testloop,i_testloop,StartTimes);
				// if (StartTimes < 7) MotorStatus = Forward;			
				// else MotorStatus = Stopped;		
				// Delay_us(500);
				// while(MotorStatus == Forward)
				// {
					// for (j = 0; j<3; j++)
					// {
						// MeanRunningCurrent += ADC_ConvertedValue[i_testloop-1];
					// }
					// if (MeanRunningCurrent > LimitCurrent) 
					// {
						// MotorColStop(i_testloop);
						// MotorStatus = Stopped;
						// USART_printf( USART2,"Motor Stopped\r\n");
					// }
					// MeanRunningCurrent = 0;
					// Delay_us(1);
				// }
				// Delay_us(1000);
			}
		}	
	}
	
	EchoToMaster(&HandShakeToMaster[0]);
	while (1)
	{		
		//if (TchScrSltStatus != 1) USART_printf( USART2,"\r\n TchScrSltStatus %d, step_timer3 %d, TchActd %d\r\n",\
		//										TchScrSltStatus,step_timer3,TouchScreenActivity);
		//IWDG_ReloadCounter();
		switch (TchScrSltStatus)
		{
			case MotorStoppedTop:			
				I2C_PCF8574_BufferRead(&I2CTouchKey, 0x42);
				if( !(I2CTouchKey &= 1<<3) ) 
				{
					Delay_us(50);
					I2C_PCF8574_BufferRead(&I2CTouchKey, 0x42);
					if( !(I2CTouchKey &= 1<<3) )  TchScrSltStatus = KeyPushed;
				}
				break;
			case KeyPushed:
				if( !DownLimSWCheck ) 
				{
					if(CheckMotorStatus() == 0)
					{
						EchoToMaster(&ShelterOpened[0]);	
						MotorStopAll();
						MotorDrive(1,9,11,7);//backforward
						TchScrSltStatus = MotorStartDown;
					}
				}
				else TchScrSltStatus = MotorStoppedBottom;
				break;
			case MotorStartDown:
				if( DownLimSWCheck )
				{
					MotorStopAll();
					TchScrSltStatus = MotorStoppedBottom;
				}
				break;
			case MotorStoppedBottom:
				if(TouchScreenActivity == 0)
				{
					I2C_PCF8574_BufferRead(&I2CInfaraedSsr, 0x42);
					if ( (I2CInfaraedSsr &= 1<<2) ) 
					{
						TchScrSltStatus = InfraredSensorFirst;//
						START_TIME3;
					}
				}
				else 
				{
					TchScrSltStatus = MotorStoppedBottom;
					step_timer3 = 0;
					STOP_TIME3;
				}
				break;		
			case InfraredSensorFirst:
				if(TouchScreenActivity == 0)
				{
					if(step_timer3 >= 500) 
					{
						step_timer3 = 0;
						STOP_TIME3;
						I2C_PCF8574_BufferRead(&I2CInfaraedSsr, 0x42);
						if ( (I2CInfaraedSsr &= 1<<2) ) 
						{
							TchScrSltStatus = InfraredSensorSecond;
							START_TIME3;
							EchoToMaster(&ShelterClosed[0]);
						}
						else 
						{
							TchScrSltStatus = MotorStoppedBottom;
						}
					}
				}
				else 
				{
					TchScrSltStatus = MotorStoppedBottom;
					step_timer3 = 0;
					STOP_TIME3;
				}
				break;
			case InfraredSensorSecond:
				if(TouchScreenActivity == 0)
				{
					if(step_timer3 >= 500) 
					{
						TchScrSltStatus = TimerTerminated;
						STOP_TIME3;
						step_timer3 = 0;
					}
				}
				else 
				{
					TchScrSltStatus = MotorStoppedBottom;
					step_timer3 = 0;
					STOP_TIME3;
				}
				break;
			case TimerTerminated:
				if(TouchScreenActivity == 0)
				{
					if( UpLimSWCheck )
					{
						if(CheckMotorStatus() == 0)
						{
							MotorStopAll();
							MotorDrive(0,9,11,7);
							TchScrSltStatus = MotorstartUp;
						}
					}
					else TchScrSltStatus = MotorStoppedTop;
				}
				else TchScrSltStatus = MotorStoppedBottom;
				break;
			case MotorstartUp:
				if(TouchScreenActivity == 0)
				{
					if( (!DownLimSWCheck) && (!UpLimSWCheck ) ) 
					{
						Delay_us(100);
						MotorStopAll();
						TchScrSltStatus = MotorStoppedTop;
					}
				}
				else 
				{
					TchScrSltStatus = MotorStoppedTop;
					MotorStopAll();
				}
				break;
			case DoingNothing:
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
					if(CheckMotorStatus() == 0)
					{
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
										MotorStopAll();
										MotorStartInfo[4] = MotorDrive( 1, i+1, j+1,7) ;
										MotorStartInfo[6] = (0xB7+i+j+MotorStartInfo[4]);									
										EchoToMaster(&MotorStartInfo[0]);
									}
								}
							}
						}
						Delay_us(500);
					}
					else EchoToMaster(&MotorRunningTryLater[0]);
					break;
				case MotorBackward:
					if(CheckMotorStatus() == 0)
					{
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
										MotorStopAll();
										MotorStartInfo[4] = MotorDrive( 0, i+1, j+1,7) ;
										MotorStartInfo[6] = (0xB7+i+j+MotorStartInfo[4]);									
										EchoToMaster(&MotorStartInfo[0]);
									}
								}
							}
						}
						Delay_us(400);
					}
					else EchoToMaster(&MotorRunningTryLater[0]);
					break;
				case StopAllMotor: 
					MotorStopAll();
					if(! (((GPIO_ReadOutputData(GPIOA)&0x8A10)<0x8A10) | ((GPIO_ReadOutputData(GPIOB)&0xEC00)<0xEC00)\
					| ((GPIO_ReadOutputData(GPIOC)&0x1680)<0x1680) | ((GPIO_ReadOutputData(GPIOD)&0xA200)<0xA200)\
					| ((GPIO_ReadOutputData(GPIOE)&0x780)<0x780)) )//need to be verfied
					{
						EchoToMaster(&AllMotorStopped[0]);
					}
					break;
				case ReverseRowMotor:
					MotorStopAll();
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
					WaitPtoEtcSW = 1;
					EchoToMaster(&StartPtoEtcSWSuccedded[0]);
					break;
				case StopPtoEtcSW:
					StopAllPtoEtcSW();
					WaitPtoEtcSW = 0;
					EchoToMaster(&StopPtoEtcSWSuccedded[0]);
					break;
				case StartLED:
					MotorStopAll();
					MotorColDrive(0,10);
					EchoToMaster(&StarLEDSuccedded[0]);				
					break;
				case StopLED:
					MotorStopAll();
					MotorColStop(10);
					EchoToMaster(&StopLEDSuccedded[0]);	
					break;
				case UploadPCBID:
					if ( I2C_PCF8574_BufferRead( &IDOfPCB, 0x40 ) ) 	
					{
						PCBID[2] = IDOfPCB & 0x0F;
						for(i=0; i<4;i++)
						{
							PCBID[3] |= (IDOfPCB & (0x01<<(i+4))) >> (2*i+1);
						}
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
					break;
				case ReadI2C2:
					I2C_PCF8574_BufferRead(&I2CTouchKey, 0x42);
					USART_printf( USART2," I2C addressed 0x42 is %d\r\n",I2CTouchKey);
					break;
				case ReadOptCplr:
					USART_printf( USART2," S_SEN1 is %d\r\n",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13));
					USART_printf( USART2," S_SEN2 is %d\r\n",GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2));
					USART_printf( USART2," S_SEN3 is %d\r\n",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14));
					USART_printf( USART2," S_SEN4 is %d\r\n",GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3));
					break;
				case L298Driver:
					if (gU2RecvBuff.data[0]==1)
					{
						GPIO_ResetBits(GPIOE,GPIO_Pin_11);
						GPIO_SetBits(GPIOE,GPIO_Pin_12);
						GPIO_SetBits(GPIOA,GPIO_Pin_1);
						USART_printf( USART2," Enable L298, TC_S1=1, TC_S2=0 \r\n");
					}
					else if (gU2RecvBuff.data[0]==2)
					{
						GPIO_ResetBits(GPIOE,GPIO_Pin_12);
						GPIO_SetBits(GPIOE,GPIO_Pin_11);
						GPIO_SetBits(GPIOA,GPIO_Pin_1);
						USART_printf( USART2," Enable L298, TC_S1=0, TC_S2=1 \r\n");
					}
					else if (gU2RecvBuff.data[0]==3)
					{
						GPIO_SetBits(GPIOE,GPIO_Pin_11 | GPIO_Pin_12);
						GPIO_SetBits(GPIOA,GPIO_Pin_1);
						USART_printf( USART2," Enable L298, TC_S1=1, TC_S2=1 \r\n");
					}
					else 
					{
						GPIO_ResetBits(GPIOE,GPIO_Pin_11 | GPIO_Pin_12);
						GPIO_ResetBits(GPIOA,GPIO_Pin_1);
						USART_printf( USART2,"Disable L298 and All control port is set zero \r\n");
					}
					break;
				case ShelterUpToLimitSW:
					if (UpLimSWCheck &&((TchScrSltStatus == MotorStoppedTop)|| (TchScrSltStatus == DoingNothing) ) )	
					{
						MotorStopAll();
						MotorDrive(0,9,11,7);//Up
						ShelterUp = 1;
					}
					break;
				case ShelterDownToLimitSW:
					if( (!DownLimSWCheck) && (TchScrSltStatus == MotorStoppedTop))
					{
						MotorStopAll();
						MotorDrive(1,9,11,7);//Down
						ShelterDown = 1;
					}
					break;
				case ReadAllCurrent:
					for(i=0;i<11;i++)
					{
						USART_printf(USART2,"\r\n the current of Column %d is %d/4620A.\r\n",i+1,ADC_ConvertedValue[i]);
					}
					break;
				case TouchScreenStart:
					TouchScreenActivity = 1;
					break;
				case TouchScreenEnd:
					TouchScreenActivity = 0;
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
					EchoToMaster(&MedecineSuccedded[0]);
				}							
			}
			//10 Col current check
			for(i=0;i<10;i++)
			{
				for (j = 0; j<3; j++)
				{
					MeanRunningCurrent += ADC_ConvertedValue[i];
					//Delay_us(1);
				}
				if (MeanRunningCurrent > LimitCurrent) //if anything wrong during running(current feedback), stop all the motors;
				{
					MotorColStop(i+1);
					ColCurrentOverRange[2] = (i+1);
					ColCurrentOverRange[6] = (i+1)+0xB4;
					EchoToMaster(&ColCurrentOverRange[0]);
					ColCurrentOverRange[2] = 0;
					ColCurrentOverRange[6] = 0xB4;
				}
				MeanRunningCurrent = 0;
				//for(j=0;j<50;j++);//just for time Delay_us;
			}	

			for (j = 0; j<3; j++)
			{
				MeanRunningCurrent += ADC_ConvertedValue[10];
				//Delay_us(1);
			}

			if (TchScrSltStatus == MotorStartDown)	ShelterCrtLim = 4620;
			else ShelterCrtLim = 10000;
			
			if (MeanRunningCurrent > ShelterCrtLim ) //4620 -- 1A
			{
				ColCurrentOverRange[2] = 0x0B;
				ColCurrentOverRange[6] = (0x0B+0xB4);
				EchoToMaster(&ColCurrentOverRange[0]);
				ColCurrentOverRange[2] = 0;
				ColCurrentOverRange[6] = 0xB4;
				TchScrSltStatus = MotorStoppedTop;
				MotorStopAll();
			}
			MeanRunningCurrent = 0;
			//for(j=0;j<50;j++);//just for time Delay_us;
			
			if (ShelterUp)
			{
				if ( (!UpLimSWCheck) && (!DownLimSWCheck) )
				{
					
					Delay_us(100);
					MotorStopAll();
					ShelterUp = 0;
					TchScrSltStatus = MotorStoppedTop;
				}
			}
			if (ShelterDown)
			{
				if ( DownLimSWCheck )
				{
					MotorStopAll();
					ShelterDown = 0;
					TchScrSltStatus = DoingNothing;
				}
			}
// 			//PtoEtcSW wait time terminated
// 			if (step_timer2 > DelayTimeOfTimer2) 
// 			{
// 				WaitPtoEtcSW = 0;
// 				step_timer2 = 0;
// 				EchoToMaster(&MedecineFailed[0]);
// 				STOP_TIME2;
// 				MotorStopAll();
// 			}
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

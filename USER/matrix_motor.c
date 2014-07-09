#include "matrix_motor.h"
#include "stm32f10x.h"
#include "PCF8574_I2C.h"
#include "SysTick.h"
#include "usart.h"

extern volatile u16 ADC_ConvertedValue[11];
extern unsigned char I2CValue2;

/**/
unsigned char MotorDrive(int Dir, int Row, int Col, int TryTimes)
{
	unsigned char i= 0;
	if( (Row==9) && (Col==11) )
	{
		MotorRowDrive (Dir , Row);
		MotorColDrive (Dir , Col);
		Delay_us(500);
		return i;
	}
	
	for (i = 0; i < TryTimes; i++ )
	{
		MotorRowDrive (Dir , Row);
		MotorColDrive (Dir , Col);
		
		Delay_us(150);
		if (ADC_ConvertedValue[Col-1] > 1540) //1540/4096*2.66=1A
		{
			//printf("\r\n start current is %d \r\n",ADC_ConvertedValue[Col-1]);
			MotorColStop(Col);
			Delay_us(500);				
		}
		else break;	
	}
	return i;
}
/**/
void MotorDriveResverse(int Row)
{
		unsigned char i;
		unsigned int current = 0;
		MotorRowDrive(0,Row);
	
		for (i = 1; i <= 9; i++ )
		{
			MotorColDrive(0,i);//
			Delay_us(200);			
			if (ADC_ConvertedValue[i-1] > 1540) //1540/4096*2.66=1A
			{
				MotorColStop(i);
				Delay_us(500);				
			}
		}
		
		for(i = 0; i <9; i++) current += ADC_ConvertedValue[i];
		printf("\r\ntotal current is %d \r\n",current);
}
/**/
void MotorStopAll(void)
{
	unsigned char i;
	for (i = 1; i <= 9; i++)
	{
	MotorRowStop(i);
	MotorColStop(i);
	}
	MotorRowStop(10);//col 10 was connected to LED, not included here
	MotorRowStop(11);
	MotorColStop(11);
	/**************************************************************/
	Delay_us(500);//
	/**************************************************************/
}
/**/
void MotorColStop(int Col)
{
	
	unsigned char i;
	switch (Col) 
	{
		case 1:
				GPIO_SetBits(GPIOC, GPIO_Pin_12);//Col 1st
				GPIO_SetBits(GPIOA, GPIO_Pin_4);
			break;
		case 2:
				GPIO_SetBits(GPIOC, GPIO_Pin_10);//Col 2nd
				GPIO_SetBits(GPIOE, GPIO_Pin_7);
			break;
		case 3:
				GPIO_SetBits(GPIOA, GPIO_Pin_15);//Col 3rd
				GPIO_SetBits(GPIOE, GPIO_Pin_8);
			break;
		case 4:
				GPIO_SetBits(GPIOA, GPIO_Pin_11);//Col 4th
				GPIO_SetBits(GPIOE, GPIO_Pin_9);
			break;
		case 5:
				GPIO_SetBits(GPIOA, GPIO_Pin_9);//Col 5th
				GPIO_SetBits(GPIOE, GPIO_Pin_10);
			break;
		case 6:
				GPIO_SetBits(GPIOC, GPIO_Pin_9);//Col 6th
				GPIO_SetBits(GPIOB, GPIO_Pin_10);
			break;
		case 7:
				GPIO_SetBits(GPIOC, GPIO_Pin_7);//Col 7th
				GPIO_SetBits(GPIOB, GPIO_Pin_11);
			break;
		case 8:
				GPIO_SetBits(GPIOD, GPIO_Pin_15);//Col 8th
				GPIO_SetBits(GPIOB, GPIO_Pin_13);
			break;
		case 9:
				GPIO_SetBits(GPIOD, GPIO_Pin_13);//Col 9th
				GPIO_SetBits(GPIOB, GPIO_Pin_15);
			break;
		case 10:
				GPIO_SetBits(GPIOD, GPIO_Pin_11);//Col 10th
				GPIO_SetBits(GPIOB, GPIO_Pin_12);
			break;
		case 11:
				GPIO_SetBits(GPIOD, GPIO_Pin_9);//Col 11th
				GPIO_SetBits(GPIOB, GPIO_Pin_14);
		break;
		default:
			break;
	}
	//必须所有的列关闭，行才能关闭，if的条件中!0 = 1， 则所有的为0，相或才能为0；
	//若要每个为0，则必须大于等于0x8A10,如果要等于8A10,则必须所有的都为高电平
		if(! (((GPIO_ReadOutputData(GPIOA)&0x8A10)<0x8A10) | ((GPIO_ReadOutputData(GPIOB)&0xEC00)<0xEC00)\
		| ((GPIO_ReadOutputData(GPIOC)&0x1680)<0x1680) | ((GPIO_ReadOutputData(GPIOD)&0xA200)<0xA200)\
		| ((GPIO_ReadOutputData(GPIOE)&0x780)<0x780) ))//未检测10列的正反转
		{
			for(i = 1; i <=9; i++)
			{
				MotorRowStop(i);
			}
		}
}

/**/
void MotorRowStop(int Row)
{
	switch (Row) //
	{
		case 1:
				GPIO_SetBits(GPIOD, GPIO_Pin_8);//Row 1st
				GPIO_SetBits(GPIOC, GPIO_Pin_11);
			break;
		case 2:
				GPIO_SetBits(GPIOD, GPIO_Pin_10);//Row 2nd
				GPIO_SetBits(GPIOD, GPIO_Pin_4);
			break;
		case 3:
				GPIO_SetBits(GPIOD, GPIO_Pin_12);//Row 3rd
				GPIO_SetBits(GPIOE, GPIO_Pin_3);
			break;
		case 4:
				GPIO_SetBits(GPIOD, GPIO_Pin_14);//Row 4th
				GPIO_SetBits(GPIOE, GPIO_Pin_2);
			break;
		case 5:
				GPIO_SetBits(GPIOC, GPIO_Pin_6);//Row 5th
				GPIO_SetBits(GPIOE, GPIO_Pin_5);
			break;
		case 6:
				GPIO_SetBits(GPIOC, GPIO_Pin_8);//Row 6th
				GPIO_SetBits(GPIOE, GPIO_Pin_4);
			break;
		case 7:
				GPIO_SetBits(GPIOA, GPIO_Pin_8);//Row 7th
				GPIO_SetBits(GPIOC, GPIO_Pin_13);
			break;
		case 8:
				GPIO_SetBits(GPIOA, GPIO_Pin_10);//Row 8th
				GPIO_SetBits(GPIOE, GPIO_Pin_6);
			break;
		case 9:
				GPIO_SetBits(GPIOA, GPIO_Pin_12);//Row 9th
				GPIO_SetBits(GPIOC, GPIO_Pin_15);
			break;
		case 10:
				I2CValue2 |= 0x40;
				I2C_PCF8574_BufferWrite(I2CValue2, 0x42 );//Row 10th
				GPIO_SetBits(GPIOC, GPIO_Pin_14);
			break;
		case 11:
				I2CValue2 |=0x80;
				I2C_PCF8574_BufferWrite(I2CValue2, 0x42 );//Row 11th
				GPIO_SetBits(GPIOA, GPIO_Pin_0);
			break;
		default:
			break;
	}
}
/**/
void MotorRowDrive(int Dir, int Row)
{
	switch (Dir)
	{
	case 0:
		switch (Row) //
			{
				case 1:
						GPIO_ResetBits(GPIOD, GPIO_Pin_8);//Row 1st
					break;
				case 2:
						GPIO_ResetBits(GPIOD, GPIO_Pin_10);//Row 2nd
					break;
				case 3:
						GPIO_ResetBits(GPIOD, GPIO_Pin_12);//Row 3rd
					break;
				case 4:
						GPIO_ResetBits(GPIOD, GPIO_Pin_14);//Row 4th
					break;
				case 5:
						GPIO_ResetBits(GPIOC, GPIO_Pin_6);//Row 5th
					break;
				case 6:
						GPIO_ResetBits(GPIOC, GPIO_Pin_8);//Row 6th
					break;
				case 7:
						GPIO_ResetBits(GPIOA, GPIO_Pin_8);//Row 7th
					break;
				case 8:
						GPIO_ResetBits(GPIOA, GPIO_Pin_10);//Row 8th
					break;
				case 9:
						GPIO_ResetBits(GPIOA, GPIO_Pin_12);//Row 9th
					break;
				case 10:
						I2CValue2 &= 0xBF;
						I2C_PCF8574_BufferWrite(I2CValue2, 0x42 );//Row 10th
					break;
				case 11:
						I2CValue2 &=0x7F;
						I2C_PCF8574_BufferWrite(I2CValue2, 0x42 );//Row 11th
					break;
				default:
					break;
			
			}
	break;
	case 1:
		switch (Row)	//
			{
			case 1:
					GPIO_ResetBits(GPIOC, GPIO_Pin_11);//Row 1st
				break;
			case 2:
					GPIO_ResetBits(GPIOD, GPIO_Pin_4);//Row 2nd
				break;
			case 3:
					GPIO_ResetBits(GPIOE, GPIO_Pin_3);//Row 3rd
				break;
			case 4:
					GPIO_ResetBits(GPIOE, GPIO_Pin_2);//Row 4th
				break;
			case 5:
					GPIO_ResetBits(GPIOE, GPIO_Pin_5);//Row 5th
				break;
			case 6:
					GPIO_ResetBits(GPIOE, GPIO_Pin_4);//Row 6th
				break;
			case 7:
					GPIO_ResetBits(GPIOC, GPIO_Pin_13);//Row 7th
				break;
			case 8:
					GPIO_ResetBits(GPIOE, GPIO_Pin_6);//Row 8th
				break;
			case 9:
					GPIO_ResetBits(GPIOC, GPIO_Pin_15);//Row 9th
				break;
			case 10:
					GPIO_ResetBits(GPIOC, GPIO_Pin_14);//Row 10th
				break;
			case 11:
					GPIO_ResetBits(GPIOA, GPIO_Pin_0);//Row 11th
				break;
			default:
				break;
			}
	break;
	default:
	break;
	}
}
/**/
void MotorColDrive(int Dir, int Col)
{
	switch (Dir)
		{	
		case 0://motor resverse
			
			switch (Col)  //
				{
			case 1:
					GPIO_ResetBits(GPIOC, GPIO_Pin_12);//Col 1st
				break;
			case 2:
					GPIO_ResetBits(GPIOC, GPIO_Pin_10);//Col 2nd
				break;
			case 3:
					GPIO_ResetBits(GPIOA, GPIO_Pin_15);//Col 3rd
				break;
			case 4:
					GPIO_ResetBits(GPIOA, GPIO_Pin_11);//Col 4th
				break;
			case 5:
					GPIO_ResetBits(GPIOA, GPIO_Pin_9);//Col 5th
				break;
			case 6:
					GPIO_ResetBits(GPIOC, GPIO_Pin_9);//Col 6th
				break;
			case 7:
					GPIO_ResetBits(GPIOC, GPIO_Pin_7);//Col 7th
				break;
			case 8:
					GPIO_ResetBits(GPIOD, GPIO_Pin_15);//Col 8th
				break;
			case 9:
					GPIO_ResetBits(GPIOD, GPIO_Pin_13);//Col 9th
				break;
			case 10:
					GPIO_ResetBits(GPIOD, GPIO_Pin_11);//Col 10th
				break;
			case 11:
					GPIO_ResetBits(GPIOD, GPIO_Pin_9);//Col 11th
				break;
			default:
				break;
				}
			break;
					
		case 1://
			
				switch (Col)	//
					{
					case 1:
							GPIO_ResetBits(GPIOA, GPIO_Pin_4);//Col 1st
						break;
					case 2:
							GPIO_ResetBits(GPIOE, GPIO_Pin_7);//Col 2nd
						break;
					case 3:
							GPIO_ResetBits(GPIOE, GPIO_Pin_8);//Col 3rd
						break;
					case 4:
							GPIO_ResetBits(GPIOE, GPIO_Pin_9);//Col 4th
						break;
					case 5:
							GPIO_ResetBits(GPIOE, GPIO_Pin_10);//Col 5th
						break;
					case 6:
							GPIO_ResetBits(GPIOB, GPIO_Pin_10);//Col 6th
						break;
					case 7:
							GPIO_ResetBits(GPIOB, GPIO_Pin_11);//Col 7th
						break;
					case 8:
							GPIO_ResetBits(GPIOB, GPIO_Pin_13);//Col 8th
						break;
					case 9:
							GPIO_ResetBits(GPIOB, GPIO_Pin_15);//Col 9th
						break;
					case 10:
							GPIO_ResetBits(GPIOB, GPIO_Pin_12);	//Col 10th
						break;
					case 11:
							GPIO_ResetBits(GPIOB, GPIO_Pin_14);//Col 11th
					break;
					default:
						break;
				}
			break;
		default:
			break;
		}
}

unsigned char CheckMotorStatus(void)
{
	unsigned char MotorRunning = 0;
	//读取列与行状态,未检测第10列（正反转）
	if(! ( ((GPIO_ReadOutputData(GPIOA)&0x8A10)<0x8A10) | ((GPIO_ReadOutputData(GPIOB)&0xEC00)<0xEC00)\
		| ((GPIO_ReadOutputData(GPIOC)&0x1680)<0x1680) | ((GPIO_ReadOutputData(GPIOD)&0xA200)<0xA200)\
		| ((GPIO_ReadOutputData(GPIOE)&0x780)<0x780) | ((GPIO_ReadOutputData(GPIOA)&0x1501)<0x1501)\
		| ((GPIO_ReadOutputData(GPIOC)&0xE940)<0xE940) | ((GPIO_ReadOutputData(GPIOD)&0x5510)<0x5510)\
		| ((GPIO_ReadOutputData(GPIOE)&0x007C)<0x007C) ))
	{
		MotorRunning = 0;
	}
	else MotorRunning = 1;
	return MotorRunning;
}
/*  //process of read Row output
	LineRight_1 -- PC11		
	LineRight_2 -- PD4
	LineRight_3 -- PE3
	LineRight_4 -- PE2
	LineRight_5 -- PE5
	LineRight_6 -- PE4
	LineRight_7 -- PC13
	LineRight_8 -- PE6
	LineRight_9 -- PC15
	LineRight_10 -- PC14
	LineRight_11 -- PA0

	RankContrary_1 -- PD8-
	RankContrary_2 -- PD10-
	RankContrary_3 -- PD12-
	RankContrary_4 -- PD14-
	RankContrary_5 -- PC6-
	RankContrary_6 -- PC8-
	RankContrary_7 -- PA8-
	RankContrary_8 -- PA10-
	RankContrary_9 -- PA12-
	RankContrary_10 -- P7 of I2C addressed at 0x42
	RankContrary_11 -- P6 of I2C addressed at 0x42(第一版电机反转的电源跟地标反了)
	
	A 0 8 10 12-->低位 1000 0000 1010 1000--> 高位 0001 0101 0000 0001--> 0x1501
	B -->0
	C 11 13 15 14 6 8 -->0000 0010 1001 0111-->1110 1001 0100 0000-->0xE940
	D 4 8 10 12 14-->0000 1000 1010 1010-->0101 0101 0001 0000-->0x5510
	E 3 2 5 4 6 -->0011 1110 0000 0000-->0000 0000 0111 1100-->0x007C
*/

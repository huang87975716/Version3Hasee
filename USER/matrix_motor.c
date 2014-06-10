#include "matrix_motor.h"
#include "stm32f10x.h"
#include "PCF8574_I2C.h"
#include "SysTick.h"
#include "usart.h"

extern volatile u16 ADC_ConvertedValue[11];
extern unsigned char I2CValue2;

/**/
unsigned char MotorDrive(int Dir, int Row, int Col)
{
	unsigned char i;
	for (i = 0; i < 7; i++ )
	{
		MotorRowDrive (Dir , Row);
		MotorColDrive (Dir , Col);
		
		Delay_us(150);
		if (ADC_ConvertedValue[Col-1] > 1540) //1540/4096*2.66=1A
		{
			MotorColStop(Col);
			Delay_us(750);				
		}
		else break;	
	}
	Delay_us(10);//just for safety, sometimes two cols need to be start together
	return i;
}
/**/
void MotorDriveResverse(int Row)
	{
		unsigned char i;
		MotorRowDrive(0,Row);
		for (i = 1; i <= 11; i++ )
		{
		MotorColDrive(0,i);//
		Delay_us(200);
		}
	}
/**/
void MotorStopAll(void)
{
	unsigned char i;
	for (i = 1; i <= 11; i++)
	{
	MotorRowStop(i);
	MotorColStop(i);
	}
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
		if(! (((GPIO_ReadOutputData(GPIOA)&0x8A10)<0x8A10) | ((GPIO_ReadOutputData(GPIOB)&0xAC00)<0xAC00)\
		| ((GPIO_ReadOutputData(GPIOC)&0x1680)<0x1680) | ((GPIO_ReadOutputData(GPIOD)&0xA000)<0xA000)\
		| ((GPIO_ReadOutputData(GPIOE)&0x780)<0x780)) )
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
				I2CValue2 &= 0x7F;
				I2C_PCF8574_BufferWrite(I2CValue2, 0x42 );//Row 10th
				GPIO_SetBits(GPIOC, GPIO_Pin_14);
			break;
		case 11:
				I2CValue2 &=0xBF;
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
						I2C_PCF8574_BufferWrite(I2CValue2, 0x42 );//Row 10th
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

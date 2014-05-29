#include "PtoEtcSW_Check.h"
#include "stm32f10x.h"
#include "PCF8574_I2C.h"
#include "SysTick.h"


static unsigned int i;
static int PtoEtcSWStatus;
	
void StartAllPtoEtcSW(void);
void OnlyStop_PtoEtcSW(int ColValue);

/*
 *name:ELSCheck
 *description: check for photoelectric switches	
 *input: none
 *output: PtoEtcSWStatus
 *call relationship: extern
 */
unsigned short int ELSCheck(void)
{
	 for(i = 1; i <= 11;i++)
	{
		StartAllPtoEtcSW();
		OnlyStop_PtoEtcSW(i);
		Delay_us(100);//it should be modefied according to the reality.
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)) PtoEtcSWStatus |= (1<<i);
		else PtoEtcSWStatus &= ( ~(1<<i) );
	}
	return PtoEtcSWStatus;
}
/*
 *name:StartAllPtoEtcSW
 *description: open all photoelectric SW
 *input: none
 *output: none
 *call relationship: intern 
 */	

void StartAllPtoEtcSW(void)
{
	/*Start all photoelectric switches*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9);
	GPIO_ResetBits(GPIOD,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	GPIO_ResetBits(GPIOE,GPIO_Pin_0 | GPIO_Pin_1);
	I2CValue2 &= 0xDF;
	I2C_PCF8574_BufferWrite(I2CValue2,0x42);
}
/*
 *name:OnlyStop_PtoEtcSW
 *description: close one of the PhtEtcSW
 *input: ColValue
 *output: none
 *call relationship: intern
 */
void OnlyStop_PtoEtcSW(int ColValue)
{
switch (ColValue)
	{
		case 1:
			GPIO_SetBits(GPIOD,GPIO_Pin_6);
			break;
		case 2:
			GPIO_SetBits(GPIOD,GPIO_Pin_5);
			break;
		case 3:
			GPIO_SetBits(GPIOB,GPIO_Pin_3);
			break;
		case 4:
			GPIO_SetBits(GPIOD,GPIO_Pin_7);
			break;
		case 5:
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
			break;
		case 6:
			GPIO_SetBits(GPIOB,GPIO_Pin_4);
			break;
		case 7:
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
			break;
		case 8:
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
			break;
		case 9:
			GPIO_SetBits(GPIOE,GPIO_Pin_1);
			break;
		case 10:
			GPIO_SetBits(GPIOE,GPIO_Pin_0);
			break;
		case 11:
			I2CValue2 |= 0x20;
			I2C_PCF8574_BufferWrite(I2CValue2,0x42);
			break;
		default:
			break;
	}
}

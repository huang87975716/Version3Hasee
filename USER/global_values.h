#ifndef __GLOBAL_VALUES_H__
#define __GLOBAL_VALUES_H__
#include "stm32f10x.h"
#include "protocol.h"
	unsigned int i = 0, j = 0;	//used for for loop and should be initialled

	// 局部变量，用于存从flash读到的电压值			 
	float ADC_ConvertedValueLocal; 
	
	unsigned char I2CValue1 = 0xFF;//I2C initial value addressed 0x40
	unsigned char I2CValue2 = 0xEF;//I2c initial value addressed 0x42, for StartLED is connected to the p4 pin;	
	unsigned char IDOfPCB = 0;
	unsigned char RunMode = 0;
	unsigned char I2CStatus = 0;
	unsigned int 	PtoEtcSWCheckResult = 0;
	
	PROTOCOL_t gU2RecvBuff;//usart information
	
	#define MotorForward 		0x03 
	#define MotorBackward 	0x04
	
	unsigned int motor_col = 0;
	unsigned int motor_row = 0;
	unsigned int MeanRunningCurrent = 0;
#endif

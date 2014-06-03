#ifndef __GLOBAL_VALUES_H__
#define __GLOBAL_VALUES_H__
#include "stm32f10x.h"
#include "protocol.h"
	unsigned int i = 0, j = 0;	//used for for loop and should be initialled

	// 局部变量，用于存从flash读到的电压值			 
	float ADC_ConvertedValueLocal; 
	
	unsigned char 	I2CValue1 = 0xFF;//I2C initial value addressed 0x40
	unsigned char 	I2CValue2 = 0xEF;//I2c initial value addressed 0x42, for StartLED is connected to the p4 pin;	
	unsigned char 	IDOfPCB = 0;
	unsigned char 	RunMode = 0;
	unsigned char 	I2CStatus = 0;
	unsigned int 	PtoEtcSWCheckResult = 0;
	
	PROTOCOL_t gU2RecvBuff;//usart information
	

	#define UploadPCBID 		0x01 		//upload PCB ID 
	#define CheckPtoElcSW		0x02		//check photoelectric switch
	#define MotorForward 		0x03 		
	#define MotorBackward 	0x04
	#define	StopAllMotor		0x05		//stop all motor
	#define ReverseRowMotor 0x06
	#define StartPtoEtcSW		0x07		//start photoelectric switch
	#define StopPtoEtcSW		0x08		//stop photoelectric switch
	#define CheckPtoEtcSW 	0x09
	#define StartLED				0x0A
	#define StopLED					0x0B	
	
	#define ResultOfPtoEtcSW	0x22
	
	unsigned int motor_col = 0;
	unsigned int motor_row = 0;
	unsigned int MeanRunningCurrent = 0;
	
	unsigned char HandShakeToMaster[7] = {0xAA, 0xBB, 00, 00, 00, 00, 00};
	//unsigned char StarLEDBack[7] = {0xAA, 0x....};
	//unsigned char StopLEDBack[7] = {};
	
	volatile u32 step_timer2;
	unsigned char WaitPtoEtcSW = 0;
#endif

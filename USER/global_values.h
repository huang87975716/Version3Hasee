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
	unsigned int 		PtoEtcSWCheckResult = 0;
	
	PROTOCOL_t gU2RecvBuff;//usart information
	

	#define UploadPCBID 		0x01 		//upload PCB ID 
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
	
	unsigned char HandShakeToMaster[7] = 			{0xAA, 0xBB, 00, 00, 00, 00, 0x65};//
	unsigned char PCBID[7] = 						{0xAA, 0x01, 00, 00, 00, 00, 0xAB};//
	unsigned char MedecineFailed[7] = 				{0xAA, 0x02, 00, 00, 00, 00, 0xAC};//
	unsigned char MedecineSuccedded[7] = 			{0xAA, 0x03, 00, 00, 00, 00, 0xAD};//
	unsigned char StarLEDSuccedded[7] = 			{0xAA, 0x04, 00, 00, 00, 00, 0xAE};//
	unsigned char StopLEDSuccedded[7] = 			{0xAA, 0x05, 00, 00, 00, 00, 0xAF};//
	unsigned char StartPtoEtcSWSuccedded[7]= 		{0xAA, 0x06, 00, 00, 00, 00, 0xB0};//
	unsigned char StopPtoEtcSWSuccedded[7] = 		{0xAA, 0x07, 00, 00, 00, 00, 0xB1};//
	unsigned char PtoEtcSWCheckResInfo[7] = 		{0xAA, 0x08, 00, 00, 00, 00, 0xB2};//
	unsigned char AllMotorStopped[7] = 				{0xAA, 0x09, 00, 00, 00, 00, 0xB3};//
	unsigned char ColCurrentOverRange[7] = 			{0xAA, 0x0A, 00, 00, 00, 00, 0xB4};//
	unsigned char MotorStartInfo[7] = 				{0xAA, 0x0B, 00, 00, 00, 00, 0xB5};//
	
	
	
	
	volatile u32 step_timer2;
	unsigned char WaitPtoEtcSW = 0;
	
	typedef enum
	{
		MotorStoppedTop = 1,
		KeyPushed,
		MotorStartDown,
		DownLimSW,
		MotorStoppedBottom,
		InfraredSensorStatus,
		TimerStarted,
		TimerTerminated,
		MotorstartUp,
		UpLimSW,
	}TchScrSltStatus_TypeDef;
	TchScrSltStatus_TypeDef TchScrSltStatus = MotorStoppedTop;
#endif

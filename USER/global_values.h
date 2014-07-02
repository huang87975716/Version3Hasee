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
	unsigned char 	I2CTouchKey = 0;
	unsigned char 	I2CInfaraedSsr = 0;
	unsigned int 		PtoEtcSWCheckResult = 0;
	
	PROTOCOL_t gU2RecvBuff;//usart information
	
	#define RunModeNormal 	0x00
	#define RunModeTest 		0x0A
	#define LimitCurrent 		3696 	// 3234/3/4096*2.66 = 0.7A
																// 3696/3/4096*2.66 = 0.8A
	#define	DownLimSWCheck		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)
	#define UpLimSWCheck 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)
	
	#define UploadPCBID 		0x01 		//
	#define MotorForward 		0x03 		//
	#define MotorBackward 		0x04		//
	#define	StopAllMotor		0x05		//
	#define ReverseRowMotor 	0x06        //not available
	#define StartPtoEtcSW		0x07		//
	#define StopPtoEtcSW		0x08		//
	#define CheckPtoEtcSW 		0x09
	#define StartLED			0x0A        //
	#define StopLED				0x0B	    //
	#define ReadI2C2			0x0C
	#define ReadOptCplr			0x0D
	#define L298Driver			0x0E
	#define ShelterUpLimitSW 	0x0F
	#define ShelterDownLimitSW 	0x10
	
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
	unsigned char ShelterOpened[7] =				{0xAA, 0x0C, 00, 00, 00, 00, 0xB6};//
	unsigned char ShelterClosed[7] =				{0xAA, 0x0D, 00, 00, 00, 00, 0xB7};//
	
	unsigned int motor_col = 0;
	unsigned int motor_row = 0;
	unsigned int MeanRunningCurrent = 0;
		
	volatile u32 step_timer2;
	volatile u32 step_timer3;
	unsigned char WaitPtoEtcSW = 0;
	unsigned int DelayTimeOfTimer2 = 1000; 
	unsigned char ShelterUp = 0;
	unsigned char ShelterDown = 0;
	
	typedef enum
	{
		MotorStoppedTop = 1,
		KeyPushed,
		MotorStartDown,
		DownLimSW,
		MotorStoppedBottom,
		InfraredSensorFirst,
		InfraredSensorSecond,
		TimerTerminated,
		MotorstartUp,
		UpLimSW,
	}TchScrSltStatus_TypeDef;
	TchScrSltStatus_TypeDef TchScrSltStatus = MotorStoppedTop;
	
	
	typedef enum
	{
		Stopped = 0,
		Forward = 1,
		Backward = 2,
	}MotorStatus_TypeDef;
	
	MotorStatus_TypeDef MotorStatus = Stopped;
#endif

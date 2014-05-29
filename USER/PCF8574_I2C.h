#ifndef __PCF8574_H__
#define __PCF8574_H__
	#include "stm32f10x.h"
	#include <stdbool.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
	#define I2C_Speed   	100000
	#define SCL_H         	GPIOB->BSRR = GPIO_Pin_7
	#define SCL_L         	GPIOB->BRR  = GPIO_Pin_7   
	#define SDA_H         	GPIOB->BSRR = GPIO_Pin_6
	#define SDA_L         	GPIOB->BRR  = GPIO_Pin_6
	#define SCL_read      	GPIOB->IDR  & GPIO_Pin_7
	#define SDA_read      	GPIOB->IDR  & GPIO_Pin_6
	#define TRUE 1;
	#define FALSE 0;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

bool I2C_PCF8574_BufferWrite(u8 Buffer, u8 PCF8574_ADDRESS);
bool I2C_PCF8574_BufferRead(u8* pBuffer, u8 PCF8574_ADDRESS);
void SoftwareI2CConfig(void);
void SoftwareI2CGPIOConfig(unsigned char I2CValue1, unsigned char I2CValue2);

//由于电路图设计错误，PB6连接到了SDA，PB7连接了SCL，以下代码被注释，该用软件模拟I2C
// 	void I2C_PCF8574_Init(void);
// 	void I2C_PCF8574_ByteWrite(uint8_t WriteBuffer, uint8_t PCF8574_ADDRESS);
// 	uint8_t I2C_PCF8574_ByteRead(uint8_t PCF8574_ADDRESS);
// 	void I2C_PCF8574_WaitPCF8574StandbyState(uint8_t PCF8574_ADDRESS);
//由于电路图设计错误，PB6连接到了SDA，PB7连接了SCL，以上代码被注释，该用软件模拟I2C
#endif

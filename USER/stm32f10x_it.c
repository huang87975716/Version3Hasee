/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "protocol.h"
extern void TimingDelay_Decrement(void);
extern PROTOCOL_t gU2RecvBuff;
#define USART_REC_LEN  			200  	//定义最大接收字节数 200

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();	
}


void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line15) != RESET) //确保是否产生了EXTI Line中断
  {
		//do something
    EXTI_ClearITPendingBit(EXTI_Line15);     //清除中断标志位
  }  
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/********************************************************************************
 * FunctionName: USART2_IRQHandler
 *
 * Description : 串口2的接收中断
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
void USART2_IRQHandler(void)
{
	static uint8_t u2StaMach = 0;
	uint8_t u2RecvCh = 0;               // 存储接收到的字符
	PROTOCOL_t *u2p = &gU2RecvBuff;  		// 指示缓存地址
	unsigned char i = 0;
	unsigned char InfoBackForMaster[7] = {0xAA, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00};
	
  // 如果存在数据
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		// 读取USART2_DR寄存器会自动清空该标志位
		u2RecvCh = (USART2->DR) & 0xFF;

		// 状态机推进
		if (u2StaMach == 0)
		{
			if (u2RecvCh == 0xAA)
			{
					u2p->header = u2RecvCh;
					u2StaMach = 1;
					u2p->checksum = u2RecvCh;
			}
		}
		else if (u2StaMach==1)
		{
			u2p->command=u2RecvCh;
			u2StaMach=2;
			u2p->checksum += u2RecvCh;
		}
		else if (u2StaMach==2)
		{
			u2p->data[u2p->dataIndex++] = u2RecvCh;
			u2p->checksum += u2RecvCh;
			if (u2p->dataIndex == 4)
			{
				u2StaMach = 3;
			}
		}
		else if (u2StaMach == 3)
		{
			// 判断校验和
			if (u2p->checksum == u2RecvCh)
			{
				for( i=0; i<7;i++) USART_SendData(USART2, InfoBackForMaster[i]);//send AABB00...to MasterPC
				u2p->protocol_ok = 1;
			}
			u2StaMach = 0;
			u2p->dataIndex =0;
		}
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);  
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

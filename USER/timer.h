#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f10x.h"

#define START_TIME2  step_timer2=0;RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME2  TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)
#define START_TIME3  step_timer3=0;RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);TIM_Cmd(TIM3, ENABLE)
#define STOP_TIME3  TIM_Cmd(TIM3, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE)

void TIM2_NVIC_Configuration(void);
void TIM2_Configuration(void);
void TIM3_NVIC_Configuration(void);
void TIM3_Configuration(void);

#endif	/* TIME_TEST_H */

#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"
void ADC1_Config(void);
// ADC1ת���ĵ�ѹֵͨ��DMA��ʽ����flash
extern volatile u16 ADC_ConvertedValue[11];

#endif /* __ADC_H */


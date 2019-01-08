#ifndef __TIMER_H__
#define __TIMER_H__



#include "stm32h7xx_hal.h"
#include "usart.h"

void TIM3_Init(unsigned short arr,unsigned short psc);
void TIM5_Init(unsigned short arr,unsigned short psc);

#endif 

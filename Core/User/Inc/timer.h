#ifndef __TIMER_H__
#define __TIMER_H__



#include "stm32h7xx_hal.h"
#include "usart.h"
#include "lcd.h"
#include "fes.h"

void TIM3_Init(unsigned short arr,unsigned short psc);
void TIM4_Init(unsigned short arr,unsigned short psc);
void TIM5_Init(unsigned short arr,unsigned short psc);
void ChangePeriod(u16 period);
#endif 

#ifndef __VISUALSCOPE_H__
#define __VISUALSCOPE_H__


#include "stm32h7xx_hal.h"


typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;



//ͨ�����ڷ���VisualScopeʶ�������
void VisualScope		(UART_HandleTypeDef *huart,int16_t CH1,int16_t CH2,int16_t CH3,int16_t CH4);



#endif

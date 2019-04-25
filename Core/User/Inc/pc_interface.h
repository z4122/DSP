#ifndef __PC_INTERFACE__
#define __PC_INTERFACE__

#include "stm32h7xx_hal.h"
#include "usart.h"

void TransferData2PC(void);
extern long double ldVolutage[10];//存储十个通道的值


#endif

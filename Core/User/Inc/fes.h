#ifndef _FES_H_
#define _FES_H_



#include "stm32h7xx_hal.h"
#include "usart.h"
#include "time.h"


extern float value[17][2];




typedef struct {
	volatile u8 HEADER;
	volatile u8 CONTROL;
	volatile u8 CHANNEL;
	volatile u16 LENGTH;
	volatile u16 WIDTH;
	volatile u16 FREQUENCY;
	volatile u8 DELAY;
	volatile u8 CIRCULATE;
	volatile u16 TIMES;
	volatile u8 NUMERATOR;
	volatile u16 DENOMINATOR;
	volatile u8 AMPLITUDE;
	volatile u8 ENDFLAG;
} configtx;


void merge_stimulate_parameter(UART_HandleTypeDef *huart,int amplitude);
void stim_search(UART_HandleTypeDef *huart);
void stim_start(UART_HandleTypeDef *huart);
void stim_stop(UART_HandleTypeDef *huart);
void stimulate(UART_HandleTypeDef *huart,float amp);

#endif

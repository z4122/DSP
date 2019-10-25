#ifndef _FES_H_
#define _FES_H_



#include "stm32h7xx_hal.h"
#include "usart.h"
#include "time.h"
#include "ad.h"
#include "math.h"


enum Mode {zzz = 0,zj = 1};

extern float value[17][2];
extern enum Mode name;
enum Hand {left=0,right=1};
extern enum Hand amputatedHand;


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


void merge_stimulate_parameter(UART_HandleTypeDef *huart,double amplitude,int channel);
void merge_stimulate_parameter_freerun(UART_HandleTypeDef *huart,int channel);
void stim_search(UART_HandleTypeDef *huart);
void stim_start(UART_HandleTypeDef *huart);
void stim_stop(UART_HandleTypeDef *huart);
void stimulate(UART_HandleTypeDef *huart,double amplitude,int channel);
void stimulateFreeRun(UART_HandleTypeDef *huart,int channel);
void stimulateDSU(void);
void FreeRun_Init(void);
void PCFollow_Init(void);

#endif

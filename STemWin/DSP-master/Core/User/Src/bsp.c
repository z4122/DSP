#include "bsp.h"



void BSP_Init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE(); 
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
	__HAL_RCC_GPIOJ_CLK_ENABLE();
	__HAL_RCC_GPIOK_CLK_ENABLE();
	
	
//	Reset_Init();
	AD_Init();
	USART1_Init();//ch1
	USART3_Init();//ch2
	UART4_Init();//ch3·
	UART5_Init();//ch4
	UART7_Init();//ch5
	UART8_Init();//加入8之后，屏幕会闪烁
	ADC_Init();
	TIM3_Init(500-1,20000-1);//单位ms，用于与DSSU通信，优先级高 200M/20000 = 10k 1/10k = 0.1ms 500*0.1ms=50ms
	TIM5_Init(1000-1,20000-1);//单位ms，用于AD数据转换,优先级低 100ms

	//USART2_Init();暂时用不到
	//USART6_Init();暂时用不到
	
	

	
}




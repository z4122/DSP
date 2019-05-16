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
	
	//错开与定时器3的执行时间
	TIM5_Init(1000-1,20000-1);//AD采样的定时器，优先级低，100ms

	//Reset_Init();
	AD_Init();
	USART1_Init();//ch1
	USART3_Init();//ch2
	UART4_Init();//ch3
	UART5_Init();//ch4
	UART7_Init();//ch5
	UART8_Init();//与PC通信的串口
	ADC_Init();

	TIM3_Init(500-1,20000-1);//与DSU通信的定时器，优先级高，400M/2/20000 = 10k 1/10k = 0.1ms 500*0.1ms=50ms
	

	//USART2_Init();暂时用不到
	//USART6_Init();暂时用不到
	
	

	
}




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
	//AD_Init();
	//USART1_Init();
	//USART3_Init();
	//UART4_Init();
	//UART5_Init();
	//UART7_Init();
	UART8_Init();//����8֮����Ļ����˸
	ADC_Init();
	//USART2_Init();��ʱ�ò���
	//USART6_Init();��ʱ�ò���
	
	

	
}




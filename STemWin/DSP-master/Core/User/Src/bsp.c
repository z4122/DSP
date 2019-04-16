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
	UART4_Init();//ch3��
	UART5_Init();//ch4
	UART7_Init();//ch5
	UART8_Init();//����8֮����Ļ����˸
	ADC_Init();
	TIM3_Init(500-1,20000-1);//��λms��������DSSUͨ�ţ����ȼ��� 200M/20000 = 10k 1/10k = 0.1ms 500*0.1ms=50ms
	TIM5_Init(1000-1,20000-1);//��λms������AD����ת��,���ȼ��� 100ms

	//USART2_Init();��ʱ�ò���
	//USART6_Init();��ʱ�ò���
	
	

	
}




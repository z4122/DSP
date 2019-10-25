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
	
	USART1_Init();//ch1
	USART3_Init();//ch2
	UART4_Init();//ch3
	UART5_Init();//ch4
	UART7_Init();//ch5
	UART8_Init();//与PC通信的串口
	TIM4_Init(5000-1,20000-1);//显示状态的定时器，优先级低
	LCDFontInit(); 
	//等待上位机的串口选择了正确的模式
	GUI_DispStringAt("Please select work mode",160,270);
	while(initMode==0);

	if(initMode==1){
		//跟随模式
		//错开与定时器3的执行时间
		AD_Init();
		ADC_Init();
		TIM3_Init(100-1,20000-1);//通用定时器，优先级高，400M/2/20000 = 10k 1/10k = 0.1ms 100*0.1ms=10ms
	}else if(initMode==2){
		//自由测试模式
		FreeRun_Init();
	}else if(initMode==3){
		//接受从PC端传过来的压力数据。
		PCFollow_Init();
		TIM3_Init(100-1,20000-1);//通用定时器，优先级高，400M/2/20000 = 10k 1/10k = 0.1ms 100*0.1ms=10ms
	}

	//USART2_Init();暂时用不到
	//USART6_Init();暂时用不到	
}




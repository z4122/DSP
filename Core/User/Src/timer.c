#include "timer.h"

extern void MainLoop(void);//在主函数中定义
extern void GetAdData(void);//在主函数中定义，调用AD采样模块
extern void TransferData2PC(void);//在主函数中定义，传输到PC端

TIM_HandleTypeDef TIM3_Handler;
TIM_HandleTypeDef TIM4_Handler;
TIM_HandleTypeDef TIM5_Handler;

//定时器周期 Tout = ((arr+1)*(psc+1))/Ft  us
//Ft代表定时器工作频率200MHZ
void TIM3_Init(unsigned short arr,unsigned short psc)
{
	TIM3_Handler.Instance=TIM3;  //定时器3
	TIM3_Handler.Init.Prescaler=psc; //分频系数
	TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;  //向上计数器
	TIM3_Handler.Init.Period=arr;  //自动装载值
	TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; //时钟分频因子
	HAL_TIM_Base_Init(&TIM3_Handler);  //初始化定时器3
	HAL_TIM_Base_Start_IT(&TIM3_Handler);  //使能定时器3和定时器3更新中断
}

void TIM4_Init(unsigned short arr,unsigned short psc)
{
	TIM4_Handler.Instance=TIM4;  //定时器4
	TIM4_Handler.Init.Prescaler=psc; //分频系数
	TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;  //向上计数器
	TIM4_Handler.Init.Period=arr;  //自动装载值
	TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; //时钟分频因子
	HAL_TIM_Base_Init(&TIM4_Handler);  //初始化定时器4
	
	HAL_TIM_Base_Start_IT(&TIM4_Handler);  //使能定时器4和定时器4更新中断
	
}

void TIM5_Init(unsigned short arr,unsigned short psc)
{
	TIM5_Handler.Instance=TIM5;  //定时器5
	TIM5_Handler.Init.Prescaler=psc; //分频系数
	TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;  //向上计数器
	TIM5_Handler.Init.Period=arr;  //自动装载值
	TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; //时钟分频因子
	HAL_TIM_Base_Init(&TIM5_Handler);  //初始化定时器5
	
	HAL_TIM_Base_Start_IT(&TIM5_Handler);  //使能定时器5和定时器5更新中断
	
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3_Handler.Instance)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();  //使能TIM3中断
		HAL_NVIC_SetPriority(TIM3_IRQn,1,0); 
		HAL_NVIC_EnableIRQ(TIM3_IRQn); 
	}
	if(htim->Instance==TIM4_Handler.Instance)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();  //使能TIM4中断
		HAL_NVIC_SetPriority(TIM4_IRQn,3,0);//优先级低，能够被定时器3中断嵌套，注意一开始的优先级设置。
		HAL_NVIC_EnableIRQ(TIM4_IRQn); 
	}
	if(htim->Instance==TIM5_Handler.Instance)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();  //使能TIM5中断
		HAL_NVIC_SetPriority(TIM5_IRQn,2,0);//优先级低，能够被定时器3中断嵌套，注意一开始的优先级设置。
		HAL_NVIC_EnableIRQ(TIM5_IRQn); 
	}
}

void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM3_Handler);
}

void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM4_Handler);
}

void TIM5_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM5_Handler);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3_Handler.Instance)
	{
		MainLoop();
	}
	else if(htim->Instance==TIM4_Handler.Instance)
	{
		DisplayRunning();
	}
	else if(htim->Instance==TIM5_Handler.Instance)
	{
		
		TransferData2PC();
	}
	
	
}

//改变定时器3的定时周期，单位为ms
void ChangePeriod(u16 period){
	TIM3_Handler.Init.Period=period*10-1;  //自动装载值
	HAL_TIM_Base_Init(&TIM3_Handler);  //初始化定时器3
}


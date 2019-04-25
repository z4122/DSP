#include "timer.h"

extern void MainLoop(void);//���������ж���
extern void GetAdData(void);//���������ж��壬����AD����ģ��
extern void TransferData2PC(void);//���������ж��壬�������ݵ�PC��

TIM_HandleTypeDef TIM3_Handler;
TIM_HandleTypeDef TIM5_Handler;

//��ʱ������ Tout = ((arr+1)*(psc+1))/Ft  us
//Ft����ʱ������Ƶ�� 200MHZ
void TIM3_Init(unsigned short arr,unsigned short psc)
{
	TIM3_Handler.Instance=TIM3;  //��ʱ��3
	TIM3_Handler.Init.Prescaler=psc; //��Ƶϵ��
	TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;  //���ϼ�����
	TIM3_Handler.Init.Period=arr;  //�Զ�װ��ֵ
	TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; //ʱ�ӷ�Ƶ����
	HAL_TIM_Base_Init(&TIM3_Handler);  //��ʼ����ʱ��3
	HAL_TIM_Base_Start_IT(&TIM3_Handler);  //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����ж�
}

void TIM5_Init(unsigned short arr,unsigned short psc)
{
	TIM5_Handler.Instance=TIM5;  //��ʱ��5
	TIM5_Handler.Init.Prescaler=psc; //��Ƶϵ��
	TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;  //���ϼ�����
	TIM5_Handler.Init.Period=arr;  //�Զ�װ��ֵ
	TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; //ʱ�ӷ�Ƶ����
	HAL_TIM_Base_Init(&TIM5_Handler);  //��ʼ����ʱ��5
	
	HAL_TIM_Base_Start_IT(&TIM5_Handler);  //ʹ�ܶ�ʱ��5�Ͷ�ʱ��5�����ж�
	
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3_Handler.Instance)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();  //ʹ��TIM3�ж�
		HAL_NVIC_SetPriority(TIM3_IRQn,1,0); 
		HAL_NVIC_EnableIRQ(TIM3_IRQn); 
	}
	if(htim->Instance==TIM5_Handler.Instance)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();  //ʹ��TIM4�ж�
		HAL_NVIC_SetPriority(TIM5_IRQn,2,0);//���ȼ��ͣ��ܹ�����ʱ��3�ж�Ƕ�ס� ע����һ��ʼ�����ȼ�����
		HAL_NVIC_EnableIRQ(TIM5_IRQn); 
	}
}

void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM3_Handler);
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
	else if(htim->Instance==TIM5_Handler.Instance)
	{
		
		TransferData2PC();
	}
	
}


//改变定时器3的定时周期，单位为ms
void ChangePeriod(u16 period){
	TIM3_Handler.Init.Period=period*10;  //�Զ�װ��ֵ
	HAL_TIM_Base_Init(&TIM3_Handler);  //��ʼ����ʱ��3
}


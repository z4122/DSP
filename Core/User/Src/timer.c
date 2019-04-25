#include "timer.h"

extern void MainLoop(void);//ÔÚÖ÷º¯ÊıÖĞ¶¨Òå
extern void GetAdData(void);//ÔÚÖ÷º¯ÊıÖĞ¶¨Òå£¬µ÷ÓÃAD²ÉÑùÄ£¿é
extern void TransferData2PC(void);//ÔÚÖ÷º¯ÊıÖĞ¶¨Òå£¬´«ÊäÊı¾İµ½PC¶Ë

TIM_HandleTypeDef TIM3_Handler;
TIM_HandleTypeDef TIM5_Handler;

//¶¨Ê±Æ÷ÖÜÆÚ Tout = ((arr+1)*(psc+1))/Ft  us
//Ft´ú±í¶¨Ê±Æ÷¹¤×÷ÆµÂÊ 200MHZ
void TIM3_Init(unsigned short arr,unsigned short psc)
{
	TIM3_Handler.Instance=TIM3;  //¶¨Ê±Æ÷3
	TIM3_Handler.Init.Prescaler=psc; //·ÖÆµÏµÊı
	TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;  //ÏòÉÏ¼ÆÊıÆ÷
	TIM3_Handler.Init.Period=arr;  //×Ô¶¯×°ÔØÖµ
	TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; //Ê±ÖÓ·ÖÆµÒò×Ó
	HAL_TIM_Base_Init(&TIM3_Handler);  //³õÊ¼»¯¶¨Ê±Æ÷3
	HAL_TIM_Base_Start_IT(&TIM3_Handler);  //Ê¹ÄÜ¶¨Ê±Æ÷3ºÍ¶¨Ê±Æ÷3¸üĞÂÖĞ¶Ï
}

void TIM5_Init(unsigned short arr,unsigned short psc)
{
	TIM5_Handler.Instance=TIM5;  //¶¨Ê±Æ÷5
	TIM5_Handler.Init.Prescaler=psc; //·ÖÆµÏµÊı
	TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;  //ÏòÉÏ¼ÆÊıÆ÷
	TIM5_Handler.Init.Period=arr;  //×Ô¶¯×°ÔØÖµ
	TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; //Ê±ÖÓ·ÖÆµÒò×Ó
	HAL_TIM_Base_Init(&TIM5_Handler);  //³õÊ¼»¯¶¨Ê±Æ÷5
	
	HAL_TIM_Base_Start_IT(&TIM5_Handler);  //Ê¹ÄÜ¶¨Ê±Æ÷5ºÍ¶¨Ê±Æ÷5¸üĞÂÖĞ¶Ï
	
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3_Handler.Instance)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();  //Ê¹ÄÜTIM3ÖĞ¶Ï
		HAL_NVIC_SetPriority(TIM3_IRQn,1,0); 
		HAL_NVIC_EnableIRQ(TIM3_IRQn); 
	}
	if(htim->Instance==TIM5_Handler.Instance)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();  //Ê¹ÄÜTIM4ÖĞ¶Ï
		HAL_NVIC_SetPriority(TIM5_IRQn,2,0);//ÓÅÏÈ¼¶µÍ£¬ÄÜ¹»±»¶¨Ê±Æ÷3ÖĞ¶ÏÇ¶Ì×¡£ ×¢Òâ×îÒ»¿ªÊ¼µÄÓÅÏÈ¼¶ÉèÖÃ
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


//æ”¹å˜å®šæ—¶å™¨3çš„å®šæ—¶å‘¨æœŸï¼Œå•ä½ä¸ºms
void ChangePeriod(u16 period){
	TIM3_Handler.Init.Period=period*10;  //×Ô¶¯×°ÔØÖµ
	HAL_TIM_Base_Init(&TIM3_Handler);  //³õÊ¼»¯¶¨Ê±Æ÷3
}


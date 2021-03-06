#include "usart.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart6;
UART_HandleTypeDef huart7;
UART_HandleTypeDef huart8;



int UART1RxBuffCount = 0;
int UART3RxBuffCount = 0;
int UART4RxBuffCount = 0;
int UART5RxBuffCount = 0;
int UART7RxBuffCount = 0;
int UART8RxBuffCount = 0;


uint8_t UART1RxBuff; 
uint8_t UART3RxBuff; 
uint8_t UART4RxBuff; 
uint8_t UART5RxBuff; 
uint8_t UART7RxBuff; 
uint8_t UART8RxBuff; 

//1 电流；2频率；3频率
u16 parameter[6][5]; //5个通道，为了对齐选了6
u16 threshold[6][6]; // 5个通道，为了对齐选了6
u8  channelEnableflag[6] = {0}; //5个通道，为了对齐选了6
float pressureLowerThreshold[6] = {0};//5个通道，为了对齐选了6，压力有效的下限
float pressureUpperThreshold[6] = {0};//5个通道，为了对齐选了6，压力有效的上限

int channelchange = 0;
int	initMode =0;//选择跟随模式还是自由测试模式，跟随模式=1，自由测试模式=2
int startFlag = 0;
uint8_t tempRxBuffer;
uint8_t upperRxBuffer[7];
int cnt = 0;
volatile int testmode_flag = 6;//通过修改此处可以修改启动的模式
int parameterTransmittedSuccess=0;



static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
	GUI_DispStringAt("usart error",100,20);

  while(1)
  {
  }
}


void Reset_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void USART1_Init(void)
{

  huart1.Instance 					= USART1;
  huart1.Init.BaudRate 			= 115200;
  huart1.Init.WordLength 		= UART_WORDLENGTH_8B;
  huart1.Init.StopBits 			= UART_STOPBITS_1;
  huart1.Init.Parity 				= UART_PARITY_NONE;
  huart1.Init.Mode 					= UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl			= UART_HWCONTROL_NONE;
  huart1.Init.OverSampling 	= UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
	
	if(HAL_UART_Receive_IT(&huart1,&UART1RxBuff, 1) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }

}

void USART2_Init(void)
{

  huart2.Instance 					= USART2;
  huart2.Init.BaudRate 			= 115200;
  huart2.Init.WordLength 		= UART_WORDLENGTH_8B;
  huart2.Init.StopBits 			= UART_STOPBITS_1;
  huart2.Init.Parity 				= UART_PARITY_NONE;
  huart2.Init.Mode 					= UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl			= UART_HWCONTROL_NONE;
  huart2.Init.OverSampling 	= UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}
void USART3_Init(void)
{

  huart3.Instance 					= USART3;
  huart3.Init.BaudRate 			= 115200;
  huart3.Init.WordLength 		= UART_WORDLENGTH_8B;
  huart3.Init.StopBits 			= UART_STOPBITS_1;
  huart3.Init.Parity 				= UART_PARITY_NONE;
  huart3.Init.Mode 					= UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl			= UART_HWCONTROL_NONE;
  huart3.Init.OverSampling 	= UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
	
	if(HAL_UART_Receive_IT(&huart3, &UART3RxBuff, 1) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }

}
void UART4_Init(void)
{

  huart4.Instance 					= UART4;
  huart4.Init.BaudRate 			= 115200;
  huart4.Init.WordLength 		= UART_WORDLENGTH_8B;
  huart4.Init.StopBits 			= UART_STOPBITS_1;
  huart4.Init.Parity 				= UART_PARITY_NONE;
  huart4.Init.Mode 					= UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl			= UART_HWCONTROL_NONE;
  huart4.Init.OverSampling 	= UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
	
	
	if(HAL_UART_Receive_IT(&huart4, &UART4RxBuff, 1) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }

}
void UART5_Init(void)
{

  huart5.Instance 					= UART5;
  huart5.Init.BaudRate 			= 115200;
  huart5.Init.WordLength 		= UART_WORDLENGTH_8B;
  huart5.Init.StopBits 			= UART_STOPBITS_1;
  huart5.Init.Parity 				= UART_PARITY_NONE;
  huart5.Init.Mode 					= UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl			= UART_HWCONTROL_NONE;
  huart5.Init.OverSampling 	= UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
	
	if(HAL_UART_Receive_IT(&huart5, &UART5RxBuff, 1) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }

}
void USART6_Init(void)
{

  huart6.Instance 					= USART6;
  huart6.Init.BaudRate 			= 115200;
  huart6.Init.WordLength 		= UART_WORDLENGTH_8B;
  huart6.Init.StopBits 			= UART_STOPBITS_1;
  huart6.Init.Parity 				= UART_PARITY_NONE;
  huart6.Init.Mode 					= UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl			= UART_HWCONTROL_NONE;
  huart6.Init.OverSampling 	= UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }

}

void UART7_Init(void)
{

  huart7.Instance 					= UART7;
  huart7.Init.BaudRate 			= 115200;
  huart7.Init.WordLength 		= UART_WORDLENGTH_8B;
  huart7.Init.StopBits 			= UART_STOPBITS_1;
  huart7.Init.Parity 				= UART_PARITY_NONE;
  huart7.Init.Mode 					= UART_MODE_TX_RX;
  huart7.Init.HwFlowCtl			= UART_HWCONTROL_NONE;
  huart7.Init.OverSampling 	= UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart7) != HAL_OK)
  {
    Error_Handler();
  }

	if(HAL_UART_Receive_IT(&huart7,&UART7RxBuff, 1) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }
}
void UART8_Init(void)
{

  huart8.Instance 					= UART8;
  huart8.Init.BaudRate 			= 115200;
  huart8.Init.WordLength 		= UART_WORDLENGTH_8B;
  huart8.Init.StopBits 			= UART_STOPBITS_1;
  huart8.Init.Parity 				= UART_PARITY_NONE;
  huart8.Init.Mode 					= UART_MODE_TX_RX;
  huart8.Init.HwFlowCtl			= UART_HWCONTROL_NONE;
  huart8.Init.OverSampling 	= UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart8) != HAL_OK)
  {
    Error_Handler();
  }
	
	if(HAL_UART_Receive_IT(&huart8, &UART8RxBuff, 1) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspInit 0 */

  /* USER CODE END UART4_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_UART4_CLK_ENABLE();
  
    /**UART4 GPIO Configuration    
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* UART4 interrupt Init */
    HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspInit 1 */

  /* USER CODE END UART4_MspInit 1 */
  }
  else if(huart->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspInit 0 */

  /* USER CODE END UART5_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_UART5_CLK_ENABLE();
  
    /**UART5 GPIO Configuration    
    PB12     ------> UART5_RX
    PB13     ------> UART5_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_UART5;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* UART5 interrupt Init */
    HAL_NVIC_SetPriority(UART5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
  /* USER CODE BEGIN UART5_MspInit 1 */

  /* USER CODE END UART5_MspInit 1 */
  }
  else if(huart->Instance==UART7)
  {
  /* USER CODE BEGIN UART7_MspInit 0 */

  /* USER CODE END UART7_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_UART7_CLK_ENABLE();
  
    /**UART7 GPIO Configuration    
    PF6     ------> UART7_TX
    PF7     ------> UART7_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_UART7;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

   

    /* UART7 interrupt Init */
    HAL_NVIC_SetPriority(UART7_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(UART7_IRQn);
  /* USER CODE BEGIN UART7_MspInit 1 */

  /* USER CODE END UART7_MspInit 1 */
  }
  else if(huart->Instance==UART8)
  {
  /* USER CODE BEGIN UART8_MspInit 0 */

  /* USER CODE END UART8_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_UART8_CLK_ENABLE();
  
    /**UART8 GPIO Configuration    
    PJ8     ------> UART8_TX
    PJ9     ------> UART8_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART8;
    HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

    /* UART8 interrupt Init */
    HAL_NVIC_SetPriority(UART8_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(UART8_IRQn);
  /* USER CODE BEGIN UART8_MspInit 1 */

  /* USER CODE END UART8_MspInit 1 */
  }
  else if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PB15     ------> USART1_RX
    PB14     ------> USART1_TX 
    */

		GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(huart->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    /**USART2 GPIO Configuration    
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(huart->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();
  
    /**USART3 GPIO Configuration    
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
  else if(huart->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspInit 0 */

  /* USER CODE END USART6_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART6_CLK_ENABLE();
  
    /**USART6 GPIO Configuration    
    PG9     ------> USART6_RX
    PG14     ------> USART6_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART6;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* USART6 interrupt Init */
    HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspInit 1 */

  /* USER CODE END USART6_MspInit 1 */
  }

}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

  if(huart->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspDeInit 0 */

  /* USER CODE END UART4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART4_CLK_DISABLE();
  
    /**UART4 GPIO Configuration    
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_11);

    /* UART4 interrupt DeInit */
    HAL_NVIC_DisableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspDeInit 1 */

  /* USER CODE END UART4_MspDeInit 1 */
  }
  else if(huart->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspDeInit 0 */

  /* USER CODE END UART5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART5_CLK_DISABLE();
  
    /**UART5 GPIO Configuration    
    PB12     ------> UART5_RX
    PB13     ------> UART5_TX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13);

    /* UART5 interrupt DeInit */
    HAL_NVIC_DisableIRQ(UART5_IRQn);
  /* USER CODE BEGIN UART5_MspDeInit 1 */

  /* USER CODE END UART5_MspDeInit 1 */
  }
  else if(huart->Instance==UART7)
  {
  /* USER CODE BEGIN UART7_MspDeInit 0 */

  /* USER CODE END UART7_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART7_CLK_DISABLE();
  
    /**UART7 GPIO Configuration    
    PF7     ------> UART7_TX
    PF6     ------> UART7_RX 
    */
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_6|GPIO_PIN_7);


    /* UART7 interrupt DeInit */
    HAL_NVIC_DisableIRQ(UART7_IRQn);
  /* USER CODE BEGIN UART7_MspDeInit 1 */

  /* USER CODE END UART7_MspDeInit 1 */
  }
  else if(huart->Instance==UART8)
  {
  /* USER CODE BEGIN UART8_MspDeInit 0 */

  /* USER CODE END UART8_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART8_CLK_DISABLE();
  
    /**UART8 GPIO Configuration    
    PJ8     ------> UART8_TX
    PJ9     ------> UART8_RX 
    */
    HAL_GPIO_DeInit(GPIOJ, GPIO_PIN_8|GPIO_PIN_9);

    /* UART8 interrupt DeInit */
    HAL_NVIC_DisableIRQ(UART8_IRQn);
  /* USER CODE BEGIN UART8_MspDeInit 1 */

  /* USER CODE END UART8_MspDeInit 1 */
  }
  else if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PB15     ------> USART1_RX
    PB14     ------> USART1_TX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_15|GPIO_PIN_14);

    /* USART1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(huart->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5|GPIO_PIN_6);

    /* USART2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(huart->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();
  
    /**USART3 GPIO Configuration    
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9);

    /* USART3 interrupt DeInit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
  else if(huart->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspDeInit 0 */

  /* USER CODE END USART6_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART6_CLK_DISABLE();
  
    /**USART6 GPIO Configuration    
    PG9     ------> USART6_RX
    PG14     ------> USART6_TX 
    */
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_9|GPIO_PIN_14);

    /* USART6 interrupt DeInit */
    HAL_NVIC_DisableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspDeInit 1 */

  /* USER CODE END USART6_MspDeInit 1 */
  }

}

///* USER CODE BEGIN 1 */
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
//{
//  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_SET);
//}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	//HAL_UAR	T_Transmit(&huart1,(uint8_t*)1,1,0xfff);
	
	if(UartHandle->Instance==USART1)
	{
		//HAL_UART_Receive_IT(UartHandle,(uint8_t *)UART1RxBuff,1);
		
	}
	
		if(UartHandle->Instance==USART3)
	{
		//HAL_UART_Receive_IT(&huart3,(uint8_t *)UART3RxBuff,1);
		
	}
	
		if(UartHandle->Instance==UART4)
	{
		//HAL_UART_Receive_IT(&huart4,(uint8_t *)UART4RxBuff,1);
		
	}
	
		if(UartHandle->Instance==UART5)
	{
		HAL_UART_Receive_IT(&huart5,&UART5RxBuff,1);
		
	}
	
		if(UartHandle->Instance==UART7)
	{
		HAL_UART_Receive_IT(&huart7,&UART7RxBuff,1);
		
	}
	
		if(UartHandle->Instance==UART8)
	{		
		HAL_UART_Receive_IT(&huart8,&UART8RxBuff,1);
		if(cnt==0){
				if(UART8RxBuff==0xFF){
					cnt++;
				}
				else
					cnt=0;
		}
		else if(cnt==1){
				if(UART8RxBuff==0xFF){
					cnt++;
				}
				else
					cnt=0;
		}
		else if(cnt<9&&cnt>=2){
			upperRxBuffer[cnt-2] = UART8RxBuff;
			cnt++;
			return;
		}
		else if(cnt==9){
			u8 sum = 0;
			cnt=0;
			for(int i=0; i<7; i++)
        sum+=upperRxBuffer[i];
			sum = sum + 0xff + 0xff;
			
			if(sum==UART8RxBuff){
				testmode_flag = upperRxBuffer[0];
				parameterTransmittedSuccess = 1;//与屏幕显示相配合，做提示用
				switch (upperRxBuffer[0]){
					case 0:{
						startFlag=0;
						return;
					}
					case 7:{
						startFlag = 1;
						return;//跳出到下面执行参数的赋值
					}
					case 8:{
						//电流幅值下限
						threshold[1][0] = upperRxBuffer[1];
						threshold[2][0] = upperRxBuffer[2];
						threshold[3][0] = upperRxBuffer[3];
						threshold[4][0] = upperRxBuffer[4];
						threshold[5][0] = upperRxBuffer[5];

						//频率设定的值
						parameter[1][2] = upperRxBuffer[6];
						parameter[2][2] = upperRxBuffer[6];
						parameter[3][2] = upperRxBuffer[6];
						parameter[4][2] = upperRxBuffer[6];
						parameter[5][2] = upperRxBuffer[6];
						return;
					}
					case 9:{
						//电流幅值上限
						threshold[1][1] = upperRxBuffer[1];
						threshold[2][1] = upperRxBuffer[2];
						threshold[3][1] = upperRxBuffer[3];
						threshold[4][1] = upperRxBuffer[4];
						threshold[5][1] = upperRxBuffer[5];
						return;
					}
					case 10:{
						//脉宽最低阈值
						threshold[1][2] = upperRxBuffer[1];
						threshold[2][2] = upperRxBuffer[2];
						threshold[3][2] = upperRxBuffer[3];
						threshold[4][2] = upperRxBuffer[4];
						threshold[5][2] = upperRxBuffer[5];
						return;
					}
				  case 11:{
						//脉宽最高阈值
						threshold[1][3] = upperRxBuffer[1];
						threshold[2][3] = upperRxBuffer[2];
						threshold[3][3] = upperRxBuffer[3];
						threshold[4][3] = upperRxBuffer[4];
						threshold[5][3] = upperRxBuffer[5];
						return;
					}
					case 12:{
						//五个通道的使能
						channelEnableflag[1] = upperRxBuffer[1];
						channelEnableflag[2] = upperRxBuffer[2];
						channelEnableflag[3] = upperRxBuffer[3];
						channelEnableflag[4] = upperRxBuffer[4];
						channelEnableflag[5] = upperRxBuffer[5];
						return;
					}
					case 13:{
						//开机三种模式的选择，follow mode，freerun mode，mojoco mode
						initMode = upperRxBuffer[1];
						return;
					}
					case 14:{
						//五个通道的激活压力下限值
						pressureLowerThreshold[1] = upperRxBuffer[1]/10.0;
						pressureLowerThreshold[2] = upperRxBuffer[2]/10.0;
						pressureLowerThreshold[3] = upperRxBuffer[3]/10.0;
						pressureLowerThreshold[4] = upperRxBuffer[4]/10.0;
						pressureLowerThreshold[5] = upperRxBuffer[5]/10.0;
						return;
					}
					case 15:{
						//五个通道的激活压力上限值，是整数值，不用除以10
						pressureUpperThreshold[1] = upperRxBuffer[1];
						pressureUpperThreshold[2] = upperRxBuffer[2];
						pressureUpperThreshold[3] = upperRxBuffer[3];
						pressureUpperThreshold[4] = upperRxBuffer[4];
						pressureUpperThreshold[5] = upperRxBuffer[5];
						return;
					}
					case 16:{
						
						
						return;
					}
					case 0x11:{
						//1通道freerun参数
						parameter[1][0] = upperRxBuffer[1];//1通道幅值
						parameter[1][1] = upperRxBuffer[3]<<8|upperRxBuffer[2];//1通道脉宽
						parameter[1][2] = upperRxBuffer[5]<<8|upperRxBuffer[4];//1通道频率
						return;
					}
					case 0x12:{
						//2通道freerun参数
						parameter[2][0] = upperRxBuffer[1];//2通道幅值
						parameter[2][1] = upperRxBuffer[3]<<8|upperRxBuffer[2];//2通道脉宽
						parameter[2][2] = upperRxBuffer[5]<<8|upperRxBuffer[4];//2通道频率
						return;
					}
					case 0x13:{
						//3通道freerun参数
						parameter[3][0] = upperRxBuffer[1];//3通道幅值
						parameter[3][1] = upperRxBuffer[3]<<8|upperRxBuffer[2];//3通道脉宽
						parameter[3][2] = upperRxBuffer[5]<<8|upperRxBuffer[4];//3通道频率
						return;
					}
					case 0x14:{
						//4通道freerun参数
						parameter[4][0] = upperRxBuffer[1];//4通道幅值
						parameter[4][1] = upperRxBuffer[3]<<8|upperRxBuffer[2];//4通道脉宽
						parameter[4][2] = upperRxBuffer[5]<<8|upperRxBuffer[4];//4通道频率
						return;
					}
					case 0x15:{
						//5通道freerun参数
						parameter[5][0] = upperRxBuffer[1];//5通道幅值
						parameter[5][1] = upperRxBuffer[3]<<8|upperRxBuffer[2];//5通道脉宽
						parameter[5][2] = upperRxBuffer[5]<<8|upperRxBuffer[4];//5通道频率
						return;
					}
					case 0x21:{
						ldVolutage[0] = (upperRxBuffer[1]<<8|upperRxBuffer[2])/1000.0;//mojoco模式通道1
						ldVolutage[1] = (upperRxBuffer[3]<<8|upperRxBuffer[4])/1000.0;//mojoco模式通道2
						ldVolutage[2] = (upperRxBuffer[5]<<8|upperRxBuffer[6])/1000.0;//mojoco模式通道3
						parameterTransmittedSuccess = 0;
						return;
					}
					case 0x22:{
						ldVolutage[3] = (upperRxBuffer[1]<<8|upperRxBuffer[2])/1000.0;//mojoco模式通道4
						ldVolutage[4] = (upperRxBuffer[3]<<8|upperRxBuffer[4])/1000.0;//mojoco模式通道5
						parameterTransmittedSuccess = 0;
						return;
					}

					default:
						return;
				}	
			}
			return;
		}
		
	}
}





/*重定向c库函数printf到DEBUG_USARTX*/
int fputc(int ch,FILE *f)
{
  HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xffff);
  return ch;
}

int fgetc(FILE *f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart1,&ch,1,0xffff);
  return ch;
}

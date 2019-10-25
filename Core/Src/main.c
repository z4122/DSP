/**
  ******************************************************************************
  * @file    STemWin/STemWin_HelloWorld/Src/main.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    29-December-2017
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "WM.h"
#include "math.h"
/** @addtogroup STM32H7xx_HAL_Applications
  * @{
  */

/** @addtogroup STemWin_HelloWorld
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);

float MeanFilter(float input,float *a);
void display(void);
void GetAdData(void);
void MainLoop(void);
void TransferData2PC(void);
long ulResult;


float filter[10][10];



/*
0x00 0x00  0xXX 0xXX  0xXX 0xXX  0xXX 0xXX  0xXX 0xXX  0xXX 0xXX  0xXX 0xXX    0xXX 
| ����ͷ |  ͨ��һ  |  ͨ����   |  ͨ����  |  ͨ����  |  ͨ����  |���������ֽ�|У���|

*/
/* Private functions ---------------------------------------------------------*/

UART_HandleTypeDef UartHandle;

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the MPU attributes as Write Through */

  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32H7xx HAL library initialization:
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 400 MHz */
  SystemClock_Config();
  
  /* Initializes the SDRAM device */
  BSP_SDRAM_Init();  
  
  /* Init the STemWin GUI Library */  
  __HAL_RCC_CRC_CLK_ENABLE(); /* Enable the CRC Module */
  
  GUI_Init();
	GUI_Clear();

	/*Init usart*/
	BSP_Init();

	HAL_Delay(30);
	
	//一开机向PC发送的字节
	u8 t[10];
	t[0] = 0xaa;
	t[1] = 0xbb;
	for(int i = 0;i<5;i++)
	{
		HAL_UART_Transmit(&huart8,t,2,0xfff);
	}
	
	while(1)
	{
		
	}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 160
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Supply configuration update enable 
    */
  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);

    /**Configure the main internal regulator output voltage 
    */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) 
  {
    
  }
    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    //_Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    //_Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    //_Error_Handler(__FILE__, __LINE__);
  }

  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_1);

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(SystemCoreClock/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


/**
  * @brief  Configure the MPU attributes as Write Through for External SDRAM.
  * @note   The Base Address is SDRAM_DEVICE_ADDR .
  *         The Configured Region Size is 32MB because same as SDRAM size.
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_32MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE; 
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Turn LED1 on: Transfer in transmission process is correct */
  BSP_LED_On(LED1);
}



void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  /* Turn LED3 on: Transfer error in reception/transmission process */
  BSP_LED_On(LED3);
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
		
  }
}
#endif

/**
  * @brief  display in LCD
	* @param  i	 : index of channel,
	* @param 	num: the number that will be displayed
  * @retval None
  */
void display()
{
	double num = 0;
		if(name == zzz)
	{
		if(amputatedHand==right)
		{
			for(int i = 0;i<5;i++)
			{
				num = ldVolutage[i];
				if(num<0.01)//为了让LCD显示0.00的时候正常显示
					num = 0;
				switch(i)
				{
					//右手
					case 0:{
						GUI_DispStringAt("channel 1  ", 100, 150); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 150); 
						GUI_DispFloat(transferredDsuValue[i],4);//sensor20
						break;
					}
					case 1:{
						GUI_DispStringAt("channel 2  ", 100, 180); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 180); 
						GUI_DispFloat(transferredDsuValue[i],4);									
						break;
					}
					case 2:{
						GUI_DispStringAt("channel 3  ", 100, 210); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 210); 	
						GUI_DispFloat(transferredDsuValue[i],4);//sensor22	
						break;
					}
					case 3:{//15 sensor1
						GUI_DispStringAt("channel 4  ", 100, 240); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 240); 
						GUI_DispFloat(transferredDsuValue[i],4);//sensor23
						break;
					}
					case 4:{//1 sensor4
						GUI_DispStringAt("channel 5  ", 100, 270); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 270); 
						GUI_DispFloat(transferredDsuValue[i],4);//sensor24
						break;
					}
					default:
						break;
				}	
			}
		}
		else
		{
			for(int i = 5;i<10;i++)
			{
				num = ldVolutage[i];
				switch(i)
				{
					//左手1
					case 5:{
						GUI_DispStringAt("channel 1  ", 100, 150); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 150); 
						GUI_DispFloat(transferredDsuValue[i],4);
						break;
					}
					case 6:{
						GUI_DispStringAt("channel 2  ", 100, 180); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 180); 
						GUI_DispFloat(transferredDsuValue[i],4);
						break;
					}
					case 7:{
						GUI_DispStringAt("channel 3  ", 100, 210); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 210); 
						GUI_DispFloat(transferredDsuValue[i],4);
						break;
					}
					case 8:{
						GUI_DispStringAt("channel 4  ", 100, 240); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 240); 
						GUI_DispFloat(transferredDsuValue[i],4);				
						break;
					}
					case 9:{
						GUI_DispStringAt("channel 5  ", 100, 270); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 270); 
						GUI_DispFloat(transferredDsuValue[i],4);						
						break;
					}
					default:
						break;
				}	
			}
		}
	}
	else if(name == zj)
	{
		if(amputatedHand==right)
		{
			for(int i = 0;i<5;i++)
			{
				num = ldVolutage[i];
				if(num<0.01)//为了让LCD显示0.00的时候正常显示
					num = 0;
				switch(i)
				{
					//右手
					case 0:{//16 sensor2
						GUI_DispStringAt("channel 1  ", 100, 150); 
						GUI_DispFloat(num,4);//除以1000000后是实际电压值
						GUI_DispStringAt("transferred data  ", 300, 150); 
						GUI_DispFloat(transferredDsuValue[i],4);//sensor20	
						break;
					}
					case 1:{//3 sensor3
						GUI_DispStringAt("channel 2  ", 100, 180); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 180); 						
						GUI_DispFloat(transferredDsuValue[i],4);
						break;
					}
					case 2:{//2 sensor6
						GUI_DispStringAt("channel 3  ", 100, 210); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 210); 
						GUI_DispFloat(transferredDsuValue[i],4);//sensor22
						break;
					}
					case 3:{//15 sensor1
						GUI_DispStringAt("channel 4  ", 100, 240); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 240); 
						GUI_DispFloat(transferredDsuValue[i],4);//sensor23
						break;
					}
					case 4:{//1 sensor4
						GUI_DispStringAt("channel 5  ", 100, 270); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 270); 
						GUI_DispFloat(transferredDsuValue[i],4);//sensor24
						break;
					}
					default:
						break;
				}	
			}
		}
		else
		{
			for(int i = 5;i<10;i++)
			{
				num = ldVolutage[i];
				switch(i)
				{
					//左手
					case 5:{
						GUI_DispStringAt("channel 1  ", 100, 150); 
						GUI_DispFloat(num,4);//除以后是实际电压值
						GUI_DispStringAt("transferred data  ", 300, 150); 						
						GUI_DispFloat(transferredDsuValue[i],4);
						break;
					}
					case 6:{
						GUI_DispStringAt("channel 2  ", 100, 180); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 180); 							
						GUI_DispFloat(transferredDsuValue[i],4);
						break;
					}
					case 7:{
						GUI_DispStringAt("channel 3  ", 100, 210); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 210); 						
						GUI_DispFloat(transferredDsuValue[i],4);						
						break;
					}
					case 8:{
						GUI_DispStringAt("channel 4  ", 100, 240); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 240); 
						GUI_DispFloat(transferredDsuValue[i],4);
						break;
					}
					case 9:{
						GUI_DispStringAt("channel 5  ", 100, 270); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 270); 
						GUI_DispFloat(transferredDsuValue[i],4);						
						break;			
					}
					default:
						break;
				}	
			}
			
		}
	}
}

//从ADS1256中取8个通道的值，另外从STM32AD端口中取两个通道的值，合在一起。
void GetAdData(void)
{
		static int tempcnt = 0;
		
		for(int i = 0;i < 5;i++)
		{
			ulResult = ADS_sum( (i << 4) | 0x08);	
			
			if( ulResult & 0x800000 )
			{
			 	ulResult = ~(unsigned long)ulResult;
				ulResult &= 0x7fffff;
				ulResult += 1;
				ulResult = -ulResult;
			}
			
			long double temp = (long double)ulResult*0.59604644775390625;
			if(tempcnt<10){
				offset[i] += temp/10;
			}
			if(temp>0&&temp/1000<5000){
				ldVolutage[i] = temp/1000000;//-offset[i];
				//ldVolutage[i] = MeanFilter(ldVolutage[i],filter[i]);
				//ldVolutage[i] = ldVolutage[i]/1000;
				
			}
		}

		for(int i = 5;i < 8;i++)
		{
			ulResult = ADS_sum( (i << 4) | 0x08);
			if( ulResult & 0x800000 )
			{
			 	ulResult = ~(unsigned long)ulResult;
				ulResult &= 0x7fffff;
				ulResult += 1;
				ulResult = -ulResult;
			}		
				
			long double temp = (long double)ulResult*0.59604644775390625;
			
			if(tempcnt<10){
				offset[i] += temp/10;
			}
			
			if(temp>0&&temp/1000<5000){
				ldVolutage[i] = temp/1000000;//-offset[i];
				//ldVolutage[i] = MeanFilter(ldVolutage[i],filter[i]);
				//ldVolutage[i] = ldVolutage[i]/1000;
			}
		}
	
			ADC_convertedvalue[0] = (ADC_detectedvalue[0]&0xffff)*3.3/65536;
			ADC_convertedvalue[1] = (ADC_detectedvalue[1]&0xffff)*3.3/65536;
		
			if(tempcnt<10){
				tempcnt++;
				offset[8] += ADC_convertedvalue[0]/10;
				offset[9] += ADC_convertedvalue[1]/10;
			}
			ldVolutage[8] = MeanFilter(ADC_convertedvalue[0],filter[8]);//-offset[8];
			ldVolutage[9] = MeanFilter(ADC_convertedvalue[1],filter[9]);//-offset[9];
}

//负责把内容显示在屏幕上，并且
void MainLoop()
{
	static int last_flag = 0;

	if(last_flag!=testmode_flag){
		GUI_Clear();
	}
	
	if(testmode_flag<8&&testmode_flag!=0){
		display();//正常工作模式不会大于7
	}
	else{
		testmode_flag=0;
	}

	switch(testmode_flag)
	{
		case 0:
			//clear_flag++;
			//if(clear_flag>=10){
			//	GUI_Clear();
			//	clear_flag=0;
			//}
			if(last_flag!=testmode_flag){
				GUI_Clear();
			}
			GUI_DispStringAt("stop ",250,270);

			break;
			
		case 1:
			GUI_DispStringAt("left amplitude mode   ", 200, 330); 	
			amputatedHand = left;
			break;
		case 2:
			GUI_DispStringAt("left frequency mode", 200, 330); 
			amputatedHand = left;
			break;
		case 3:
			GUI_DispStringAt("left width mode", 200, 330); 
			amputatedHand = left;
			break;
		case 4:
			GUI_DispStringAt("right amplitude mode   ", 200, 330); 	
			amputatedHand = right;
			break;
		case 5:
			GUI_DispStringAt("right frequency mode", 200, 330); 
			amputatedHand = right;
			break;
		case 6:
			GUI_DispStringAt("right width mode", 200, 330); 
			amputatedHand = right;
			break;
		case 7:
			GUI_DispStringAt("test mode  ", 100, 330); 
			GUI_DispFloat(parameter[1][0]/10.0,4);//电流
			GUI_DispStringAt("mA  ", 290, 330); 
			GUI_DispFloat(parameter[1][1],4);//频率
			GUI_DispStringAt("us  ", 410, 330); 
			GUI_DispFloat(parameter[1][2],4);//脉宽
			GUI_DispStringAt("hz  ", 520, 330); 
			break;
		
		default:
			break;			
	}
		last_flag = testmode_flag;
}

//均值滤波
float MeanFilter(float input,float *a)
{
	float temp = 0;
	for(int i = 0;i<9;i++){
		a[i]=a[i+1];
		temp += a[i];
	}
	a[9]=input;
	temp+=input;
	return temp/10;
}


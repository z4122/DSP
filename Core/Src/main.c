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
static void Error_Handler(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
extern void MainTask(char*,int,int);
float MeanFilter(float input,float *a);
void display(void);
void GetAdData(void);
void MainLoop(void);
long ulResult;
long double ldVolutage[10];

uint8_t data[15];//传输给上位机的数据
float ADC_convertedvalue[2];
float filter0[10],filter1[10];
enum Hand {left=0,right=1};
enum Hand amputatedHand = left;
/*
0x00 0x00  0xXX 0xXX  0xXX 0xXX  0xXX 0xXX  0xXX 0xXX  0xXX 0xXX  0xXX 0xXX    0xXX 
| 数据头 |  通道一  |  通道二   |  通道三  |  通道四  |  通道五  |保留两个字节|校验和|

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
	MainTask("test mode",100,20);  
	 GUI_Clear();

	/*Init usart*/
	BSP_Init();

		
	HAL_Delay(500);
		
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1)
  {
  }
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
	long double num = 0;
	if(amputatedHand==right)
	{
		for(int i = 0;i<5;i++)
		{
			num = ldVolutage[i];
			switch(i)
			{
				//右手为截肢端
				case 0://16
					//GUI_Clear();	
					GUI_DispStringAt("channel 1  ", 100, 20); 
					GUI_DispFloat(num/1000000,4);//调整显示的位数
					GUI_DispStringAt("transfered data  ", 300, 20); 
					GUI_DispFloat(16.9*num/1000000-5.19,4);
					//stimulate(&huart5,16.9*num/1000000-5.19);//ok ch1
					break;
				case 1://3
					GUI_DispStringAt("channel 2  ", 100, 50); 
					GUI_DispFloat(num/1000000,4);
					GUI_DispStringAt("transfered data  ", 300, 50); 
					GUI_DispFloat(22.9*num/1000000-3.459,4);
					stimulate(&huart3,22.9*num/1000000-3.459);//ok ch2
					break;
				case 2://2
					GUI_DispStringAt("channel 3  ", 100, 80); 
					GUI_DispFloat(num*3.3/5000000,4);
					GUI_DispStringAt("transfered data  ", 300, 80); 
					GUI_DispFloat(20.04*num/1000000-2.464,4);
					//stimulate(&huart1,20.04*num/1000000-2.464);//bad ch3
					break;
				case 3://15
					GUI_DispStringAt("channel 4  ", 100, 110); 
					GUI_DispFloat(num/1000000,4);
					GUI_DispStringAt("transfered data  ", 300, 110); 
					GUI_DispFloat(18.81*num/1000000-1.538,4);
					stimulate(&huart4,18.81*num/1000000-1.538);
					break;
				case 4://1
					GUI_DispStringAt("channel 5  ", 100, 140); 
					GUI_DispFloat(num/1000000,4);
					GUI_DispStringAt("transfered data  ", 300, 140); 
					GUI_DispFloat(23.48*num/1000000-2.942,4);
					stimulate(&huart7,23.48*num/1000000-2.942);//ok ch6
				
					break;
				
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
				//左手为截肢端
				case 5:
					GUI_DispStringAt("channel 1  ", 100, 20); 
					GUI_DispFloat(num/1000000,4);//调整显示的位数
					GUI_DispStringAt("transfered data  ", 300, 20); 
					GUI_DispFloat(19.86*num/1000000-3.248,4);
					//stimulate(&huart5,19.86*num/1000000-3.248);
					break;
				case 6:
					GUI_DispStringAt("channel 2  ", 100, 50); 
					GUI_DispFloat(num/1000000,4);
					GUI_DispStringAt("transfered data  ", 300, 50); 
					GUI_DispFloat(20.78*num/1000000-2.536,4);
					//stimulate(&huart3,20.78*num/1000000-2.536);
					break;
				case 7:
					GUI_DispStringAt("channel 3  ", 100, 80); 
					GUI_DispFloat(num*3.3/5000000,4);
					GUI_DispStringAt("transfered data  ", 300, 80); 
					GUI_DispFloat(20.2*num/1000000-0.8187,4);
					//stimulate(&huart1,20.2*num/1000000-0.8187);
					break;
				case 8:
					GUI_DispStringAt("channel 4  ", 100, 110); 
					GUI_DispFloat(num,4);
					GUI_DispStringAt("transfered data  ", 300, 110); 
					GUI_DispFloat(21.08*num-4.121,4);
					//stimulate(&huart4,21.08*num-4.121);
					break;
				case 9:
					GUI_DispStringAt("channel 5  ", 100, 140); 
					GUI_DispFloat(num,4);
					GUI_DispStringAt("transfered data  ", 300, 140); 
					GUI_DispFloat(20.94*num-0.7668,4);
					//stimulate(&huart7,20.94*num-0.7668);
					break;
				
				
				default:
					break;
			}	
		}
		
	}
}

void GetAdData(void)
{
		//代表1-5通道
		data[0] = 0xff;
		data[1] = 0xff;
	
		for(int i = 0;i < 5;i++)
		{
			ulResult = ADS_sum( (i << 4) | 0x08);	
			
			//ulResult = ADS_sum( ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);	
			if( ulResult & 0x800000 )
			{
			 	ulResult = ~(unsigned long)ulResult;
				ulResult &= 0x7fffff;
				ulResult += 1;
				ulResult = -ulResult;
			}
						
			ldVolutage[i] = (long double)ulResult*0.59604644775390625;

			
			//ldVolutage[i] = ldVolutage[i]/1000;
			data[i*2+2] = ((u16)(ldVolutage[i]/1000))>>8;
			data[i*2+3] = ((u16)(ldVolutage[i]/1000))&0xFF;
		
			//HAL_Delay(10);	
		}
		
		HAL_UART_Transmit(&huart8,data,15,0xfff);
		
		
		//代表6-10通道
		data[0] = 0xff;
		data[1] = 0xf1;
	
		for(int i = 5;i < 8;i++)
		{
			 ulResult = ADS_sum( (i << 4) | 0x08);
				
			//ulResult = ADS_sum( ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);	
			if( ulResult & 0x800000 )
			{
			 	ulResult = ~(unsigned long)ulResult;
				ulResult &= 0x7fffff;
				ulResult += 1;
				ulResult = -ulResult;
			}		
			
			ldVolutage[i] = (long double)ulResult*0.59604644775390625;
				
			//ldVolutage[i] = ldVolutage[i]/1000;
			data[i*2-8] = ((u16)(ldVolutage[i]/1000))>>8;
			data[i*2-7] = ((u16)(ldVolutage[i]/1000))&0xFF;
			

			//HAL_Delay(10);	
		}
	
			ADC_convertedvalue[0] = (float)(ADC_detectedvalue[0]&0xffff)*3.3/65536;
			ADC_convertedvalue[1] = (float)(ADC_detectedvalue[1]&0xffff)*3.3/65536;
		
			ldVolutage[8] = MeanFilter(ADC_convertedvalue[0],filter0);
			ldVolutage[9] = MeanFilter(ADC_convertedvalue[1],filter1);
		
			data[8] = (u16)ADC_convertedvalue[0]>>8;
			data[9] = (u16)ADC_convertedvalue[0]&0xFF;
			data[10] =(u16)ADC_convertedvalue[1]>>8;
			data[11] =(u16)ADC_convertedvalue[1]&0xFF;
		
			HAL_UART_Transmit(&huart8,data,15,0xfff);
		
			HAL_UART_Receive_IT(&huart8,&UART8RxBuff,1);
}


void MainLoop()
{
	static int last_flag = 0;
	static int clear_flag = 0;
	u8 t[10];
	t[0] = 0xaa;
	t[1] = 0xbb;
	
	for(int i = 0;i<5;i++)
	{
		HAL_UART_Transmit(&huart8,t,2,0xfff);
	}
 		
	//VisualScope(&huart1,1,2,3,4);
	//fputc(1,0);
	
	display();
	if(last_flag!=testmode_flag)
		GUI_Clear();
		
	switch(testmode_flag)
	{
		case 0:
			clear_flag++;
			if(clear_flag>=10){
				GUI_Clear();
				clear_flag=0;
			}
			GUI_DispStringAt("stop ",250,270);
			//while(testmode_flag==0) {
			//	display();
			//}
			break;
		case 1:
			GUI_DispStringAt("right amplitude mode   ", 200, 270); 	
			amputatedHand = right;
			break;
		case 2:
			GUI_DispStringAt("right frequency mode", 200, 270); 
			amputatedHand = right;
			break;
		case 3:
			GUI_DispStringAt("right width mode", 200, 270); 
			amputatedHand = right;
			break;
		case 4:
			GUI_DispStringAt("test mode  ", 100, 270); 
			GUI_DispFloat(parameter[1][0]/10.0,4);//调整显示的位数
			GUI_DispStringAt("mA  ", 290, 270); 
			GUI_DispFloat(parameter[1][1],4);//调整显示的位数
			GUI_DispStringAt("us  ", 410, 270); 
			GUI_DispFloat(parameter[1][2],4);//调整显示的位数
			GUI_DispStringAt("hz  ", 520, 270); 
			break;
		case 5:
			GUI_DispStringAt("left amplitude mode   ", 200, 270); 	
			amputatedHand = left;
			break;
		case 6:
			GUI_DispStringAt("left frequency mode", 200, 270); 
			amputatedHand = left;
			break;
		case 7:
			GUI_DispStringAt("left width mode", 200, 270); 
			amputatedHand = left;
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


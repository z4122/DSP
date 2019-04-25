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
extern void MainTask(char*,int,int);
float MeanFilter(float input,float *a);
void display(void);
void GetAdData(void);
void MainLoop(void);
void TransferData2PC(void);
long ulResult;


float filter[10][10];
enum Hand {left=0,right=1};
enum Mode {zzz = 0,zj = 1};
enum Hand amputatedHand = right;
enum Mode name = zj;

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
	MainTask("test mode",100,20);  
	GUI_Clear();

	/*Init usart*/
	BSP_Init();

	HAL_Delay(30);

	//一开机向PC发送的字节，
	u8 t[10];
	t[0] = 0xaa;
	t[1] = 0xbb;
	for(int i = 0;i<5;i++)
	{
		HAL_UART_Transmit(&huart8,t,2,0xfff);
	}
	
	while(1)
	{	
		GetAdData();
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
					case 0:{//16 sensor2
						//GUI_Clear();	
						GUI_DispStringAt("channel 1  ", 100, 20); 
						GUI_DispFloat(num/1000000,4);//除以1000000后是实际电压值
						GUI_DispStringAt("transferred data  ", 300, 20); 
						//float curve1=1.657*exp(2.113*(num/1000000-0.07));
						//float curve2=-1.056*exp(-0.5708*(num/1000000-0.07));
						float curve1=-18.4*(num/1000000)*(num/1000000)*(num/1000000)*(num/1000000)+59.65*(num/1000000)*(num/1000000)*(num/1000000);
						float curve2=-61.04*(num/1000000)*(num/1000000)+33.58*(num/1000000)-5.038;
						float curve3=curve1+curve2;
						GUI_DispFloat(curve3,4);//sensor20
						//if(channelEnableflag[1])
							stimulate(&huart1,curve3,1);//ok ch1
						break;
					}
					case 1:{//3 sensor3
						GUI_DispStringAt("channel 2  ", 100, 50); 
						GUI_DispFloat(num/1000000,4);
						GUI_DispStringAt("transferred data  ", 300, 50); 
						//GUI_DispFloat(4.302*exp(1.132*num/1000000)-3.705*exp(-1.904*num/1000000),4);
						//float curve4=1.69*exp(1.789*(num/1000000));
						//float curve5=-1.168*exp(-1.216*(num/1000000));
						float curve4=6.006*(num/1000000)*(num/1000000);
						float curve5=4.532*(num/1000000)-1.164;
						float curve6=curve4+curve5;
						GUI_DispFloat(curve6,4);//sensor21
						//if(channelEnableflag[2])
							stimulate(&huart3, curve6,2);
						break;
					}
					case 2:{//2 sensor6
						GUI_DispStringAt("channel 3  ", 100, 80); 
						GUI_DispFloat(num/1000000,4);
						GUI_DispStringAt("transferred data  ", 300, 80); 
						//float curve7=2.25*exp(1.431*num/1000000+0.25);
						//float curve8=-2.087*exp(-2.515*num/1000000+0.75);
						float curve7=4.348*(num/1000000)*(num/1000000)*(num/1000000)-3.622*(num/1000000)*(num/1000000);
						float curve8=9.488*(num/1000000)-2.023;
						float curve9=curve7+curve8;				
						GUI_DispFloat(curve9,4);//sensor22
						//if(channelEnableflag[3])
							stimulate(&huart4,curve9,3);//bad ch3
						break;
					}
					case 3:{//15 sensor1
						GUI_DispStringAt("channel 4  ", 100, 110); 
						GUI_DispFloat(num/1000000,4);
						GUI_DispStringAt("transferred data  ", 300, 110); 
						//GUI_DispFloat(3.732*exp(1.905*num/1000000*4.6/5)-4.65*exp(-4.638*num/1000000*4.6/5),4);
						float curve10=12.04*(num/1000000)*(num/1000000)*(num/1000000)*(num/1000000)-24.93*(num/1000000)*(num/1000000)*(num/1000000);
						float curve11=18.57*(num/1000000)*(num/1000000)+1.145*(num/1000000)-1.04;
						float curve12=curve10+curve11;
						GUI_DispFloat(curve12,4);//sensor23
						//if(channelEnableflag[4])
							stimulate(&huart5,curve12,4);
						break;
					}
					case 4:{//1 sensor4
						GUI_DispStringAt("channel 5  ", 100, 140); 
						GUI_DispFloat(num/1000000,4);
						GUI_DispStringAt("transferred data  ", 300, 140); 
						//GUI_DispFloat(2.878*exp( 2.423*num/1000000*4.6/5-0.08)-3.286*exp(-7.177*num/1000000*4.6/5-0.08),4);
						//float curve10=1.707*exp(2.293*(num/1000000-0.04));
						//float curve11=-1.727*exp(-8.538*(num/1000000-0.04));
						float curve13=-8.86*(num/1000000)*(num/1000000)*(num/1000000)*(num/1000000)+30.72*(num/1000000)*(num/1000000)*(num/1000000);
						float curve14=-29.84*(num/1000000)*(num/1000000)+16.15*(num/1000000)-2.713;
						float curve15=curve13+curve14;				
						GUI_DispFloat(curve15,4);//sensor24
						
						stimulate(&huart7, curve15,5);//ok ch6
					
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
						GUI_DispStringAt("channel 1  ", 100, 20); 
						GUI_DispFloat(num/1000000,4);//除以1000000后是实际电压值

						GUI_DispStringAt("transferred data  ", 300, 20); 
						float curve01=1.957*exp(1.988*num/1000000*4.6/5-0.05);
						float curve02=-1.76*exp(-3.789*num/1000000*4.6/5-0.05);
						float curve03=curve01+curve02;
						GUI_DispFloat(curve03,4);
						if(channelEnableflag[1])
							stimulate(&huart1, curve03,1);
						break;
					}
					case 6:{
						GUI_DispStringAt("channel 2  ", 100, 50); 
						GUI_DispFloat(num/1000000,4);

						GUI_DispStringAt("transferred data  ", 300, 50); 
						float curve04=1.946e+04*exp(0.8933*num/1000000*4.6/5-0.125);
						float curve05=-1.946e+04*exp(0.8929*num/1000000*4.6/5-0.125);
						float curve06=curve04+curve05;
						GUI_DispFloat(curve06,4);

						if(channelEnableflag[2])
							stimulate(&huart3,curve06,2);
						break;
					}
					case 7:{
						GUI_DispStringAt("channel 3  ", 100, 80); 
						GUI_DispFloat(num*3.3/5000000,4);

						GUI_DispStringAt("transferred data  ", 300, 80); 
						float curve07=2.455*exp(1.891*num/1000000*5/4.6-0.3);
						float curve08=-2.977*exp(-3.137*num/1000000*5/4.6-0.3);
						float curve09=curve07+curve08;
						GUI_DispFloat(curve09,4);

						if(channelEnableflag[3])
							stimulate(&huart4,curve09,3);
						break;
					}
					case 8:{
						GUI_DispStringAt("channel 4  ", 100, 110); 
						GUI_DispFloat(num,4);

						GUI_DispStringAt("transferred data  ", 300, 110); 
						GUI_DispFloat(17*(num-0.15),4);

						if(channelEnableflag[4])
							stimulate(&huart5,17*(num-0.15),4);
						break;
					}
					case 9:{
						GUI_DispStringAt("channel 5  ", 100, 140); 
						GUI_DispFloat(num,4);

						GUI_DispStringAt("transferred data  ", 300, 140); 
						GUI_DispFloat(17*(num-0.3),4);

						if(channelEnableflag[5])
							stimulate(&huart7,17*(num-0.3),5);
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
					case 0:{//16
						//GUI_Clear();	
						GUI_DispStringAt("channel 1  ", 100, 20); 
						GUI_DispFloat(num/1000000,4);//除以1000000后是实际电压值
						GUI_DispStringAt("transferred data  ", 300, 20); 
						//double curve1=(double)(1.657*exp(2.113*(num/1000000-0.07)));
						//double curve2=-1.056*exp(-0.5708*(num/1000000-0.07));
						double curve1=(double)(1.793*exp(2.264*num/1000000*4.6/5));
						double curve2=-1.72*exp(-4.736*num/1000000*4.6/5)-5;					
						double curve3=curve1+curve2;
						GUI_DispFloat(curve3,4);//sensor20
						
						stimulate(&huart1,curve3,1);//ok ch1
						break;
					}
					case 1:{//3
						GUI_DispStringAt("channel 2  ", 100, 50); 
						GUI_DispFloat(num/1000000,4);
						GUI_DispStringAt("transferred data  ", 300, 50); 
						//GUI_DispFloat(4.302*exp(1.132*num/1000000)-3.705*exp(-1.904*num/1000000),4);
						//double curve4=1.69*exp(1.789*(num/1000000));
						//double curve5=-1.168*exp(-1.216*(num/1000000));
						double curve4=3.616*exp(1.62*num/1000000*4.6/5-0.2);
						double curve5=-3.588*exp(-2.139*num/1000000*4.6/5-0.2);					
						double curve6=curve4+curve5;
						GUI_DispFloat(curve6,4);//sensor21
						
						stimulate(&huart3, curve6,2);
						break;
					}
					case 2:{//2
						GUI_DispStringAt("channel 3  ", 100, 80); 
						GUI_DispFloat(num/1000000,4);
						GUI_DispStringAt("transferred data  ", 300, 80); 
						//double curve7=2.25*exp(1.431*num/1000000+0.25);
						//double curve8=-2.087*exp(-2.515*num/1000000+0.75);
						double curve7=4.461*exp(1.572*num/1000000*4.6/5);
						double curve8=-5.077*exp(-2.702*num/1000000*4.6/5);					
						double curve9=curve7+curve8;				
						GUI_DispFloat(curve9,4);//sensor22
						
						stimulate(&huart4,curve9,3);//bad ch3
						break;
					}
					case 3:{//15
						GUI_DispStringAt("channel 4  ", 100, 110); 
						GUI_DispFloat(num/1000000,4);
						GUI_DispStringAt("transferred data  ", 300, 110); 
						//GUI_DispFloat(3.732*exp(1.905*num/1000000*4.6/5)-4.65*exp(-4.638*num/1000000*4.6/5),4);
						//sensor23
						double curve10=3.732*exp(1.905*num/1000000*4.6/5-0.15);
						double curve11=-4.65*exp(-4.638*num/1000000*4.6/5-0.15);//sensor17
						
						float curve12=curve10+curve11;	
						GUI_DispFloat(curve12,4);					
						stimulate(&huart5,curve12,4);
						break;
					}
					case 4:{//1
						GUI_DispStringAt("channel 5  ", 100, 140); 
						GUI_DispFloat(num/1000000,4);
						GUI_DispStringAt("transferred data  ", 300, 140); 
						//GUI_DispFloat(2.878*exp( 2.423*num/1000000*4.6/5-0.08)-3.286*exp(-7.177*num/1000000*4.6/5-0.08),4);
						//double curve10=1.707*exp(2.293*(num/1000000-0.04));
						//double curve11=-1.727*exp(-8.538*(num/1000000-0.04));
						double curve10=2.878*exp( 2.423*num/1000000*4.6/5-0.35);
						double curve11=-3.286*exp(-7.177*num/1000000*4.6/5-0.35);					
						double curve12=curve10+curve11;				
						GUI_DispFloat(curve12,4);//sensor24
						
						stimulate(&huart7, curve12,5);//ok ch6
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
						GUI_DispStringAt("channel 1  ", 100, 20); 
						GUI_DispFloat(num/1000000,4);//除以1000000后是实际电压值

						GUI_DispStringAt("transferred data  ", 300, 20); 
						//double curve01=1.957*exp(1.988*num/1000000*4.6/5-0.05);
						//double curve02=-1.76*exp(-3.789*num/1000000*4.6/5-0.05);
						double curve01=1.476*exp(1.864*num/1000000+0.01984);
						double curve02=-2.169*exp(-13.69*num/1000000-1.2321);				
						double curve03=curve01+curve02;
						GUI_DispFloat(curve03,4);
						
						stimulate(&huart1, curve03,1);
						break;
						}
					case 6:{
						GUI_DispStringAt("channel 2  ", 100, 50); 
						GUI_DispFloat(num/1000000,4);

						GUI_DispStringAt("transferred data  ", 300, 50); 
						//double curve04=1.946e+04*exp(0.8933*num/1000000*4.6/5-0.125);
						//double curve05=-1.946e+04*exp(0.8929*num/1000000*4.6/5-0.125);
						double curve04=2.282*exp(1.986*num/1000000+0.05958);
						double curve05=-3.907*exp(-5.262*num/1000000-0.18996);					
						double curve06=curve04+curve05;
						GUI_DispFloat(curve06,4);

						stimulate(&huart3,curve06,2);
						break;
					}
					case 7:{
						GUI_DispStringAt("channel 3  ", 100, 80); 
						GUI_DispFloat(num*3.3/5000000,4);

						GUI_DispStringAt("transferred data  ", 300, 80); 
						//double curve07=2.455*exp(1.891*num/1000000*5/4.6-0.3);
						//double curve08=-2.977*exp(-3.137*num/1000000*5/4.6-0.3);
						double curve07=-0.026*exp(7.94*num/1000000-0.1588);
						double curve08=0.8*exp(4.538*num/1000000-0.09076);
						double curve09=curve07+curve08;
						GUI_DispFloat(curve09,4);

						stimulate(&huart4,curve09,3);
						break;
					}
					case 8:{
						GUI_DispStringAt("channel 4  ", 100, 110); 
						GUI_DispFloat(num,4);

						GUI_DispStringAt("transferred data  ", 300, 110); 
						GUI_DispFloat(14.1*num-4,4);

						stimulate(&huart5,14.1*num-4,4);
						break;
					}
					case 9:{
						GUI_DispStringAt("channel 5  ", 100, 140); 
						GUI_DispFloat(num,4);

						GUI_DispStringAt("transferred data  ", 300, 140); 
						GUI_DispFloat(16.55*num-4.5,4);

						stimulate(&huart7,16.55*num-4.5,5);
						break;			
					}
					default:
						break;
				}	
			}
			
		}
	}
}

//从ADS1256中取8个通道的值，从STM32AD端口中取两个通道的值，合在一起。
void GetAdData(void)
{
		static int tempcnt = 0;
		
		HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);//Reset
		ADS1256_Init();//理论上不用再次初始化，这是折衷的做法，否则会出现AD采样偶尔错误的后果。经测试推测是与DSU通信有关。
		HAL_Delay(10);
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
				ldVolutage[i] = temp;//-offset[i];
				//ldVolutage[i] = MeanFilter(temp,filter[i]);
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
				ldVolutage[i] = temp;//-offset[i];
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

	display();
	if(last_flag!=testmode_flag)
		GUI_Clear();
	
	if(testmode_flag>=8){
		testmode_flag = last_flag;//正常工作模式不会大于7
	}
	switch(testmode_flag)
	{
		case 0:
			//clear_flag++;
			//if(clear_flag>=10){
				GUI_Clear();
			//	clear_flag=0;
			//}
			GUI_DispStringAt("stop ",250,270);
			while(testmode_flag==0) {
				//display();
//				stim_stop(&huart1);
//				stim_stop(&huart3);
//				stim_stop(&huart4);
//				stim_stop(&huart5);
//				stim_stop(&huart7);
			}
			break;
			
		case 1:
			GUI_DispStringAt("left amplitude mode   ", 200, 270); 	
			amputatedHand = left;
			break;
		case 2:
			GUI_DispStringAt("left frequency mode", 200, 270); 
			amputatedHand = left;
			break;
		case 3:
			GUI_DispStringAt("left width mode", 200, 270); 
			amputatedHand = left;
			break;
		case 4:
			GUI_DispStringAt("right amplitude mode   ", 200, 270); 	
			amputatedHand = right;
			break;
		case 5:
			GUI_DispStringAt("right frequency mode", 200, 270); 
			amputatedHand = right;
			break;
		case 6:
			GUI_DispStringAt("right width mode", 200, 270); 
			amputatedHand = right;
			break;
		case 7:
			GUI_DispStringAt("test mode  ", 100, 270); 
			GUI_DispFloat(parameter[1][0]/10.0,4);//电流
			GUI_DispStringAt("mA  ", 290, 270); 
			GUI_DispFloat(parameter[1][1],4);//频率
			GUI_DispStringAt("us  ", 410, 270); 
			GUI_DispFloat(parameter[1][2],4);//脉宽
			GUI_DispStringAt("hz  ", 520, 270); 
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


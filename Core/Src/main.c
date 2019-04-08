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

uint8_t data[15];//�������λ��������
float ADC_convertedvalue[2];
float filter0[10],filter1[10];
enum Hand {left=0,right=1};
enum Hand amputatedHand = right;
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

	
		
	HAL_Delay(500);

	//����ȷ���źŸ�PC
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
	float num = 0;
	if(amputatedHand==right)
	{
		for(int i = 0;i<5;i++)
		{
			num = ldVolutage[i];
			if(num<0.01)//Ϊ��LCD��ʾ�������쳣�������ص�����Ϊ0
				num = 0;
			switch(i)
			{
				//����Ϊ��֫��
				case 0://16
					//GUI_Clear();	.
					GUI_DispStringAt("channel 1  ", 100, 20); 
					GUI_DispFloat(num/1000000,4);//������ʾ��λ��
					GUI_DispStringAt("transfered data  ", 300, 20); 
					float curve1=1.657*exp(2.113*(num/1000000-0.07));
				  float curve2=-1.056*exp(-0.5708*(num/1000000-0.07));
				  float curve3=curve1+curve2;
				  GUI_DispFloat(curve3,4);//sensor20
					if(channelEnableflag[1])
						stimulate(&huart1,curve3,1);//ok ch1
					break;
				case 1://3
					GUI_DispStringAt("channel 2  ", 100, 50); 
					GUI_DispFloat(num/1000000,4);
					GUI_DispStringAt("transferred data  ", 300, 50); 
					//GUI_DispFloat(4.302*exp(1.132*num/1000000)-3.705*exp(-1.904*num/1000000),4);
					float curve4=1.69*exp(1.789*(num/1000000));
				  float curve5=-1.168*exp(-1.216*(num/1000000));
				  float curve6=curve4+curve5;
				  GUI_DispFloat(curve6,4);//sensor21
					if(channelEnableflag[2])
						stimulate(&huart3, curve6,2);
					break;
					
				case 2://2
					GUI_DispStringAt("channel 3  ", 100, 80); 
					GUI_DispFloat(num/1000000,4);
					GUI_DispStringAt("transfered data  ", 300, 80); 
					float curve7=2.25*exp(1.431*num/1000000+0.25);
				  float curve8=-2.087*exp(-2.515*num/1000000+0.75);
				  float curve9=curve7+curve8;				
				  GUI_DispFloat(curve9,4);//sensor22
					if(channelEnableflag[3])
						stimulate(&huart4,curve9,3);//bad ch3
					break;
				case 3://15
					GUI_DispStringAt("channel 4  ", 100, 110); 
					GUI_DispFloat(num/1000000,4);
					//GUI_DispFloat(3.732*exp(1.905*num/1000000*4.6/5)-4.65*exp(-4.638*num/1000000*4.6/5),4);
					GUI_DispFloat(1.124*exp(2.141*num/1000000-0.2),4);//sensor23
					if(channelEnableflag[4])
						stimulate(&huart5,1.124*exp(2.141*num/1000000-0.2),4);
					break;
				case 4://1
					GUI_DispStringAt("channel 5  ", 100, 140); 
					GUI_DispFloat(num/1000000,4);
					GUI_DispStringAt("transferred data  ", 300, 140); 
					//GUI_DispFloat(2.878*exp( 2.423*num/1000000*4.6/5-0.08)-3.286*exp(-7.177*num/1000000*4.6/5-0.08),4);
					float curve10=1.707*exp(2.293*(num/1000000-0.04));
				  float curve11=-1.727*exp(-8.538*(num/1000000-0.04));
				  float curve12=curve10+curve11;				
				  GUI_DispFloat(curve12,4);//sensor24
					if(channelEnableflag[5])
						stimulate(&huart7, curve12,5);//ok ch6
				
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
				//����Ϊ��֫��
				case 5:
					GUI_DispStringAt("channel 1  ", 100, 20); 
					GUI_DispFloat(num/1000000,4);//������ʾ��λ��
<<<<<<< HEAD
					GUI_DispStringAt("transferred data  ", 300, 20); 
					GUI_DispFloat(1.957*exp(1.988*num/1000000*4.6/5-0.35)-1.76*exp(-3.789*num/1000000*4.6/5-0.35),4);
=======
					GUI_DispStringAt("transfered data  ", 300, 20); 
					 float curve01=1.957*exp(1.988*num/1000000*4.6/5-0.05);
				  float curve02=-1.76*exp(-3.789*num/1000000*4.6/5-0.05);
				  float curve03=curve01+curve02;
					GUI_DispFloat(curve03,4);
>>>>>>> 593d0de0b42646de75d9ee4c4110e2608e84bbec
					if(channelEnableflag[1])
						stimulate(&huart1, curve03,1);
					break;
				case 6:
					GUI_DispStringAt("channel 2  ", 100, 50); 
					GUI_DispFloat(num/1000000,4);
<<<<<<< HEAD
					GUI_DispStringAt("transferred data  ", 300, 50); 
					GUI_DispFloat(1.946e+04*exp(0.8933*num/1000000*4.6/5-0.25)-1.946e+04*exp(0.8929*num/1000000*4.6/5-0.25),4);
=======
					GUI_DispStringAt("transfered data  ", 300, 50); 
					float curve04=1.946e+04*exp(0.8933*num/1000000*4.6/5-0.125);
				  float curve05=-1.946e+04*exp(0.8929*num/1000000*4.6/5-0.125);
				  float curve06=curve04+curve05;
					GUI_DispFloat(curve06,4);
>>>>>>> 593d0de0b42646de75d9ee4c4110e2608e84bbec
					if(channelEnableflag[2])
						stimulate(&huart3,curve06,2);
					break;
				case 7:
					GUI_DispStringAt("channel 3  ", 100, 80); 
					GUI_DispFloat(num*3.3/5000000,4);
<<<<<<< HEAD
					GUI_DispStringAt("transferred data  ", 300, 80); 
					GUI_DispFloat(2.455*exp(1.891*num*3.3/5000000*5/4.6)-2.977*exp(-3.137*num*3.3/5000000*5/4.6),4);
=======
					GUI_DispStringAt("transfered data  ", 300, 80); 
					float curve07=2.455*exp(1.891*num/1000000*5/4.6-0.3);
          float curve08=-2.977*exp(-3.137*num/1000000*5/4.6-0.3);
				  float curve09=curve07+curve08;
					GUI_DispFloat(curve09,4);
>>>>>>> 593d0de0b42646de75d9ee4c4110e2608e84bbec
					if(channelEnableflag[3])
						stimulate(&huart4,curve09,3);
					break;
				case 8:
					GUI_DispStringAt("channel 4  ", 100, 110); 
					GUI_DispFloat(num,4);
<<<<<<< HEAD
					GUI_DispStringAt("transferred data  ", 300, 110); 
					GUI_DispFloat(22.24*(num-0.43)-0.8579,4);
=======
					GUI_DispStringAt("transfered data  ", 300, 110); 
					GUI_DispFloat(17*(num-0.15),4);
>>>>>>> 593d0de0b42646de75d9ee4c4110e2608e84bbec
					if(channelEnableflag[4])
						stimulate(&huart5,17*(num-0.15),4);
					break;
				case 9:
					GUI_DispStringAt("channel 5  ", 100, 140); 
					GUI_DispFloat(num,4);
<<<<<<< HEAD
					GUI_DispStringAt("transferred data  ", 300, 140); 
					GUI_DispFloat(23.38*(num-0.37)-0.5574,4);
=======
					GUI_DispStringAt("transfered data  ", 300, 140); 
					GUI_DispFloat(17*(num-0.3),4);
>>>>>>> 593d0de0b42646de75d9ee4c4110e2608e84bbec
					if(channelEnableflag[5])
						stimulate(&huart7,17*(num-0.3),5);
					break;
				
				
				default:
					break;
			}	
		}
		
	}
}

void GetAdData(void)
{
	  static int cnt = 0;
		cnt++;
		static int tempcnt = 0;
		//����1-5ͨ��
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
			
			long double temp = (long double)ulResult*0.59604644775390625;
			if(tempcnt<10){
				offset[i] += temp/10;
			}
			if(temp>0&&temp/1000<5000){
				
				ldVolutage[i] = temp-offset[i];
				//ldVolutage[i] = ldVolutage[i]/1000;
				data[i*2+2] = ((u16)(ldVolutage[i]/1000))>>8;
				data[i*2+3] = ((u16)(ldVolutage[i]/1000))&0xFF;
			}
			//HAL_Delay(1);	
		}
		if(cnt==10)
			HAL_UART_Transmit(&huart8,data,15,0xfff);
		
		
		//����6-10ͨ��
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
				
			long double temp = (long double)ulResult*0.59604644775390625;
			
			if(tempcnt<10){
				offset[i] += temp/10;
			}
			
			if(temp>0&&temp/1000<5000){
				ldVolutage[i] = temp-offset[i];
				//ldVolutage[i] = ldVolutage[i]/1000;
				data[i*2-8] = ((u16)(ldVolutage[i]/1000))>>8;
				data[i*2-7] = ((u16)(ldVolutage[i]/1000))&0xFF;
			}

			//HAL_Delay(1);	
		}
	
			ADC_convertedvalue[0] = (float)(ADC_detectedvalue[0]&0xffff)*3.3/65536;
			ADC_convertedvalue[1] = (float)(ADC_detectedvalue[1]&0xffff)*3.3/65536;
		
			
		
			if(tempcnt<10){
				tempcnt++;
				offset[8] += ADC_convertedvalue[0]/10;
				offset[9] += ADC_convertedvalue[1]/10;
			}
			ldVolutage[8] = MeanFilter(ADC_convertedvalue[0],filter0)-offset[8];
			ldVolutage[9] = MeanFilter(ADC_convertedvalue[1],filter1)-offset[9];
		
			data[8] = ((u16)(ldVolutage[8]*1000))>>8;
			data[9] = ((u16)(ldVolutage[8]*1000))&0xFF;
			data[10] =((u16)(ldVolutage[9]*1000))>>8;
			data[11] =((u16)(ldVolutage[9]*1000))&0xFF;
		
			if(cnt==10){
				HAL_UART_Transmit(&huart8,data,15,0xfff);
				HAL_UART_Receive_IT(&huart8,&UART8RxBuff,1);
				cnt = 0;
			}
		
			
}


void MainLoop()
{
	static int last_flag = 0;
	static int clear_flag = 0;
//	u8 t[10];
//	t[0] = 0xaa;
//	t[1] = 0xbb;
//	
//	for(int i = 0;i<5;i++)
//	{
//		HAL_UART_Transmit(&huart8,t,2,0xfff);
//	}
 		
	//VisualScope(&huart1,1,2,3,4);
	//fputc(1,0);
	
	display();
	if(last_flag!=testmode_flag)
		GUI_Clear();
	
	if(testmode_flag>=8){
		testmode_flag = last_flag;//���¼������ֵģʽ��Ӱ����ǰģʽ�Ĺ���
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
			GUI_DispFloat(parameter[1][0]/10.0,4);//������ʾ��λ��
			GUI_DispStringAt("mA  ", 290, 270); 
			GUI_DispFloat(parameter[1][1],4);//������ʾ��λ��
			GUI_DispStringAt("us  ", 410, 270); 
			GUI_DispFloat(parameter[1][2],4);//������ʾ��λ��
			GUI_DispStringAt("hz  ", 520, 270); 
			break;
		
		default:
			break;			
	}
		last_flag = testmode_flag;
}

//��ֵ�˲�
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


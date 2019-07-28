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
enum Hand {left=0,right=1};
enum Mode {zzz = 0,zj = 1};
enum Hand amputatedHand = right;
enum Mode name = zzz;

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
						GUI_DispStringAt("channel 1  ", 100, 150); 
						GUI_DispFloat(num,4);//除以1000000后是实际电压值
						GUI_DispStringAt("transferred data  ", 300, 150); 

//						float curve1=12.04*(num)*(num)*(num)*(num)-24.93*(num)*(num)*(num);//sensor1
//						float curve2=18.57*(num)*(num)+1.145*(num)-1.04;
//						float curve3=curve1+curve2;
//						float curve1=0.7073*(num)*(num)*(num)*(num)+6.861*(num)*(num)*(num);//sensor7
//						float curve2=-9.486*(num)*(num)+13.34*(num)-3.192;
//						float curve3=curve1+curve2;

						float curve1=-9.368*(num)*(num)*(num)*(num)+32.51*(num)*(num)*(num);//sensor10_modified1
						float curve2=-26.39*(num)*(num)+16.84*(num)-2.77;
						float curve3=curve1+curve2;
//						float curve1=9.038*(num)*(num)*(num)*(num)-19.81*(num)*(num)*(num);//sensor10_zjsystem
//						float curve2=24.75*(num)*(num)-5.059*(num)-0.04702;
//						float curve3=curve1+curve2;
//						float curve1=-21.71*(num)*(num)*(num)*(num)+84.42*(num)*(num)*(num);//A201 25lbs sensor35
//						float curve2=-44.53*(num)*(num)+42.89*(num)-5.881;
//						float curve3=curve1+curve2;
//						float curve1=11.61*(num)*(num)*(num);//namisuo sensor6
//						float curve2=-23.74*(num)*(num)+17.8*(num)-4.146;
//						float curve3=curve1+curve2;
//						float curve1=-48.03*(num)*(num)*(num)*(num)+135.8*(num)*(num)*(num);//namisuo sensor1
//						float curve2=-134.5*(num)*(num)+57.13*(num)-8.466;
//						float curve3=curve1+curve2;
						GUI_DispFloat(curve3,4);//sensor20
						//if(channelEnableflag[1])
							stimulate(&huart1,curve3,1);//ok ch1
						break;
					}
					case 1:{//3 sensor3
						GUI_DispStringAt("channel 2  ", 100, 180); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 180); 

//						float curve4=6.006*(num)*(num);//sensor3
//						float curve5=4.532*(num)-1.164;
//						float curve6=curve4+curve5;
//						float curve4=-29.41*(num)*(num)*(num)*(num)+98.32*(num)*(num)*(num);//sensor8
//						float curve5=-101.6*(num)*(num)+50.09*(num)-7.829;
//						float curve6=curve4+curve5;

						float curve4=11.01*(num)*(num)*(num)*(num)-25.4*(num)*(num)*(num);//sensor11_modified1
						float curve5=29.67*(num)*(num)-5.807*(num)+0.2646;
						float curve6=curve4+curve5;	
//					  float curve4=9.925*(num)*(num)*(num)*(num)-16*(num)*(num)*(num);//sensor11_zjsystem
//						float curve5=15.57*(num)*(num)-1.162*(num)-0.4986;
//						float curve6=curve4+curve5;
//						float curve4=-3.045*(num)*(num)*(num)*(num)+44.08*(num)*(num)*(num);//A201 25lbs sensor25
//						float curve5=-32.39*(num)*(num)+38.67*(num)-3.168;
//						float curve6=curve4+curve5;
//						float curve4=8.482*(num)*(num)*(num);//sensor8
//						float curve5=-15.2*(num)*(num)+9.845*(num)-1.995; //namisuo sensor7
//						float curve6=curve4+curve5;
//						float curve4=1.551*(num)*(num)*(num)*(num)+10.15*(num)*(num)*(num);//namisuo sensor2
//						float curve5=-22.6*(num)*(num)+15.59*(num)-3.069;
//						float curve6=curve4+curve5;
						GUI_DispFloat(curve6,4);
						//if(channelEnableflag[2])
							stimulate(&huart3, curve6,2);
						break;
					}
					case 2:{//2 sensor6
						GUI_DispStringAt("channel 3  ", 100, 210); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 210); 

//						float curve7=4.348*(num)*(num)*(num)-3.622*(num)*(num);//sensor6
//						float curve8=9.488*(num)-2.023;
//						float curve9=curve7+curve8;
//						float curve7=1.004*(num)*(num)*(num)*(num)+4.567*(num)*(num)*(num);//sensor9
//						float curve8=-7.793*(num)*(num)+12.99*(num)-2.708;
//						float curve9=curve7+curve8;		
//						float curve7=10.17*(num)*(num)*(num)*(num)-21.69*(num)*(num)*(num);//sensor9_modified1
//						float curve8=21.51*(num)*(num)-3.173*(num)+0.2217;
//						float curve9=curve7+curve8;		
						float curve7=16.67*(num)*(num)*(num)*(num)-39.43*(num)*(num)*(num);//sensor8_modified1
						float curve8=38.47*(num)*(num)-8.183*(num)+0.3524;
						float curve9=curve7+curve8;
//						float curve7=-5.625*(num)*(num)*(num)*(num)+23.75*(num)*(num)*(num);//sensor8_zjsystem
//						float curve8=-15.12*(num)*(num)+9.447*(num)-1.811;
//						float curve9=curve7+curve8;
//						float curve7=-30.89*(num)*(num)*(num)*(num)+99.53*(num)*(num)*(num);//A201 25lbs sensor31
//						float curve8=-55.95*(num)*(num)+53.72*(num)-6.505;
//						float curve9=curve7+curve8;	

//						float curve7=5.223*(num)*(num)*(num)-8.161*(num)*(num);//nnamisuo sensor8
//						float curve8=6.196*(num)-1.177;
//						float curve9=curve7+curve8;	
//						float curve7=-38.96*(num)*(num)*(num)*(num)+114.2*(num)*(num)*(num);//namisuo sensor3
//						float curve8=-112.1*(num)*(num)+46.17*(num)-6.709;
//						float curve9=curve7+curve8;							
						GUI_DispFloat(curve9,4);//sensor22
						//if(channelEnableflag[3])
							stimulate(&huart4,curve9,3);//bad ch3
						break;
					}
					case 3:{//15 sensor1
						GUI_DispStringAt("channel 4  ", 100, 240); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 240); 

//						float curve10=-18.4*(num)*(num)*(num)*(num)+59.65*(num)*(num)*(num);//sensor2
//						float curve11=-61.04*(num)*(num)+33.58*(num)-5.038;
//						float curve12=curve10+curve11;
//						float curve10=4.212*(num)*(num)*(num)*(num)+0.2938*(num)*(num)*(num);//sensor10
//						float curve11=-6.719*(num)*(num)+15.99*(num)-3.489;
//						float curve12=curve10+curve11;
						float curve10=-1.664*(num)*(num)*(num)*(num)+16.54*(num)*(num)*(num);//sensor7_modified1
						float curve11=-18.74*(num)*(num)+16.48*(num)-3.238;
						float curve12=curve10+curve11;
//						float curve10=22.89*(num)*(num)*(num)*(num)-60.87*(num)*(num)*(num);//sensor7_zjsystem
//						float curve11=70.75*(num)*(num)-24.36*(num)+2.574;
//						float curve12=curve10+curve11;
//						float curve10=3.909*(num)*(num)*(num)*(num)+12.39*(num)*(num)*(num);//A201 25lbs sensor34
//						float curve11=17.49*(num)*(num)+21.28*(num)-2.004;
//						float curve12=curve10+curve11;
//						float curve10=-26.66*(num)*(num)*(num)*(num)+80.63*(num)*(num)*(num);//namisuo sensor9
//						float curve11=-83.53*(num)*(num)+36.66*(num)-5.676;
//						float curve12=curve10+curve11;
						GUI_DispFloat(curve12,4);//sensor23
						//if(channelEnableflag[4])
							stimulate(&huart5,curve12,4);
						break;
					}
					case 4:{//1 sensor4
						GUI_DispStringAt("channel 5  ", 100, 270); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 270); 

//						float curve13=-8.86*(num)*(num)*(num)*(num)+30.72*(num)*(num)*(num);//sensor4
//						float curve14=-29.84*(num)*(num)+16.15*(num)-2.713;
//						float curve15=curve13+curve14;	
						
//						float curve13=-3.237*(num)*(num)*(num)*(num)+16.61*(num)*(num)*(num);//sensor11
//						float curve14=-16.17*(num)*(num)+14.18*(num)-3.079;
//						float curve15=curve13+curve14;		
//						float curve13=-3.975*(num)*(num)*(num)*(num)+14.22*(num)*(num)*(num);//sensor12
//						float curve14=-4.909*(num)*(num)+4.908*(num)-1.074;
//						float curve15=curve13+curve14;	

						float curve13=-2.035*(num)*(num)*(num)*(num)+14.54*(num)*(num)*(num);//sensor12_modified1
						float curve14=-13.22*(num)*(num)+11.94*(num)-2.073;
						float curve15=curve13+curve14;
//						float curve13=29.33*(num)*(num)*(num)*(num)-83.12*(num)*(num)*(num);//sensor12_zjsystem
//						float curve14=87.87*(num)*(num)-29.84*(num)+3.258;
//						float curve15=curve13+curve14;
//						float curve13=83.39*(num)*(num)*(num)*(num)-141.3*(num)*(num)*(num);//A201 25lbs sensor37
//						float curve14=116.2*(num)*(num)+5.029*(num)-1.712;
//						float curve15=curve13+curve14;
//						float curve13=-848.4*(num)*(num)*(num)*(num)+1110*(num)*(num)*(num);//namisuo sensor5
//						float curve14=-697.7*(num)*(num)+211.7*(num)-24.87+250.8*(num)*(num)*(num)*(num)*(num);
//						float curve15=curve13+curve14;	
//						float curve13=-19.9*(num)*(num)*(num)*(num)+61*(num)*(num)*(num);//namisuo sensor10
//						float curve14=-63.94*(num)*(num)+29.21*(num)-4.6;
//						float curve15=curve13+curve14;	
//						float curve13=26.62*(num)*(num)*(num);//namisuo sensor5
//						float curve14=-50.28*(num)*(num)+32.63*(num)-6.94;
//						float curve15=curve13+curve14;						
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
						GUI_DispStringAt("channel 1  ", 100, 150); 
						GUI_DispFloat(num,4);//除以1000000后是实际电压值

						GUI_DispStringAt("transferred data  ", 300, 150); 
//						float curve01=1.957*exp(1.988*num*4.6/5-0.05);
//						float curve02=-1.76*exp(-3.789*num*4.6/5-0.05);
//						float curve03=curve01+curve02;
//						float curve01=-572*exp(0.7345*num);//A201 25lbs sensor20
//						float curve02=571.7*exp(0.749*num);
//						float curve03=curve01+curve02;
//						float curve01=10.58*(num)*(num);//A201 25lbs sensor20_zjsystem
//						float curve02=7.375*(num)-1.312;
//						float curve03=curve01+curve02;
//						float curve03=(2.404*exp(1.894*num)*1/1-2.848*exp(-3.094*num)*1/1)*1/1;
						float curve01=4.505*(num)*(num)*(num);//A201 25lbs sensor26
						float curve02=3.267*(num)*(num)+3.478*(num)+0.1523;
						float curve03=curve01+curve02;
						GUI_DispFloat(curve03,4);
						
						stimulate(&huart1, curve03,1);
						break;
					}
					case 6:{
						GUI_DispStringAt("channel 2  ", 100, 180); 
						GUI_DispFloat(num,4);

						GUI_DispStringAt("transferred data  ", 300, 180); 
//						float curve04=1.946e+04*exp(0.8933*num*4.6/5-0.125);
//						float curve05=-1.946e+04*exp(0.8929*num*4.6/5-0.125);
//						float curve06=curve04+curve05;

//						float curve04=1.814*exp(2.017*num);//A201 25lbs sensor21
//						float curve05=-1.785*exp(-2.315*num);
//						float curve06=curve04+curve05;
//						float curve04=1.89*exp(1.921*num);//A201 25lbs sensor21_zjsystem
//						float curve05=-2.864*exp(-1.983*num);
//						float curve06=curve04+curve05;
						float curve04=-35.59*(num)*(num)*(num);//A201 25lbs sensor27
						float curve05=51.93*(num)*(num)+0.1189*(num)+0.648;
						float curve06=curve04+curve05;
						GUI_DispFloat(curve06,4);
			
						stimulate(&huart3,curve06,2);
						break;
					}
					case 7:{
						GUI_DispStringAt("channel 3  ", 100, 210); 
						GUI_DispFloat(num,4);

						GUI_DispStringAt("transferred data  ", 300, 210); 
//						float curve07=2.455*exp(1.891*num*5/4.6-0.3);
//						float curve08=-2.977*exp(-3.137*num*5/4.6-0.3);
//						float curve09=curve07+curve08;
//						float curve07=3.047*exp(1.593*num);//A201 25lbs sensor22
//						float curve08=-3.33*exp(-2.045*num);
//						float curve09=curve07+curve08;
//						float curve07=2470*exp(0.7199*num);//A201 25lbs sensor22_zjsystem
//						float curve08=-2471*exp(0.7168*num);
//						float curve09=curve07+curve08;
						float curve07=-58.81*(num)*(num)*(num)*(num)+112.3*(num)*(num)*(num);//A201 25lbs sensor29
						float curve08=-55.17*(num)*(num)+24.16*(num)-1.139;
						float curve09=curve07+curve08;	
						GUI_DispFloat(curve09,4);
					
						stimulate(&huart4,curve09,3);
						break;
					}
					case 8:{
						GUI_DispStringAt("channel 4  ", 100, 240); 
						GUI_DispFloat(num,4);

						GUI_DispStringAt("transferred data  ", 300, 240); 
//						float curve10=3.118*exp(1.671*num);//A201 25lbs sensor23
//						float curve11=-3.553*exp(-1.261*num);
//						float curve12=curve10+curve11;
//						float curve10=5.451*exp(1.285*num);//A201 25lbs sensor23_zjsystem
//						float curve11=-7.063*exp(-0.4687*num);
//						float curve12=curve10+curve11;
						float curve10=19.7*(num)*(num)*(num)*(num)-28.02*(num)*(num)*(num);//A201 25lbs sensor33
						float curve11=23.44*(num)*(num)+5.416*(num)-1.32;
						float curve12=curve10+curve11;
						GUI_DispFloat(curve12,4);
						
						stimulate(&huart5,curve12,4);
						break;
					}
					case 9:{
						GUI_DispStringAt("channel 5  ", 100, 270); 
						GUI_DispFloat(num,4);

						GUI_DispStringAt("transferred data  ", 300, 270); 
//						float curve13=3.799*exp(1.526*num);//A201 25lbs sensor23
//						float curve14=-3.897*exp(-1.069*num);
//						float curve15=curve13+curve14;
//						float curve13=1.884*exp(2.133*num);//A201 25lbs sensor23_zjsystem
//						float curve14=-3.291*exp(-2.677*num);
//						float curve15=curve13+curve14;
						float curve13=1.792*exp(1.931*num);//A201 25lbs sensor36
						float curve14=-2.049*exp(-3.516*num);
						float curve15=curve13+curve14;
						GUI_DispFloat(curve15,4);

						stimulate(&huart7,curve15,5);
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
						//GUI_Clear();	
						GUI_DispStringAt("channel 1  ", 100, 150); 
						GUI_DispFloat(num,4);//除以1000000后是实际电压值
						GUI_DispStringAt("transferred data  ", 300, 150); 

//						float curve1=12.04*(num)*(num)*(num)*(num)-24.93*(num)*(num)*(num);//sensor1
//						float curve2=18.57*(num)*(num)+1.145*(num)-1.04;
//						float curve3=curve1+curve2;
//						float curve1=0.7073*(num)*(num)*(num)*(num)+6.861*(num)*(num)*(num);//sensor7
//						float curve2=-9.486*(num)*(num)+13.34*(num)-3.192;
//						float curve3=curve1+curve2;

//						float curve1=-9.368*(num)*(num)*(num)*(num)+32.51*(num)*(num)*(num);//sensor10_modified1
//						float curve2=-26.39*(num)*(num)+16.84*(num)-2.77;
//						float curve3=curve1+curve2;
//						float curve1=9.038*(num)*(num)*(num)*(num)-19.81*(num)*(num)*(num);//sensor10_zjsystem
//						float curve2=24.75*(num)*(num)-5.059*(num)-0.04702;
//						float curve3=curve1+curve2;
						float curve1=-21.71*(num)*(num)*(num)*(num)+84.42*(num)*(num)*(num);//A201 25lbs sensor35
						float curve2=-44.53*(num)*(num)+42.89*(num)-5.881;
						float curve3=curve1+curve2;
//						float curve1=11.61*(num)*(num)*(num);//namisuo sensor6
//						float curve2=-23.74*(num)*(num)+17.8*(num)-4.146;
//						float curve3=curve1+curve2;
//						float curve1=-48.03*(num)*(num)*(num)*(num)+135.8*(num)*(num)*(num);//namisuo sensor1
//						float curve2=-134.5*(num)*(num)+57.13*(num)-8.466;
//						float curve3=curve1+curve2;
						GUI_DispFloat(curve3,4);//sensor20
						//if(channelEnableflag[1])
							stimulate(&huart1,curve3,1);//ok ch1
						break;
					}
					case 1:{//3 sensor3
						GUI_DispStringAt("channel 2  ", 100, 180); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 180); 

//						float curve4=6.006*(num)*(num);//sensor3
//						float curve5=4.532*(num)-1.164;
//						float curve6=curve4+curve5;
//						float curve4=-29.41*(num)*(num)*(num)*(num)+98.32*(num)*(num)*(num);//sensor8
//						float curve5=-101.6*(num)*(num)+50.09*(num)-7.829;
//						float curve6=curve4+curve5;

//						float curve4=11.01*(num)*(num)*(num)*(num)-25.4*(num)*(num)*(num);//sensor11_modified1
//						float curve5=29.67*(num)*(num)-5.807*(num)+0.2646;
//						float curve6=curve4+curve5;	
//					  float curve4=9.925*(num)*(num)*(num)*(num)-16*(num)*(num)*(num);//sensor11_zjsystem
//						float curve5=15.57*(num)*(num)-1.162*(num)-0.4986;
//						float curve6=curve4+curve5;
						float curve4=-3.045*(num)*(num)*(num)*(num)+44.08*(num)*(num)*(num);//A201 25lbs sensor25
						float curve5=-32.39*(num)*(num)+38.67*(num)-3.168;
						float curve6=curve4+curve5;
//						float curve4=8.482*(num)*(num)*(num);//sensor8
//						float curve5=-15.2*(num)*(num)+9.845*(num)-1.995; //namisuo sensor7
//						float curve6=curve4+curve5;
//						float curve4=1.551*(num)*(num)*(num)*(num)+10.15*(num)*(num)*(num);//namisuo sensor2
//						float curve5=-22.6*(num)*(num)+15.59*(num)-3.069;
//						float curve6=curve4+curve5;
						GUI_DispFloat(curve6,4);
						//if(channelEnableflag[2])
							stimulate(&huart3, curve6,2);
						break;
					}
					case 2:{//2 sensor6
						GUI_DispStringAt("channel 3  ", 100, 210); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 210); 

//						float curve7=4.348*(num)*(num)*(num)-3.622*(num)*(num);//sensor6
//						float curve8=9.488*(num)-2.023;
//						float curve9=curve7+curve8;
//						float curve7=1.004*(num)*(num)*(num)*(num)+4.567*(num)*(num)*(num);//sensor9
//						float curve8=-7.793*(num)*(num)+12.99*(num)-2.708;
//						float curve9=curve7+curve8;		
//						float curve7=10.17*(num)*(num)*(num)*(num)-21.69*(num)*(num)*(num);//sensor9_modified1
//						float curve8=21.51*(num)*(num)-3.173*(num)+0.2217;
//						float curve9=curve7+curve8;		
//						float curve7=16.67*(num)*(num)*(num)*(num)-39.43*(num)*(num)*(num);//sensor8_modified1
//						float curve8=38.47*(num)*(num)-8.183*(num)+0.3524;
//						float curve9=curve7+curve8;
//						float curve7=-5.625*(num)*(num)*(num)*(num)+23.75*(num)*(num)*(num);//sensor8_zjsystem
//						float curve8=-15.12*(num)*(num)+9.447*(num)-1.811;
//						float curve9=curve7+curve8;
						float curve7=-30.89*(num)*(num)*(num)*(num)+99.53*(num)*(num)*(num);//A201 25lbs sensor31
						float curve8=-55.95*(num)*(num)+53.72*(num)-6.505;
						float curve9=curve7+curve8;	

//						float curve7=5.223*(num)*(num)*(num)-8.161*(num)*(num);//nnamisuo sensor8
//						float curve8=6.196*(num)-1.177;
//						float curve9=curve7+curve8;	
//						float curve7=-38.96*(num)*(num)*(num)*(num)+114.2*(num)*(num)*(num);//namisuo sensor3
//						float curve8=-112.1*(num)*(num)+46.17*(num)-6.709;
//						float curve9=curve7+curve8;							
						GUI_DispFloat(curve9,4);//sensor22
						//if(channelEnableflag[3])
							stimulate(&huart4,curve9,3);//bad ch3
						break;
					}
					case 3:{//15 sensor1
						GUI_DispStringAt("channel 4  ", 100, 240); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 240); 

//						float curve10=-18.4*(num)*(num)*(num)*(num)+59.65*(num)*(num)*(num);//sensor2
//						float curve11=-61.04*(num)*(num)+33.58*(num)-5.038;
//						float curve12=curve10+curve11;
//						float curve10=4.212*(num)*(num)*(num)*(num)+0.2938*(num)*(num)*(num);//sensor10
//						float curve11=-6.719*(num)*(num)+15.99*(num)-3.489;
//						float curve12=curve10+curve11;
//						float curve10=-1.664*(num)*(num)*(num)*(num)+16.54*(num)*(num)*(num);//sensor7_modified1
//						float curve11=-18.74*(num)*(num)+16.48*(num)-3.238;
//						float curve12=curve10+curve11;
//						float curve10=22.89*(num)*(num)*(num)*(num)-60.87*(num)*(num)*(num);//sensor7_zjsystem
//						float curve11=70.75*(num)*(num)-24.36*(num)+2.574;
//						float curve12=curve10+curve11;
						float curve10=3.909*(num)*(num)*(num)*(num)+12.39*(num)*(num)*(num);//A201 25lbs sensor34
						float curve11=17.49*(num)*(num)+21.28*(num)-2.004;
						float curve12=curve10+curve11;
//						float curve10=-26.66*(num)*(num)*(num)*(num)+80.63*(num)*(num)*(num);//namisuo sensor9
//						float curve11=-83.53*(num)*(num)+36.66*(num)-5.676;
//						float curve12=curve10+curve11;
						GUI_DispFloat(curve12,4);//sensor23
						//if(channelEnableflag[4])
							stimulate(&huart5,curve12,4);
						break;
					}
					case 4:{//1 sensor4
						GUI_DispStringAt("channel 5  ", 100, 270); 
						GUI_DispFloat(num,4);
						GUI_DispStringAt("transferred data  ", 300, 270); 

//						float curve13=-8.86*(num)*(num)*(num)*(num)+30.72*(num)*(num)*(num);//sensor4
//						float curve14=-29.84*(num)*(num)+16.15*(num)-2.713;
//						float curve15=curve13+curve14;	
						
//						float curve13=-3.237*(num)*(num)*(num)*(num)+16.61*(num)*(num)*(num);//sensor11
//						float curve14=-16.17*(num)*(num)+14.18*(num)-3.079;
//						float curve15=curve13+curve14;		
//						float curve13=-3.975*(num)*(num)*(num)*(num)+14.22*(num)*(num)*(num);//sensor12
//						float curve14=-4.909*(num)*(num)+4.908*(num)-1.074;
//						float curve15=curve13+curve14;	

//						float curve13=-2.035*(num)*(num)*(num)*(num)+14.54*(num)*(num)*(num);//sensor12_modified1
//						float curve14=-13.22*(num)*(num)+11.94*(num)-2.073;
//						float curve15=curve13+curve14;
//						float curve13=29.33*(num)*(num)*(num)*(num)-83.12*(num)*(num)*(num);//sensor12_zjsystem
//						float curve14=87.87*(num)*(num)-29.84*(num)+3.258;
//						float curve15=curve13+curve14;
						float curve13=83.39*(num)*(num)*(num)*(num)-141.3*(num)*(num)*(num);//A201 25lbs sensor37
						float curve14=116.2*(num)*(num)+5.029*(num)-1.712;
						float curve15=curve13+curve14;
//						float curve13=-848.4*(num)*(num)*(num)*(num)+1110*(num)*(num)*(num);//namisuo sensor5
//						float curve14=-697.7*(num)*(num)+211.7*(num)-24.87+250.8*(num)*(num)*(num)*(num)*(num);
//						float curve15=curve13+curve14;	
//						float curve13=-19.9*(num)*(num)*(num)*(num)+61*(num)*(num)*(num);//namisuo sensor10
//						float curve14=-63.94*(num)*(num)+29.21*(num)-4.6;
//						float curve15=curve13+curve14;	
//						float curve13=26.62*(num)*(num)*(num);//namisuo sensor5
//						float curve14=-50.28*(num)*(num)+32.63*(num)-6.94;
//						float curve15=curve13+curve14;						
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
						GUI_DispStringAt("channel 1  ", 100, 150); 
						GUI_DispFloat(num,4);//除以后是实际电压值

						GUI_DispStringAt("transferred data  ", 300, 150); 
						//double curve01=1.957*exp(1.988*num*4.6/5-0.05);
						//double curve02=-1.76*exp(-3.789*num*4.6/5-0.05);
						double curve01=1.476*exp(1.864*num+0.01984);
						double curve02=-2.169*exp(-13.69*num-1.2321);				
						double curve03=curve01+curve02;
						GUI_DispFloat(curve03,4);
						
						stimulate(&huart1, curve03,1);
						break;
						}
					case 6:{
						GUI_DispStringAt("channel 2  ", 100, 180); 
						GUI_DispFloat(num,4);

						GUI_DispStringAt("transferred data  ", 300, 180); 
						//double curve04=1.946e+04*exp(0.8933*num*4.6/5-0.125);
						//double curve05=-1.946e+04*exp(0.8929*num*4.6/5-0.125);
						double curve04=2.282*exp(1.986*num+0.05958);
						double curve05=-3.907*exp(-5.262*num-0.18996);					
						double curve06=curve04+curve05;
						GUI_DispFloat(curve06,4);

						stimulate(&huart3,curve06,2);
						break;
					}
					case 7:{
						GUI_DispStringAt("channel 3  ", 100, 210); 
						GUI_DispFloat(num,4);

						GUI_DispStringAt("transferred data  ", 300, 210); 
						//double curve07=2.455*exp(1.891*num*5/4.6-0.3);
						//double curve08=-2.977*exp(-3.137*num*5/4.6-0.3);
						double curve07=-0.026*exp(7.94*num-0.1588);
						double curve08=0.8*exp(4.538*num-0.09076);
						double curve09=curve07+curve08;
						GUI_DispFloat(curve09,4);

						stimulate(&huart4,curve09,3);
						break;
					}
					case 8:{
						GUI_DispStringAt("channel 4  ", 100, 240); 
						GUI_DispFloat(num,4);

						GUI_DispStringAt("transferred data  ", 300, 240); 
						GUI_DispFloat(14.1*num-4,4);

						stimulate(&huart5,14.1*num-4,4);
						break;
					}
					case 9:{
						GUI_DispStringAt("channel 5  ", 100, 270); 
						GUI_DispFloat(num,4);

						GUI_DispStringAt("transferred data  ", 300, 270); 
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

//从ADS1256中取8个通道的值，另外从STM32AD端口中取两个通道的值，合在一起。
void GetAdData(void)
{
		static int tempcnt = 0;
		
		HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);//Reset
		ADS1256_Init();//理论上不用再次初始化，这是折衷的做法，否则会出现AD采样偶尔错误的后果。经测试推测是与DSU通信有关。
		HAL_Delay(20);
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
				ldVolutage[i] = MeanFilter(temp,filter[i]);
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
				ldVolutage[i] = MeanFilter(temp,filter[i]);
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


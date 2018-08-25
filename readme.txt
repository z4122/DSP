/**
  @page STemWin_HelloWorld Readme file
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    STemWin/STemWin_HelloWorld/readme.txt 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    29-December-2017    
  * @brief   
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
   @endverbatim

@par Application Description

This directory contains a set of source files that implement a simple "Hello
World" application based on STemWin for STM32H743xx Devices.

The application allows also to run the different Segger samples that can be
downloaded from here:
http://www.segger.com/emwin-samples.html
To do this, user has only to replace the file "Basic_HelloWorld.c" into the
project workspace by the downloaded one.

Note that the following user files may need to be updated:
  LCDConf_stm32h743i_eval.c  
  GUIConf_stm32h743i_eval.c
(if for example more GUI allocated memory is needed)

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note If the application is not running normally as mentionned above , you can accordingly modify either
      the Heap and Stack of the application or the GUI_NUMBYTES define in the GUIConf.c file

@Note If the  application is using the DTCM/ITCM memories (@0x20000000/ 0x0000000: not cacheable and only accessible
      by the Cortex M7 and the  MDMA), no need for cache maintenance when the Cortex M7 and the MDMA access these RAMs.
      If the application needs to use DMA(or other masters) based access or requires more RAM, then  the user has to:
              - Use a non TCM SRAM. (example : D1 AXI-SRAM @ 0x24000000)
              - Add a cache maintenance mechanism to ensure the cache coherence between CPU and other masters(DMAs,DMA2D,LTDC,MDMA).
              - The addresses and the size of cacheable buffers (shared between CPU and other masters)
                must be	properly defined to be aligned to L1-CACHE line size (32 bytes). 
 
@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
      It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
              In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
              Even though the user must manage the cache coherence for read accesses.
              Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
              Please refer to the AN4839 “Level 1 cache on STM32F7 Series”

@par Directory contents 

  - STemWin/STemWin_HelloWorld/STemWin/Target/GUIConf.h                  Header for GUIConf.c
  - STemWin/STemWin_HelloWorld/STemWin/Target/LCDConf.h                  Header for LCDConf*.c
  - STemWin/STemWin_HelloWorld/Core/Inc/main.h                           Main program header file
  - STemWin/STemWin_HelloWorld/Core/Inc/stm32h7xx_hal_conf.h             Library Configuration file
  - STemWin/STemWin_HelloWorld/Core/Inc/stm32h7xx_it.h                   Interrupt handlers header file
  - STemWin/STemWin_HelloWorld/STemWin/App/BASIC_HelloWorld.c            Simple demo drawing "Hello world"
  - STemWin/STemWin_HelloWorld/STemWin/Target/LCDConf_stm32h743i_eval.c  Display controller initialization
  - STemWin/STemWin_HelloWorld/STemWin/Target/GUIConf_stm32h743i_eval.c  Configuration file for the GUI library
  - STemWin/STemWin_HelloWorld/Core/Src/main.c                           Main program file
  - STemWin/STemWin_HelloWorld/Core/Src/stm32h7xx_it.c                   STM32H7xx Interrupt handlers
  - STemWin/STemWin_HelloWorld/Core/Src/system_stm32h7xx.c               STM32H7xx system file
 
 
@par Hardware and Software environment  

  - This application runs on STM32H743xx Devices.
    
  - This example has been tested with STM32H743I-EVAL boards and can be easily 
    tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

#ifndef _AD_H_
#define _AD_H_


#include "stm32h7xx_hal.h"


#define TEST_Pin GPIO_PIN_2
#define TEST_GPIO_Port GPIOE

#define SCK_Pin GPIO_PIN_7
#define SCK_GPIO_Port GPIOB
#define NSS_Pin GPIO_PIN_5
#define NSS_GPIO_Port GPIOD
#define MISO_Pin GPIO_PIN_4
#define MISO_GPIO_Port GPIOB
#define MOSI_Pin GPIO_PIN_6
#define MOSI_GPIO_Port GPIOB
#define RESET_Pin GPIO_PIN_4
#define RESET_GPIO_Port GPIOD
#define DRDY_Pin GPIO_PIN_7
#define DRDY_GPIO_Port GPIOD

//#define SCK_Pin GPIO_PIN_15
//#define SCK_GPIO_Port GPIOF
//#define NSS_Pin GPIO_PIN_9
//#define NSS_GPIO_Port GPIOH
//#define MISO_Pin GPIO_PIN_8
//#define MISO_GPIO_Port GPIOH
//#define MOSI_Pin GPIO_PIN_14
//#define MOSI_GPIO_Port GPIOE
//#define RESET_Pin GPIO_PIN_4
//#define RESET_GPIO_Port GPIOG
//#define DRDY_Pin GPIO_PIN_10
//#define DRDY_GPIO_Port GPIOH

//#define SCK_Pin GPIO_PIN_15
//#define SCK_GPIO_Port GPIOE
//#define NSS_Pin GPIO_PIN_5
//#define NSS_GPIO_Port GPIOG
//#define MISO_Pin GPIO_PIN_11
//#define MISO_GPIO_Port GPIOD
//#define MOSI_Pin GPIO_PIN_13
//#define MOSI_GPIO_Port GPIOE
//#define RESET_Pin GPIO_PIN_11
//#define RESET_GPIO_Port GPIOH
//#define DRDY_Pin GPIO_PIN_12
//#define DRDY_GPIO_Port GPIOD

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

 /*Registers' Address*/
#define REG_STATUS  0
#define REG_MUX     1
#define REG_ADCON   2
#define REG_DRATE   3
#define REG_IO      4
#define REG_OFC0    5
#define REG_OFC1    6
#define REG_OPC2    7
#define REG_FSC0    8
#define REG_FSC1    9
#define REG_FSC2    10

/*Operation Command*/
//#define CMD_WAKEUP     0x00
#define CMD_RDATA      0x01
#define CMD_RDATAC     0x03
#define CMD_SDATAC     0x0F
#define CMD_RREG       0x10
#define CMD_WREG       0x50
#define CMD_SELFCAL    0xf0
#define CMD_SELFOCAL   0xf1
#define CMD_SELFGCAL   0xf2
#define CMD_SYSOCAL    0xf3
#define CMD_SYSGCAL    0xf4
#define CMD_SYNC       0xfc
#define CMD_STANDBY    0xfd
#define CMD_RESET      0xfe
#define CMD_WAKEUP     0xFF

#define PGA_1            0x00
#define PGA_2            0x01
#define PGA_4            0x02
#define PGA_8            0x03
#define PGA_16           0x04
#define PGA_32           0x05
#define PGA_64           0x06
//#define PGA_64           0x07


#define POSITIVE_AIN0            (0X00<<4)
#define POSITIVE_AIN1            (0X01<<4)
#define POSITIVE_AIN2            (0X02<<4)
#define POSITIVE_AIN3            (0X03<<4)
#define POSITIVE_AIN4            (0X04<<4)
#define POSITIVE_AIN5            (0X05<<4)
#define POSITIVE_AIN6            (0X06<<4)
#define POSITIVE_AIN7            (0X07<<4)
#define POSITIVE_AINCOM          (0X08<<4)        

#define NEGTIVE_AIN0              0X00
#define NEGTIVE_AIN1              0X01
#define NEGTIVE_AIN2              0X02
#define NEGTIVE_AIN3              0X03
#define NEGTIVE_AIN4              0X04
#define NEGTIVE_AIN5              0X05
#define NEGTIVE_AIN6              0X06
#define NEGTIVE_AIN7              0X07
#define NEGTIVE_AINCOM            0X08

/*For fclkin=7.68MHz, data rate*/
#define DATARATE_30K              0xf0
#define DATARATE_15K              0xe0
#define DATARATE_7_5K             0xd0
#define DATARATE_3_7_5K           0xc0
#define DATARATE_2K               0xb0

/*STATUS REGISTER*/
#define MSB_FRIST                (0x00<<3)
#define LSB_FRIST                (0x01<<3)
#define ACAL_OFF                 (0x00<<2)
#define ACAL_ON                  (0x01<<2)
#define BUFEN_OFF                (0x00<<1)
#define BUFEN_ON                 (0x01<<1)

/*ADCON REGISTER*/
#define CLKOUT_OFF               (0x00<<5)
#define CLKOUT_CLKIN             (0x01<<5)
#define DETECT_OFF               (0x00<<3)
#define DETECT_ON_2UA            (0x02<<3)

/*GPIO REGISTER*/
/*
#define DIR3_OUT                 (~(0x1<<7)) 
#define DIR3_IN                  (0x1<<7) 
#define DIR2_OUT                 (~(0x1<<6)) 
#define DIR2_IN                  (0x1<<6) 
#define DIR1_OUT                 (~(0x1<<5)) 
#define DIR1_IN                  (0x1<<5) 
*/




#define ADS1256_DRDY    HAL_GPIO_ReadPin(DRDY_GPIO_Port,DRDY_Pin)   //	GPIOE->IDR∂¡∂Àø⁄Eµƒ ˝æ›ºƒ¥Ê∆˜
#define ADS1256_DO      HAL_GPIO_ReadPin(MISO_GPIO_Port,MISO_Pin) 

#define SetADS1256_SYNC   HAL_GPIO_WritePin(RESET_GPIO_Port,RESET_Pin,GPIO_PIN_SET)
#define SetADS1256_CS     HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET)
#define SetADS1256_IN     HAL_GPIO_WritePin(MOSI_GPIO_Port,MOSI_Pin,GPIO_PIN_SET)
#define SetADS1256_CLK    HAL_GPIO_WritePin(SCK_GPIO_Port,SCK_Pin,GPIO_PIN_SET);HAL_GPIO_WritePin(TEST_GPIO_Port,TEST_Pin,GPIO_PIN_SET)

#define ClrADS1256_SYNC   HAL_GPIO_WritePin(RESET_GPIO_Port,RESET_Pin,GPIO_PIN_RESET)
#define ClrADS1256_CS     HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET) 
#define ClrADS1256_IN     HAL_GPIO_WritePin(MOSI_GPIO_Port,MOSI_Pin,GPIO_PIN_RESET)
#define ClrADS1256_CLK    HAL_GPIO_WritePin(SCK_GPIO_Port,SCK_Pin,GPIO_PIN_RESET);HAL_GPIO_WritePin(TEST_GPIO_Port,TEST_Pin,GPIO_PIN_RESET)

extern long double offset[10];
extern long double ldVolutage[10];//Â≠òÂÇ®ÂçÅ‰∏™ÈÄöÈÅìÁöÑÂÄº
extern double ADC_convertedvalue[2];//ËΩ¨Êç¢‰∫ÜÈÄöÈÅìÁöÑÂéüÂßãADÂÄºÂêéÁöÑÂÄº

void Readad(void);
void AD_Init(void);

void ADS1256_Init(void);
void AD_GPIO_Init(void);
unsigned int ADS1256ReadData(void);
unsigned int ADS_sum(unsigned char);
unsigned char ADS1256_Read_Byte(void);
void ADS1256_Write_Byte(unsigned char);
unsigned char ADS1256_Read_one_Reg(unsigned char);
void ADS1256_Write_Reg(unsigned char, unsigned char);

void delayad_nopar(void);
void delayad(unsigned);


#endif

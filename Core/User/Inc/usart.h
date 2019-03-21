#include "stm32h7xx_hal.h"
#include "stm32h743i_eval.h"
#include "visualscope.h"
#include "GUI.h"




/* Definition for USARTx Pins */


#define TXSTARTMESSAGESIZE               (COUNTOF(aTxStartMessage) - 1)
#define TXENDMESSAGESIZE                 (COUNTOF(aTxEndMessage) - 1)

#define RXBUFFERSIZE                     2


#define USART1_RESET_PORT									GPIOB
#define USART1_RESET_PIN									GPIO_PIN_11
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart8;


extern uint8_t volatile UART1RxBuff; 
extern uint8_t volatile UART3RxBuff; 
extern uint8_t volatile UART4RxBuff; 
extern uint8_t volatile UART5RxBuff; 
extern uint8_t volatile UART7RxBuff; 
extern uint8_t volatile UART8RxBuff; 


extern uint8_t tempRxBuffer;
extern uint8_t upperRxBuffer[7];
extern volatile int testmode_flag;
extern int channelchange;
extern u16 parameter[6][5];
extern u16 threshold[6][6];
void USART1_Init(void);
void USART2_Init(void);
void USART3_Init(void);
void UART4_Init(void);
void UART5_Init(void);
void USART6_Init(void);
void UART7_Init(void);
void UART8_Init(void);

void Reset_Init(void);

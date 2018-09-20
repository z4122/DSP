
#include "fes.h"


uint8_t stimulate_parameter[20]="";

float value[17][2];


configtx txconfig = {
					0x80,			//communication header
					0x02,			//transmit data stream
					0x01, 			//default channel is 1st channel
					0x000C, 		//length is 12
					0x003C, 		//default time width is 50us = 0x32H
					0x0032, 		//default frequeny is 100 Hz = 0x0064H
					0x0A, 			//default time delay is 10us = 0x0AH
					0x01, 			//no circulation
					0x0001,			//stimulate times is 4
					0x00,			//increment numerator is 0
					0x0001,			//increment denominator is 1
					0x50,			//default stimulate amplitude is 1mA 0X0A
					0xFF,			//end flag,should be "FF FF FF"
			};


void merge_stimulate_parameter(UART_HandleTypeDef *huart,char amplitude)
{			
	stimulate_parameter[0]=txconfig.HEADER;	
	stimulate_parameter[1]=txconfig.CONTROL;
	stimulate_parameter[2]=txconfig.CHANNEL;
	stimulate_parameter[3]=txconfig.LENGTH>>8;
	stimulate_parameter[4]=txconfig.LENGTH;
	

	
	stimulate_parameter[5]=txconfig.WIDTH>>8;
	stimulate_parameter[6]=txconfig.WIDTH;
	stimulate_parameter[7]=txconfig.FREQUENCY>>8;
	stimulate_parameter[8]=txconfig.FREQUENCY;
	stimulate_parameter[9]=txconfig.DELAY;
	stimulate_parameter[10]=txconfig.CIRCULATE;
	stimulate_parameter[11]=txconfig.TIMES>>8;
	stimulate_parameter[12]=txconfig.TIMES;
	stimulate_parameter[13]=txconfig.NUMERATOR;
	stimulate_parameter[14]=txconfig.DENOMINATOR>>8;
	stimulate_parameter[15]=txconfig.DENOMINATOR;
	//stimulate_parameter[16]=txconfig.AMPLITUDE;
	stimulate_parameter[16]=amplitude;
	
	if(testmode_flag==4)
	{
		if(huart==&huart5) {
			stimulate_parameter[16] = parameter[1][0]; 
			stimulate_parameter[5] = parameter[1][1]>>8; //脉宽高位
			stimulate_parameter[6] = parameter[1][1]; //脉宽低位
			stimulate_parameter[7] = parameter[1][2]>>8; //频率高位
			stimulate_parameter[8] = parameter[1][2]; //频率低位
			
		}
		else if(huart==&huart3) {
			stimulate_parameter[16] = parameter[2][0]; 
			stimulate_parameter[5] = parameter[2][1]>>8; //脉宽高位
			stimulate_parameter[6] = parameter[2][1]; //脉宽低位
			stimulate_parameter[7] = parameter[2][2]>>8; //频率高位
			stimulate_parameter[8] = parameter[2][2]; //频率低位
			
		}
		else if(huart==&huart1) {
			stimulate_parameter[16] = parameter[3][0]; 
			stimulate_parameter[5] = parameter[3][1]>>8; //脉宽高位
			stimulate_parameter[6] = parameter[3][1]; //脉宽低位
			stimulate_parameter[7] = parameter[3][2]>>8; //频率高位
			stimulate_parameter[8] = parameter[3][2]; //频率低位
			
		}
		else if(huart==&huart4) {
			stimulate_parameter[16] = parameter[4][0]; 
			stimulate_parameter[5] = parameter[4][1]>>8; //脉宽高位
			stimulate_parameter[6] = parameter[4][1]; //脉宽低位
			stimulate_parameter[7] = parameter[4][2]>>8; //频率高位
			stimulate_parameter[8] = parameter[4][2]; //频率低位
			
		}
		else if(huart==&huart7)  {
			stimulate_parameter[16] = parameter[5][0]; 
			stimulate_parameter[5] = parameter[5][1]>>8; //脉宽高位
			stimulate_parameter[6] = parameter[5][1]; //脉宽低位
			stimulate_parameter[7] = parameter[5][2]>>8; //频率高位
			stimulate_parameter[8] = parameter[5][2]; //频率低位
			
		}
	}
	
	
	stimulate_parameter[17]=txconfig.ENDFLAG;
	stimulate_parameter[18]=txconfig.ENDFLAG;
	stimulate_parameter[19]=txconfig.ENDFLAG;
	
	HAL_UART_Transmit(huart,(uint8_t *)stimulate_parameter,sizeof(stimulate_parameter),0xffff);
}





void stim_search(UART_HandleTypeDef *huart)
{
	char temp[3] = {0x80,0x01,0x03};
	
	if(huart->Instance==USART1)
	{
		while(UART1RxBuff!=0xBC)
		{
			HAL_UART_Receive_IT(huart,&UART1RxBuff,1);
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_Delay(10);		
		}
		UART1RxBuff = 0x00;

	}
	else if(huart->Instance==USART3)
	{
		while((UART3RxBuff!=0xBC))
		{
			HAL_UART_Receive_IT(huart,&UART3RxBuff,1);
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_Delay(10);
		}
		UART3RxBuff = 0x00;

	}
	else if(huart->Instance==UART4)
	{
		while((UART4RxBuff!=0xBC))
		{
			HAL_UART_Receive_IT(huart,&UART4RxBuff,1);
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_Delay(10);
		}
		UART4RxBuff = 0x00;

	}
	else if(huart->Instance==UART5)
	{
		while((UART5RxBuff!=0xBC))
		{
			HAL_UART_Receive_IT(huart,&UART5RxBuff,1);
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_Delay(10);
		}
			
		UART5RxBuff = 0x00;

	}
	else if(huart->Instance==UART7)
	{
		while((UART7RxBuff!=0xBC))
		{
			HAL_UART_Receive_IT(huart,&UART7RxBuff,1);
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_Delay(10);
		}
		UART7RxBuff = 0x00;

	}
	else if(huart->Instance==UART8)
	{
		while((UART8RxBuff!=0xBC))
		{
			HAL_UART_Receive_IT(huart,&UART8RxBuff,1);
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_Delay(10);
		}
		UART8RxBuff = 0x00;

	}
		
}


void stim_start(UART_HandleTypeDef *huart) //receive AA 0x41 0x41
{
	char temp[3] = {0x80,0x01,0x01};

	if(huart->Instance==USART1)
	{
		while((UART1RxBuff!=0xAA)&&(UART1RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,&UART1RxBuff,1);
			HAL_Delay(50);
		}
		UART1RxBuff = 0x00;

	}
	else if(huart->Instance==USART3)
	{
		while((UART3RxBuff!=0xAA)&&(UART3RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,&UART3RxBuff,1);
			HAL_Delay(50);
		}
		UART3RxBuff = 0x00;

	}
	else if(huart->Instance==UART4)
	{
		while((UART4RxBuff!=0xAA)&&(UART4RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,&UART4RxBuff,1);
			HAL_Delay(50);
		}
		UART4RxBuff = 0x00;

	}
	else if(huart->Instance==UART5)
	{
		while((UART5RxBuff!=0xAA)&&(UART5RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,&UART5RxBuff,1);
			HAL_Delay(50);
		}
		UART5RxBuff = 0x00;

	}
	else if(huart->Instance==UART7)
	{
		while((UART7RxBuff!=0xAA)&&(UART7RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,&UART7RxBuff,1);
			HAL_Delay(50);
		}
		UART7RxBuff = 0x00;

	}
	else if(huart->Instance==UART8)
	{
		while((UART8RxBuff!=0xAA)&&(UART8RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,&UART8RxBuff,1);
			HAL_Delay(50);
		}
		UART8RxBuff = 0x00;

	}
		
}


void stim_stop(UART_HandleTypeDef *huart)// BB 0x42 0x42
{
	char temp_1 = 0x80;
	char temp_2 = 0x01;
	char temp_3 = 0x02;
	HAL_UART_Receive_IT(&huart3,&UART3RxBuff,1);
	HAL_UART_Receive_IT(&huart4,&UART4RxBuff,1);
	HAL_UART_Receive_IT(&huart5,&UART5RxBuff,1);
	HAL_UART_Receive_IT(&huart7,&UART7RxBuff,1);
	HAL_UART_Receive_IT(&huart8,&UART8RxBuff,1);

	if(huart->Instance==USART1)
	{
		while((UART1RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp_1,1,0xffff);
			HAL_UART_Transmit(huart,(uint8_t *)&temp_2,1,0xffff);
			HAL_UART_Transmit(huart,(uint8_t *)&temp_3,1,0xffff);
			HAL_UART_Receive_IT(&huart1,&UART1RxBuff,1);
			HAL_Delay(1);
		}
		UART1RxBuff = 0x00;

	}
	else if(huart->Instance==USART3)
	{

		while((UART3RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp_1,1,0xffff);
			HAL_UART_Transmit(huart,(uint8_t *)&temp_2,1,0xffff);
			HAL_UART_Transmit(huart,(uint8_t *)&temp_3,1,0xffff);
		}
		UART3RxBuff = 0x00;

	}
	else if(huart->Instance==UART4)
	{
		while((UART4RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp_1,1,0xffff);
			HAL_UART_Transmit(huart,(uint8_t *)&temp_2,1,0xffff);
			HAL_UART_Transmit(huart,(uint8_t *)&temp_3,1,0xffff);
		}
		UART4RxBuff = 0x00;

	}
	else if(huart->Instance==UART5)
	{
		while((UART5RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp_1,1,0xffff);
			HAL_UART_Transmit(huart,(uint8_t *)&temp_2,1,0xffff);
			HAL_UART_Transmit(huart,(uint8_t *)&temp_3,1,0xffff);
		}
		UART5RxBuff = 0x00;

	}
	else if(huart->Instance==UART7)
	{
		while((UART7RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp_1,1,0xffff);
			HAL_UART_Transmit(huart,(uint8_t *)&temp_2,1,0xffff);
			HAL_UART_Transmit(huart,(uint8_t *)&temp_3,1,0xffff);
		}
		UART7RxBuff = 0x00;

	}
	else if(huart->Instance==UART8)
	{
		while((UART8RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp_1,1,0xffff);
			HAL_UART_Transmit(huart,(uint8_t *)&temp_2,1,0xffff);
			HAL_UART_Transmit(huart,(uint8_t *)&temp_3,1,0xffff);
		}
		UART8RxBuff = 0x00;

	}
}

void stimulate(UART_HandleTypeDef *huart,float amp)
{
	
	stim_search(huart);// send 800103 back BC
	if(amp>=100)
		amp=99;
	merge_stimulate_parameter(huart,(int)amp);
	
	stim_start(huart);//800101,back AA
	
	//HAL_Delay(1000);

	//stim_stop(huart);//800102,back BB
	

}

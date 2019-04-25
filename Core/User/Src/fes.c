#include "fes.h"


uint8_t stimulate_parameter[20]="";

float value[17][2];
int maxpressure = 15;//最大的压力阈值

extern void ChangePeriod(u16 period);

configtx txconfig = {
					0x80,			//communication header
					0x02,			//transmit data stream
					0x01, 			//default channel is 1st channel
					0x000C, 		//length is 12
					0x0064, 		//default time width is 50us = 0x32H
					0x0064, 		//default frequeny is 100 Hz = 0x0064H
					0x0A, 			//default time delay is 10us = 0x0AH
					0x01, 			//no circulation
					//0x0055,			//stimulate times is 4
					0x0001,
					0x00,			//increment numerator is 0
					0x0001,			//increment denominator is 1
					0x0A,			//default stimulate amplitude is 1mA 0X0A
					0xFF,			//end flag,should be "FF FF FF"
};


void merge_stimulate_parameter(UART_HandleTypeDef *huart,double pressure,int channel)
{			
	if(pressure<0)
		pressure = 0;
	if(pressure>maxpressure)
		pressure = maxpressure;
	
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
	stimulate_parameter[16]=txconfig.AMPLITUDE;
	stimulate_parameter[17]=txconfig.ENDFLAG;
	stimulate_parameter[18]=txconfig.ENDFLAG;
	stimulate_parameter[19]=txconfig.ENDFLAG;
	
	//flag==1||4幅值跟随模式，flag==2||5频率跟随模式，flag==3||6脉宽跟随模式,flag==7自由测试模式
	if(testmode_flag==1||testmode_flag==4){
		stimulate_parameter[16] = (threshold[channel][1]-threshold[channel][0])*(float)pressure/maxpressure+threshold[channel][0];
	}
	else if(testmode_flag==2||testmode_flag==5){
		stimulate_parameter[7] = (int)(25+pressure/2)>>8; //频率高8位
		stimulate_parameter[8] = (int)(25+pressure/2); //频率低8位
	}		
	else if(testmode_flag==3||testmode_flag==6){
		float temp = (threshold[channel][3]-threshold[channel][2])*(float)pressure/maxpressure+threshold[channel][2];
		u16 val = (u16)temp;
		val*=10;
		if(val<20)
			val=20;
		stimulate_parameter[5] = val>>8; //脉宽高8位
		stimulate_parameter[6] = val; //脉宽低8位
		stimulate_parameter[16]=threshold[channel][0];//配合脉宽模式和诱发指感区的最低电流阈值，在脉宽模式下也会设置
	}
	else if(testmode_flag==7)
	{
		static u16 last_frequency[6] = {0};
		if(last_frequency[channel] != parameter[channel][2]){
			last_frequency[channel]=parameter[channel][2];
			if(last_frequency[channel]>=10000){
				ChangePeriod(last_frequency[channel]-10000);//改变发送给DSSU的信号周期，单位ms
				stimulate_parameter[7] = txconfig.FREQUENCY>>8; //频率高位，默认100hz
				stimulate_parameter[8] = txconfig.FREQUENCY; //频率低位
				stimulate_parameter[11]= 0x00;//次数高位
				stimulate_parameter[12]= 0x01;//次数低位
			}
			else{
				ChangePeriod(50);//单位ms，周期20ms
				stimulate_parameter[7] = parameter[1][2]>>8; //频率高位
				stimulate_parameter[8] = parameter[1][2]; //频率低位
				stimulate_parameter[11]= (unsigned int)(last_frequency[channel]*0.05)>>8;//次数高位，计算出来的刺激次数，如200hz就是200*0.02=4次
				stimulate_parameter[12]= (unsigned int)last_frequency[channel]*0.05;//次数低位
			}
		}
		stimulate_parameter[16] = parameter[channel][0]; 
		stimulate_parameter[5] = parameter[channel][1]>>8; //脉宽高8位
		stimulate_parameter[6] = parameter[channel][1]; //脉宽低8位
	}
		
	HAL_UART_Transmit(huart,(uint8_t *)stimulate_parameter,sizeof(stimulate_parameter),0xffff);
}





void stim_search(UART_HandleTypeDef *huart)
{
	char temp[3] = {0x80,0x01,0x03};
	
	if(huart->Instance==USART1)
	{
		while(UART1RxBuff!=0xBC)
		{
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART1RxBuff,1);
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_Delay(1);		
		}
		UART1RxBuff = 0x00;

	}
	else if(huart->Instance==USART3)
	{
		while((UART3RxBuff!=0xBC))
		{
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART3RxBuff,1);
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_Delay(1);
		}
		UART3RxBuff = 0x00;

	}
	else if(huart->Instance==UART4)
	{
		while((UART4RxBuff!=0xBC))
		{
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART4RxBuff,1);
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_Delay(1);
		}
		UART4RxBuff = 0x00;

	}
	else if(huart->Instance==UART5)
	{
		while((UART5RxBuff!=0xBC))
		{
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART5RxBuff,1);
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_Delay(1);
		}
			
		UART5RxBuff = 0x00;

	}
	else if(huart->Instance==UART7)
	{
		while((UART7RxBuff!=0xBC))
		{
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART7RxBuff,1);
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_Delay(1);
		}
		UART7RxBuff = 0x00;

	}
	else if(huart->Instance==UART8)
	{
		while((UART8RxBuff!=0xBC))
		{
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART8RxBuff,1);
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_Delay(1);
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
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART1RxBuff,1);
			HAL_Delay(1);
		}
		UART1RxBuff = 0x00;

	}
	else if(huart->Instance==USART3)
	{
		while((UART3RxBuff!=0xAA)&&(UART3RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART3RxBuff,1);
			HAL_Delay(1);
		}
		UART3RxBuff = 0x00;

	}
	else if(huart->Instance==UART4)
	{
		while((UART4RxBuff!=0xAA)&&(UART4RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART4RxBuff,1);
			HAL_Delay(1);
		}
		UART4RxBuff = 0x00;

	}
	else if(huart->Instance==UART5)
	{
		while((UART5RxBuff!=0xAA)&&(UART5RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART5RxBuff,1);
			HAL_Delay(1);
		}
		UART5RxBuff = 0x00;

	}
	else if(huart->Instance==UART7)
	{
		while((UART7RxBuff!=0xAA)&&(UART7RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART7RxBuff,1);
			HAL_Delay(1);
		}
		UART7RxBuff = 0x00;

	}
	else if(huart->Instance==UART8)
	{
		while((UART8RxBuff!=0xAA)&&(UART8RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART8RxBuff,1);
			HAL_Delay(1);
		}
		UART8RxBuff = 0x00;

	}
		
}


void stim_stop(UART_HandleTypeDef *huart)// BB 0x42 0x42
{
	char temp_1 = 0x80;
	char temp_2 = 0x01;
	char temp_3 = 0x02;
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&UART1RxBuff,1);
	HAL_UART_Receive_IT(&huart3,(uint8_t *)&UART3RxBuff,1);
	HAL_UART_Receive_IT(&huart4,(uint8_t *)&UART4RxBuff,1);
	HAL_UART_Receive_IT(&huart5,(uint8_t *)&UART5RxBuff,1);
	HAL_UART_Receive_IT(&huart7,(uint8_t *)&UART7RxBuff,1);

	if(huart->Instance==USART1)
	{
		while((UART1RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp_1,1,0xffff);
			HAL_UART_Transmit(huart,(uint8_t *)&temp_2,1,0xffff);
			HAL_UART_Transmit(huart,(uint8_t *)&temp_3,1,0xffff);
			HAL_UART_Receive_IT(&huart1,(uint8_t *)&UART1RxBuff,1);
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

void stimulate(UART_HandleTypeDef *huart,double pressure,int channel)
{
	//当通道选择上并且模式不是停止模式时
	if(channelEnableflag[channel]==1&&testmode_flag!=0){
		if(pressure>pressureThreshold){
			stim_search(huart);// send 800103 back BC
		
			merge_stimulate_parameter(huart,pressure,channel);
		
			stim_start(huart);//800101,back AA
		
			//HAL_Delay(1000);

			//stim_stop(huart);//800102,back BB
		}
	}

}

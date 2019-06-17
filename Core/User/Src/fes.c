#include "fes.h"

uint8_t stimulate_parameter[20]="";

float value[17][2];
float maxpressure = 15;//最大的压力阈值

extern void ChangePeriod(u16 period);

configtx txconfig = {
					0x80,			//communication header
					0x02,			//transmit data stream
					0x01, 			//default channel is 1st channel
					0x000C, 		//length is 12
					0x0064, 		//default time width is 100us = 0x64H
					0x0064, 		//default frequeny is 100 Hz = 0x0064H
					0x0A, 			//default time delay is 10us = 0x0AH
					0x01, 			//no circulation
					0xFFFF,			//stimulate times is max
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
	
	
	
	stimulate_parameter[5] = parameter[channel][1]>>8; //脉宽高8位
	stimulate_parameter[6] = parameter[channel][1]; //脉宽低8位	
	stimulate_parameter[7] = parameter[channel][2]>>8; //频率高位
	stimulate_parameter[8] = parameter[channel][2]; //频率低位
	stimulate_parameter[11] = 0x00; //次数高八位赋0
	stimulate_parameter[16] = parameter[channel][0]; //幅值
	
	if(parameter[channel][2]==200)
		stimulate_parameter[12] = 0x12;//次数
	else if(parameter[channel][2]==100)
		stimulate_parameter[12] = 0x0A;//次数
	else if(parameter[channel][2]==50)
		stimulate_parameter[12] = 0x05;//次数
	else if(parameter[channel][2]==20)
		stimulate_parameter[12] = 0x02;//次数
	else if(parameter[channel][2]==20)
		stimulate_parameter[12] = 0x01;//次数



	//flag==1||4幅值跟随模式，flag==2||5频率跟随模式，flag==3||6脉宽跟随模式,flag==7自由测试模式
	if(testmode_flag==1||testmode_flag==4){
		stimulate_parameter[16] = (threshold[channel][1]-threshold[channel][0])*(float)(pressure-pressureThreshold[channel])/(maxpressure-pressureThreshold[channel])+threshold[channel][0];
	}
	else if(testmode_flag==2||testmode_flag==5){
		stimulate_parameter[7] = (int)(25+pressure/2)>>8; //频率高8位
		stimulate_parameter[8] = (int)(25+pressure/2); //频率低8位
	}		
	else if(testmode_flag==3||testmode_flag==6){
		float temp = (threshold[channel][3]-threshold[channel][2])*(float)(pressure-pressureThreshold[channel])/(maxpressure-pressureThreshold[channel])+threshold[channel][2];
		u16 val = (u16)temp;
		val*=10;
		if(val<20)
			val=20;
		stimulate_parameter[5] = val>>8; //脉宽高8位
		stimulate_parameter[6] = val; //脉宽低8位
		stimulate_parameter[16]=threshold[channel][0];//配合脉宽模式和诱发指感区的最低电流阈值，在脉宽模式下也会设置
	}
		
	HAL_UART_Transmit(huart,(uint8_t *)stimulate_parameter,sizeof(stimulate_parameter),0xffff);
}

void merge_stimulate_parameter_freerun(UART_HandleTypeDef *huart,int channel)
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
	stimulate_parameter[16]=txconfig.AMPLITUDE;
	stimulate_parameter[17]=txconfig.ENDFLAG;
	stimulate_parameter[18]=txconfig.ENDFLAG;
	stimulate_parameter[19]=txconfig.ENDFLAG;
	
	
	//stimulate_parameter[10] = 0x00;
	stimulate_parameter[5] = parameter[channel][1]>>8; //脉宽高8位
	stimulate_parameter[6] = parameter[channel][1]; //脉宽低8位	
	stimulate_parameter[7] = parameter[channel][2]>>8; //频率高位
	stimulate_parameter[8] = parameter[channel][2]; //频率低位
	stimulate_parameter[16] = parameter[channel][0]; //幅值

	HAL_UART_Transmit(huart,(uint8_t *)stimulate_parameter,sizeof(stimulate_parameter),0xffff);
}

void Delay_nopar(void)
{
	unsigned long i = 10000;//2000
   	while(i--);
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

	char temp[3] = {0x80,0x01,0x02};

	if(huart->Instance==USART1)
	{
		//while((UART1RxBuff!=0xAA)&&(UART1RxBuff!=0xBB))
		//{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART1RxBuff,1);
			//HAL_Delay(1);
		//}
		UART1RxBuff = 0x00;
	}
	else if(huart->Instance==USART3)
	{
		//while((UART3RxBuff!=0xAA)&&(UART3RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART3RxBuff,1);
			//HAL_Delay(1);
		}
		UART3RxBuff = 0x00;
	}
	else if(huart->Instance==UART4)
	{
		//while((UART4RxBuff!=0xAA)&&(UART4RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART4RxBuff,1);
			//HAL_Delay(1);
		}
		UART4RxBuff = 0x00;
	}
	else if(huart->Instance==UART5)
	{
		//while((UART5RxBuff!=0xAA)&&(UART5RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART5RxBuff,1);
			//HAL_Delay(1);
		}
		UART5RxBuff = 0x00;
	}
	else if(huart->Instance==UART7)
	{
		//while((UART7RxBuff!=0xAA)&&(UART7RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART7RxBuff,1);
			//HAL_Delay(1);
		}
		UART7RxBuff = 0x00;
	}
	else if(huart->Instance==UART8)
	{
		//while((UART8RxBuff!=0xAA)&&(UART8RxBuff!=0xBB))
		{
			HAL_UART_Transmit(huart,(uint8_t *)&temp,3,0xffff);
			HAL_UART_Receive_IT(huart,(uint8_t *)&UART8RxBuff,1);
			//HAL_Delay(1);
		}
		UART8RxBuff = 0x00;
	}
}

void stimulate(UART_HandleTypeDef *huart,double pressure,int channel)
{
	//stim_search(huart);// send 800103 back BC
	
	//channelEnableflag[channel]=1;
	//当通道选择上并且模式不是停止模式时
	if(channelEnableflag[channel]==1&&testmode_flag!=0){	
		if(pressure>pressureThreshold[channel]){
			stim_stop(huart);//800102, no feedback
			stim_stop(huart);//800102, no feedback
			stim_stop(huart);//800102, no feedback
			stim_stop(huart);//800102, no feedback
			stim_stop(huart);//800102, no feedback
			stim_stop(huart);//800102, no feedback
			stim_stop(huart);//800102, no feedback

			//HAL_Delay(1);
			stim_stop(huart);//800102, no feedback
			//HAL_Delay(1);
			stim_stop(huart);//800102, no feedback
			//HAL_Delay(20);//在stop后必须加一个延时否则DUS会崩掉。
			merge_stimulate_parameter(huart,pressure,channel);
		
			stim_start(huart);//800101,back AA
		
			//HAL_Delay(10);		
		}
	}
}

void stimulateFreeRun(UART_HandleTypeDef *huart,int channel){
		if(channelEnableflag[channel]==1){
			stim_search(huart);// send 800103 back BC
			merge_stimulate_parameter_freerun(huart,channel);
			stim_start(huart);//800101,back AA
		}
}

void FreeRun_Init(){
	while(1){
		if(testmode_flag==7&&startFlag==1){
			stimulateFreeRun(&huart1,1);
			stimulateFreeRun(&huart3,2);
			stimulateFreeRun(&huart4,3);
			stimulateFreeRun(&huart5,4);
			stimulateFreeRun(&huart7,5);
			GUI_Clear();
			GUI_DispStringAt("test mode  ", 100, 270); 
			GUI_DispFloat(parameter[1][0]/10.0,4);//电流
			GUI_DispStringAt("mA  ", 290, 270); 
			GUI_DispFloat(parameter[1][1],4);//频率
			GUI_DispStringAt("us  ", 410, 270); 
			GUI_DispFloat(parameter[1][2],4);//脉宽
			GUI_DispStringAt("hz  ", 520, 270); 
			startFlag = 2;
		}
		else if(startFlag==0){
			for(int i = 0;i<7;i++){
				stim_stop(&huart1);//800102, no feedback
				stim_stop(&huart3);//800102, no feedback
				stim_stop(&huart4);//800102, no feedback
				stim_stop(&huart5);//800102, no feedback
				stim_stop(&huart7);//800102, no feedback	 
 			}	
			GUI_Clear(); 
			GUI_DispStringAt("stop",280,270);
			startFlag = 2;
		}
	}

}


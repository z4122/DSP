#include "fes.h"

uint8_t stimulate_parameter[20]="";

float value[17][2];


enum Mode name = zj;
enum Hand amputatedHand = right;

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
	if(pressure>pressureUpperThreshold[channel])
		pressure = pressureUpperThreshold[channel];
	
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
	
	
	
	//stimulate_parameter[5] = parameter[channel][1]>>8; //脉宽高8位
	//stimulate_parameter[6] = parameter[channel][1]; //脉宽低8位	
	stimulate_parameter[7] = parameter[channel][2]>>8; //频率高位
	stimulate_parameter[8] = parameter[channel][2]; //频率低位
	stimulate_parameter[11] = 0xFF; //次数高八位赋FF
	//stimulate_parameter[16] = parameter[channel][0]; //幅值
	
//	if(parameter[channel][2]==200)
//		stimulate_parameter[12] = 0x12;//次数
//	else if(parameter[channel][2]==100)
//		stimulate_parameter[12] = 0x0A;//次数
//	else if(parameter[channel][2]==50)
//		stimulate_parameter[12] = 0x05;//次数
//	else if(parameter[channel][2]==20)
//		stimulate_parameter[12] = 0x02;//次数
//	else if(parameter[channel][2]==10){
//		stimulate_parameter[8] = 100;
//		stimulate_parameter[12] = 0x01;//次数
//	}


	//flag==1||4幅值跟随模式，flag==2||5频率跟随模式，flag==3||6脉宽跟随模式,flag==7自由测试模式
	if(testmode_flag==1||testmode_flag==4){
		stimulate_parameter[16] = (threshold[channel][1]-threshold[channel][0])*(float)(pressure-pressureLowerThreshold[channel])/(pressureUpperThreshold[channel]-pressureLowerThreshold[channel])+threshold[channel][0];
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

void stim_change_amplitude(UART_HandleTypeDef *huart,double amplitude,int channel)
{
		if(amplitude<0)
			amplitude = 0;
		if(amplitude>pressureUpperThreshold[channel])
			amplitude = pressureUpperThreshold[channel];

		char parameter[5] = {0};
		parameter[0]=0x80;	
		parameter[1]=0x04;//代表着只改变电流的模式

		double temp = (threshold[channel][3]-threshold[channel][2])*(float)(amplitude-pressureLowerThreshold[channel])/(pressureUpperThreshold[channel]-pressureLowerThreshold[channel])+threshold[channel][2];
		
		u16 val = (u16)temp;
		val*=10;
		if(val<20)
			val=20;
	
		parameter[2]=val>>8;//脉宽高八位
		parameter[3]=val;//脉宽低八位
		parameter[4] = threshold[channel][0];//配合脉宽模式和诱发指感区的最低电流阈值，在脉宽模式下也会设置

		//parameter[2]=0x00;
		//parameter[3]=0xC8;
		//parameter[4] = threshold[channel][0];

		HAL_UART_Transmit(huart,(uint8_t *)parameter,sizeof(parameter),0xffff);
}

void stimulate(UART_HandleTypeDef *huart,double pressure,int channel)
{
	//stim_search(huart);// send 800103 back BC
	static int firstFlag[6] = {0};
	static int onStimFlag[6] = {0};
	
	if(pressure<0)
		pressure = 0;

	//当通道选择上并且模式不是停止模式时
	if(channelEnableflag[channel]==1&&testmode_flag!=0&&pressure>=pressureLowerThreshold[channel]){	
			if(firstFlag[channel]==0){//只有第一次进入的时候会传递所有参数
				//stim_stop(huart);//800102, no feedback
				merge_stimulate_parameter(huart,pressure,channel);
				//stim_start(huart);//800101,back AA
				firstFlag[channel] = 1;
				onStimFlag[channel] = 1;
			}
			else{//除了第一次之外，参数只传递幅值大小
				stim_change_amplitude(huart,pressure,channel);
			}
	} 
	else{//记住如果进入了停止模式，也即是testmode_flag==0，或是通道没有选择上，那么相应的firstFlag标志位就要重新刷新，代表着停止电刺激了
		firstFlag[channel]=0;
		if(onStimFlag[channel]==1){ //如果正在刺激，并且进入了停止模式，那么就会发送停止信号。
			stim_stop(huart);
			onStimFlag[channel]=0;
		}
	}
}

void stimulateDSU()
{
	if(name==zzz){
		if(amputatedHand == right){
			for(int i = 0;i<5;i++){
				double num = ldVolutage[i];
				switch(i){
					case 0: {
							float curve1=8.111*(num)*(num)*(num)-4.052*(num)*(num);//sensor13_19.8.28
							float curve2=5.218*(num)-1.338;
							float curve3=curve1+curve2;
							transferredDsuValue[i] = curve3;
							stimulate(&huart1,curve3,1);//
							break;
					}
					case 1: {
							float curve1=2.339*(num)*(num)*(num)+6.488*(num)*(num);//sensor14_19.8.28
							float curve2=1.666*(num)-0.9607;
							float curve3=curve1+curve2;	
							transferredDsuValue[i] = curve3;
							stimulate(&huart3, curve3,2);
							break;
					}
					case 2: {
							float curve1=16.67*(num)*(num)*(num)*(num)-39.43*(num)*(num)*(num);//sensor8_modified1
							float curve2=38.47*(num)*(num)-8.183*(num)+0.3524;
							float curve3=curve1+curve2;
							transferredDsuValue[i] = curve3;
							stimulate(&huart4,curve3,3);
							break;
					}
					case 3: {
							float curve1=-1.664*(num)*(num)*(num)*(num)+16.54*(num)*(num)*(num);//sensor7_modified1
							float curve2=-18.74*(num)*(num)+16.48*(num)-3.238;
							float curve3=curve1+curve2;
							transferredDsuValue[i] = curve3;
							stimulate(&huart5,curve3,4);
							break;
					}
					case 4: {
							float curve1=-2.035*(num)*(num)*(num)*(num)+14.54*(num)*(num)*(num);//sensor12_modified1
							float curve2=-13.22*(num)*(num)+11.94*(num)-2.073;
							float curve3=curve1+curve2;
							transferredDsuValue[i] = curve3;
							stimulate(&huart7, curve3,5);//ok ch6
							break;
					}
					default:
							break;
				}
			}
		}else {
			for(int i = 5;i<10;i++){
				double num = ldVolutage[i];
				switch(i){
					case 5: {
						float curve01=19.5*(num)*(num);//A201 25lbs sneor20 19.09.20
						float curve02=-6.607*(num)+1.761 ;
						float curve03=curve01+curve02;
						transferredDsuValue[i] = curve03;
						stimulate(&huart1, curve03,1);
						break;
					}
					case 6: {
						float curve1=12.97*(num)*(num)*(num);//A201 25lbs sneor21 19.9.20
						float curve2= -10.46*(num)*(num)+11.26*num -1.258;
						float curve3=curve1+curve2;
						transferredDsuValue[i] = curve3;
						stimulate(&huart3,curve3,2);
						break;
					}
						case 7: {
						float curve1=5.526*(num)*(num)*(num);//A201 25lbs sneor22 19.9.20
						float curve2= 1.15 *(num)*(num) + 6.804*(num) -0.7225;
						float curve3=curve1+curve2;
						transferredDsuValue[i] = curve3;
						stimulate(&huart4,curve3,3);	
						break;
					}
					case 8: {
						float curve1= 17.4*(num)*(num);//A201 25lbs sneor23 19.8.26
						float curve2= -4.452 *num +0.9479;
						float curve3=curve1+curve2;
						transferredDsuValue[i] = curve3;
						stimulate(&huart5,curve3,4);
						break;
					}
					case 9: {
						float curve1=27.16*(num)*(num)*(num);//A201 25lbs sneor24 19.9.20
						float curve2= -38.06*(num)*(num) + 27.1*(num)  -3.862 ;
						float curve3=curve1+curve2;
						transferredDsuValue[i] = curve1;
						stimulate(&huart7,curve1,5);
						break;
					}
					default:
						break;
				}
			}
		}
	}else if(name==zj){
		if(amputatedHand == right){
			for(int i = 0;i<5;i++){
				double num = ldVolutage[i];
				switch(i){
					case 0: {
							float curve1=-21.71*(num)*(num)*(num)*(num)+84.42*(num)*(num)*(num);//A201 25lbs sensor35
							float curve2=-44.53*(num)*(num)+42.89*(num)-5.881;
							float curve3=curve1+curve2;
							transferredDsuValue[i] = curve3;
							stimulate(&huart1,curve3,1);//
							break;
					}
					case 1: {
							float curve1=-3.045*(num)*(num)*(num)*(num)+44.08*(num)*(num)*(num);//A201 25lbs sensor25
							float curve2=-32.39*(num)*(num)+38.67*(num)-3.168;
							float curve3=curve1+curve2;
							transferredDsuValue[i] = curve3;
							stimulate(&huart3, curve3,2);
							break;
					}
					case 2: {
							float curve1=-30.89*(num)*(num)*(num)*(num)+99.53*(num)*(num)*(num);//A201 25lbs sensor31
							float curve2=-55.95*(num)*(num)+53.72*(num)-6.505;
							float curve3=curve1+curve2;	
							transferredDsuValue[i] = curve3;
							stimulate(&huart4,curve3,3);
							break;
					}
					case 3: {
							float curve1=3.909*(num)*(num)*(num)*(num)+12.39*(num)*(num)*(num);//A201 25lbs sensor34
							float curve2=17.49*(num)*(num)+21.28*(num)-2.004;
							float curve3=curve1+curve2;
							transferredDsuValue[i] = curve3;
							stimulate(&huart5,curve3,4);
							break;
					}
					case 4: {
							float curve1=83.39*(num)*(num)*(num)*(num)-141.3*(num)*(num)*(num);//A201 25lbs sensor37
							float curve2=116.2*(num)*(num)+5.029*(num)-1.712;
							float curve3=curve1+curve2;
							transferredDsuValue[i] = curve3;
							stimulate(&huart7, curve3,5);//ok ch6
							break;
					}
					default:
							break;
				}
			}
		}else{
			for(int i = 5;i<10;i++){
					double num = ldVolutage[i];
					switch(i){
						case 5: {
								double curve1=1.476*exp(1.864*num+0.01984);
								double curve2=-2.169*exp(-13.69*num-1.2321);				
								double curve3=curve1+curve2;
								transferredDsuValue[i] = curve3;
								stimulate(&huart1, curve3,1);
								break;
						}
						case 6: {
								double curve1=2.282*exp(1.986*num+0.05958);
								double curve2=-3.907*exp(-5.262*num-0.18996);					
								double curve3=curve1+curve2;
								transferredDsuValue[i] = curve3;
								stimulate(&huart3,curve3,2);
								break;
						}
								case 7: {
								double curve1=-0.026*exp(7.94*num-0.1588);
								double curve2=0.8*exp(4.538*num-0.09076);
								double curve3=curve1+curve2;
								transferredDsuValue[i] = curve3;
								stimulate(&huart4,curve3,3);	
								break;
						}
						case 8: {
								double curve3 = 14.1*num-4;
								transferredDsuValue[i] = curve3;
								stimulate(&huart5,curve3,4);
							break;
						}
						case 9: {
								double curve3 = 16.55*num-4.5;
								transferredDsuValue[i] = curve3;
								stimulate(&huart7,curve3,5);
							break;
						}
						default:
							break;
				}
			}
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

			GUI_DispStringAt("Channel 1 ", 100, 150); 
			GUI_DispFloat(parameter[1][0]/10.0,4);//电流
			GUI_DispStringAt("mA  ", 290, 150); 
			GUI_DispFloat(parameter[1][1],4);//频率
			GUI_DispStringAt("us  ", 410, 150); 
			GUI_DispFloat(parameter[1][2],4);//脉宽
			GUI_DispStringAt("hz  ", 520, 150); 

			GUI_DispStringAt("Channel 2 ", 100, 180); 
			GUI_DispFloat(parameter[2][0]/10.0,4);//电流
			GUI_DispStringAt("mA  ", 290, 180); 
			GUI_DispFloat(parameter[2][1],4);//频率
			GUI_DispStringAt("us  ", 410, 180); 
			GUI_DispFloat(parameter[2][2],4);//脉宽
			GUI_DispStringAt("hz  ", 520, 180); 

			GUI_DispStringAt("Channel 3 ", 100, 210); 
			GUI_DispFloat(parameter[3][0]/10.0,4);//电流
			GUI_DispStringAt("mA  ", 290, 210); 
			GUI_DispFloat(parameter[3][1],4);//频率
			GUI_DispStringAt("us  ", 410, 210); 
			GUI_DispFloat(parameter[3][2],4);//脉宽
			GUI_DispStringAt("hz  ", 520, 210); 

			GUI_DispStringAt("Channel 4 ", 100, 240); 
			GUI_DispFloat(parameter[4][0]/10.0,4);//电流
			GUI_DispStringAt("mA  ", 290, 240); 
			GUI_DispFloat(parameter[4][1],4);//频率
			GUI_DispStringAt("us  ", 410, 240); 
			GUI_DispFloat(parameter[4][2],4);//脉宽
			GUI_DispStringAt("hz  ", 520, 240); 

			GUI_DispStringAt("Channel 5 ", 100, 270); 
			GUI_DispFloat(parameter[5][0]/10.0,4);//电流
			GUI_DispStringAt("mA  ", 290, 270); 
			GUI_DispFloat(parameter[5][1],4);//频率
			GUI_DispStringAt("us  ", 410, 270); 
			GUI_DispFloat(parameter[5][2],4);//脉宽
			GUI_DispStringAt("hz  ", 520, 270); 

			startFlag = 2;
		}
		else if(startFlag==0){
			stim_stop(&huart1);//800102, no feedback
			stim_stop(&huart3);//800102, no feedback
			stim_stop(&huart4);//800102, no feedback
			stim_stop(&huart5);//800102, no feedback
			stim_stop(&huart7);//800102, no feedback	 
 			
			GUI_Clear(); 
			GUI_DispStringAt("stop",280,270);
			startFlag = 2;
		}
	}

}

void PCFollowStimulateDSU()
{
		for(int i = 0;i<5;i++){
			double num = ldVolutage[i];
			switch(i){
			case 0: {
				transferredDsuValue[i] = num;
				stimulate(&huart1,num,1);//
				break;
			}
			case 1: {
				transferredDsuValue[i] = num;
				stimulate(&huart3, num,2);
				break;
			}
			case 2: {
				transferredDsuValue[i] = num;
				stimulate(&huart4,num,3);
				break;
			}
			case 3: {
				transferredDsuValue[i] = num;
				stimulate(&huart5,num,4);
				break;
			}
			case 4: {	
				transferredDsuValue[i] = num;
				stimulate(&huart7, num,5);//ok ch6
				break;
			}
			default:
				break;
			}
		}	
}

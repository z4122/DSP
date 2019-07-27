#include "ad.h"

long double offset[10];
double ADC_convertedvalue[2];
long double ldVolutage[10];//存储十个通道的值

 void ADS1256_Write_Byte(unsigned char d)
{
	unsigned char i=8;	
	ClrADS1256_CLK ;
   	while(i--)
  	{
		if(d & 0X80)
			SetADS1256_IN  ;
		else  
			ClrADS1256_IN  ;
		delayad_nopar(); 	  	  
		SetADS1256_CLK;
		delayad_nopar();
		ClrADS1256_CLK ;
		delayad_nopar();
		d <<= 1;
	}
}

unsigned char ADS1256_Read_Byte(void)
{  
	unsigned char i=8,d;   
	ClrADS1256_CLK;	 
//	delayad(2);	   //min=200ns
   	while(i--)
    {
      	d <<=1;	
	  	SetADS1256_CLK;  
      	delayad_nopar(); 
      	ClrADS1256_CLK;   
		delayad_nopar();
		
	  	if(ADS1256_DO)
 	    	d |= 0x01;
	  	else 
 	    	d &= 0xfe;     	   	  	  
	}    
   	return d;

}

unsigned char  ADS1256_Read_one_Reg(unsigned char reg_name)
{
	unsigned char dat;
	ClrADS1256_CS;
	while(ADS1256_DRDY);
	ADS1256_Write_Byte(CMD_RREG|reg_name);
   	ADS1256_Write_Byte(0x00);
	delayad(2);   //min=50*(1/fCLKIN)=50*(1/7.68MHZ)=6500ns;max=whatever
//	while(ADS1256_DRDY);
	dat=ADS1256_Read_Byte();
	SetADS1256_CS;
	return dat;
}
 

void  ADS1256_Write_Reg(unsigned char reg_name, unsigned char reg_data)
{
	  ClrADS1256_CS;
  	while(ADS1256_DRDY);
	ADS1256_Write_Byte(CMD_WREG|reg_name);
   	ADS1256_Write_Byte(0x00);
   	ADS1256_Write_Byte(reg_data);
		SetADS1256_CS;
}


//void Readad(void)
//{
////	SetADS1256_CS;
//	ClrADS1256_CS;
//	while(ADS1256_DRDY==0);
//	while(ADS1256_DRDY);	 //DRDY??????AD????
////	SetADS1256_CS;
////	ClrADS1256_CS;
//		ADS1256_Write_Byte(CMD_SYNC);
//		ADS1256_Write_Byte(0xff);
//   	ADS1256_Write_Byte(CMD_RDATA);
//	delayad(1000);   //min=50*(1/fCLKIN)=50*(1/7.68MHZ)=6500ns;max=whatever
//	  ADS1256_Write_Byte(0xff);
//   	result1 = ADS1256_Read_Byte();
//	ADS1256_Write_Byte(0xff);
//   	result2 = ADS1256_Read_Byte();
//	ADS1256_Write_Byte(0xff);
//   	result3 = ADS1256_Read_Byte();
//   	SetADS1256_CS;
//	result=result1*65536+result2*256+result3;
//}


void ADS1256_Init(void)
{
	SetADS1256_SYNC;
	ClrADS1256_CLK;
//	SetADS1256_CS;
	delayad(2);
	ClrADS1256_CS; 
	while(!ADS1256_DRDY);
	while(ADS1256_DRDY); 		
	ADS1256_Write_Byte(CMD_RESET);
//	while(ADS1256_DRDY);
//	ADS1256_Write_Byte(ADS1256_SYNC);
//	while(ADS1256_DRDY);
//	ADS1256_Write_Byte(CMD_WAKEUP);
	while(ADS1256_DRDY);
	ADS1256_Write_Reg(REG_STATUS,0x06);	//STATUS REGISTER:Auto-Calibration Enabled,Analog Input Buffer Disabled
	//ADS1256_Write_Reg(REG_STATUS,MSB_FRIST+ACAL_ON+BUFEN_OFF);	//STATUS:MSB;AOTO ColCalibration;BUFFER DISABLEED
	while(ADS1256_DRDY);
//	ADS1256_Write_Reg(REG_MUX,POSITIVE_AIN0+ NEGTIVE_AINCOM);  //MUX:AIN0
    ADS1256_Write_Reg(REG_MUX,0x08);	//AIN0 is Positive,single-ended measurements
	while(ADS1256_DRDY);
	ADS1256_Write_Reg(REG_ADCON,0x00);	   //ADCON=00h
//	ADS1256_Write_Reg(REG_DRATE,DATARATE_30K);  //data rate 30k SPS
	ADS1256_Write_Reg(REG_DRATE,DATARATE_30K);  //data rate 30k SPS
	while(ADS1256_DRDY);
	ADS1256_Write_Reg(REG_IO,0x00);    //GPIO output  
	while(ADS1256_DRDY);
    ADS1256_Write_Byte(CMD_SELFCAL);
//	while(ADS1256_DRDY);
//	ADS1256_Write_Byte(ADS1256_SYNC);
//	ADS1256_Write_Byte(CMD_WAKEUP);
	delayad(10);
//	SetADS1256_CS;
}


unsigned int ADS_sum(unsigned char channel)
{
	ADS1256_Write_Reg(REG_MUX,channel);		//????
	return ADS1256ReadData();//??AD?,??24????
}

unsigned int ADS1256ReadData()  
{
  unsigned int sum=0;
	
	ClrADS1256_CS;
	while(ADS1256_DRDY);              //?ADS1256_DRDY????????? 
//	ADS1256WREG(ADS1256_MUX,channel);		//????
	ADS1256_Write_Byte(CMD_SYNC);
	ADS1256_Write_Byte(0xff);	               
	ADS1256_Write_Byte(CMD_RDATA);
	
  sum |= (ADS1256_Read_Byte() << 16);
	sum |= (ADS1256_Read_Byte() << 8);
	sum |= ADS1256_Read_Byte();

	SetADS1256_CS;
   return sum;
}

void AD_Init(void)
{
	AD_GPIO_Init();
	HAL_Delay(100);
	HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);//Reset???1
	ADS1256_Init();
	//Compute_Offset();
}



void AD_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOSI_GPIO_Port, MOSI_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);
	
	GPIO_InitStruct.Pin = SCK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SCK_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = NSS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(NSS_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = MOSI_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(MOSI_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : MISO_Pin */
	GPIO_InitStruct.Pin = MISO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(MISO_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : RESET_Pin */
	GPIO_InitStruct.Pin = RESET_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RESET_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : DRDY_Pin */
	GPIO_InitStruct.Pin = DRDY_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(DRDY_GPIO_Port, &GPIO_InitStruct);
	
	
	GPIO_InitStruct.Pin = TEST_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TEST_GPIO_Port, &GPIO_InitStruct);
}


void delayad_nopar(void)
{
	unsigned long i = 2000;//2000
   	while(i--);
}
void delayad(unsigned tt)
{
	unsigned char i,j;
	for(j=tt;j>0;j--)
		for(i=200;i>0;i--);//200
}

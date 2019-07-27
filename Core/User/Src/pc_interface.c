#include "pc_interface.h"

uint8_t data[15];//å­˜å‚¨çš„é€šé“çš„å€¼

/*
dataÊı¾İ¸ñÊ½
0x00 0x00  0xXX 0xXX  0xXX 0xXX  0xXX 0xXX  0xXX 0xXX  0xXX 0xXX  0xXX 0xXX    0xXX 
| Êı¾İÍ· |  Í¨µÀÒ»  |  Í¨µÀ¶ş   |  Í¨µÀÈı  |  Í¨µÀËÄ  |  Í¨µÀÎå  |±£ÁôÁ½¸ö×Ö½Ú|Ğ£ÑéºÍ|

*/

void TransferData2PC(){
	//å‰äº”ä¸ªé€šé“çš„å¤´
	data[0] = 0xff;
	data[1] = 0xff;
	data[14] = 0x00;
	for(int i = 0;i < 5;i++)
	{
		data[i*2+2] = ((u16)(ldVolutage[i]*1000))>>8;
		data[14] += data[i*2+2];
		data[i*2+3] = ((u16)(ldVolutage[i]*1000))&0xFF;
		data[14] += data[i*2+3];
	}
	

	HAL_UART_Transmit(&huart8,data,15,0xfff);

	//åäº”ä¸ªé€šé“çš„å¤´
	data[0] = 0xff;
	data[1] = 0xf1;
	data[14] = 0x00;

	for(int i = 5;i < 8;i++)
	{
		data[i*2-8] = ((u16)(ldVolutage[i]*1000))>>8;
		data[14] += data[i*2-8];
		data[i*2-7] = ((u16)(ldVolutage[i]*1000))&0xFF;
		data[14] += data[i*2-7];
	}

	data[8] = ((u16)(ldVolutage[8]*1000))>>8;
	data[9] = ((u16)(ldVolutage[8]*1000))&0xFF;
	data[10] =((u16)(ldVolutage[9]*1000))>>8;
	data[11] =((u16)(ldVolutage[9]*1000))&0xFF;

	data[14] = data[14]+data[8]+data[9]+data[10]+data[11];

	HAL_UART_Transmit(&huart8,data,15,0xfff);
	HAL_UART_Receive_IT(&huart8,&UART8RxBuff,1);

}

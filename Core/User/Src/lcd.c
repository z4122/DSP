#include "lcd.h"

void LCDFontInit() {
  GUI_SetLayerVisEx (1, 0);
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font32_1);
}


void DisplayRunning(){
	static unsigned int i = 0;
	switch(i%8){
		case 0:
			GUI_DispStringAt("Running-------- >", 50, 50);
			break;
		case 1:
			GUI_DispStringAt("Running------- ->", 50, 50);
			break;
		case 2:
			GUI_DispStringAt("Running------ -->", 50, 50);
			break;
		case 3:
			GUI_DispStringAt("Running----- --->", 50, 50);
			break;
		case 4:
			GUI_DispStringAt("Running---- ---->", 50, 50);
			break;
		case 5:
			GUI_DispStringAt("Running-- ------>", 50, 50);
			break;
		case 6:
			GUI_DispStringAt("Running- ------->", 50, 50);
			break;
		case 7:
			GUI_DispStringAt("Running -------->", 50, 50);
			break;
	}
	i++;
	if(initMode==1)
		GUI_DispStringAt("Follow    mode", 50, 90);
	else if(initMode==2)
		GUI_DispStringAt("Freerun   mode", 50, 90);
}

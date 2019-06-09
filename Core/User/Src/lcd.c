#include "lcd.h"


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
	
}

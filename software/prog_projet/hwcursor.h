/******hwcursor.h *********************************************************
*Description
* Library to control the hardware cursor accelerator 
*********************************************************************/
#ifndef HW_CURSOR_H_
#define HW_CURSOR_H_

#define RF_FB_BASE_ADDR			(1*sizeof(int))
#define RF_X_MAX_ADDR 			(2*sizeof(int))
#define RF_Y_MAX_ADDR 			(3*sizeof(int))
#define RF_X_COORDINATE_ADDR	(4*sizeof(int))
#define RF_Y_COORDINATE_ADDR	(5*sizeof(int))
#define RF_COMMAND_ADDR			(6*sizeof(int))
#define RF_START_ADDR 			(7*sizeof(int))

#define CMD_DRAW	0
#define CMD_ERASE	1
#define CDM_SAVE	2

void hwcursor_init(int x_size, int y_size);

unsigned int hwcursor_draw_cursor(int x, int y);

unsigned int hwcursor_save_cursor(int x, int y);

unsigned int hwcursor_erase_cursor(int x, int y);

#endif  // HW_CURSOR_H_

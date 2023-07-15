/******recfiller.h *********************************************************
*Description
* Librairie pour controler l'accelerateur de rectangles
*********************************************************************/
#ifndef RECFILLER_H_
#define RECFILLER_H_

#define RF_FB_BASE_ADDR     (1*sizeof(int))
#define RF_X_MAX_ADDR 		(2*sizeof(int))
#define RF_Y_MAX_ADDR 		(3*sizeof(int))
#define RF_X_LEFT_ADDR 		(4*sizeof(int))
#define RF_Y_TOP_ADDR 		(5*sizeof(int))
#define RF_X_RIGHT_ADDR 	(6*sizeof(int))
#define RF_Y_BOTTOM_ADDR 	(7*sizeof(int))
#define RF_COLOR_ADDR 		(8*sizeof(int))
#define RF_START_ADDR 		(9*sizeof(int))

void recfiller_init(int x_size, int y_size);

unsigned int recfiller_draw_rectangle(int x_left, int y_top, 
                              int x_right, int y_bottom, 
                              int color);

#endif  // RECFILLER_H_

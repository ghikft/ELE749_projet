/******recfiller.h *********************************************************
*Description
* Librairie pour controler l'accelerateur de rectangles
*********************************************************************/
#ifndef RECFILLER_H_
#define RECFILLER_H_

#define RF_FB_BASE_ADDR     (1*sizeof(int))
#define RF_X_MAX_ADDR 		(2*sizeof(int))
#define RF_Y_MAX_ADDR 		(3*sizeof(int))
#define RF_X_CENTER_ADDR 	(4*sizeof(int))
#define RF_Y_CENTER_ADDR 	(5*sizeof(int))
#define RF_X_RADIUS_ADDR 	(6*sizeof(int))
#define RF_Y_RADIUS_ADDR 	(7*sizeof(int))
#define RF_COLOR_ADDR 		(8*sizeof(int))
#define RF_START_ADDR 		(9*sizeof(int))

void hwellipse_init(int x_size, int y_size);

unsigned int hwellipse_draw_ellipse(int x_center, int y_center, 
                                    int x_radius, int y_radius, 
                                    int color);

#endif  // RECFILLER_H_

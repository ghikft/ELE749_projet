/* Description
* Library to draw with software
*********************************************************************/
#ifndef SOFT_DRAW_H_
#define SOFT_DRAW_H_

#include "alt_types.h"
#include "system.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"

typedef enum drawingOpp_En {
    EMPTY_RECT,
    EMPTY_ELLIPSE
}drawingOpp;
typedef struct pixelValue_S{
    int x;
    int y;
    alt_u8 color;
}pixelValue;
//drawingOpp lastDrawingOpp;

typedef struct lastDrawingVar_S {
    drawingOpp lastDrawingOpp;
    alt_u8 lastDrawnPixelMemory[2300];
    pixelValue lastEllipse[2400];
    int numberOfPixelForLastDraw;
    int lastFirstPointX;
    int lastFirstPointY;
    int lastSecondPointX;
    int lastSecondPointY;
    int firstErase;
}lastDrawingVar;

lastDrawingVar lastDrawingData;

void soft_recfiller_init(int x_size, int y_size);

unsigned char get_pixel_color2(int x, int y);


void soft_emptyRect_draw(int x_left, int y_top,
    int x_right, int y_bottom,
    int color, int erasePreviousWork, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer);
/*unsigned int soft_recfiller_draw_rectangle(int x_left, int y_top,
    int x_right, int y_bottom,
    int color);*/

void draw_empty_ellipse(int x_center, int y_center, int x_radius, int y_radius, 
						int color, alt_up_pixel_buffer_dma_dev* pixel_buffer, 
						int erasePreviousWork, lastDrawingVar* lastDrawingData);

#endif  // SOFT_DRAW_H_

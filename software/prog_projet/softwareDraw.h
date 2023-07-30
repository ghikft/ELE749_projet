/* Description
* Library to draw with software
*********************************************************************/
#ifndef SOFT_DRAW_H_
#define SOFT_DRAW_H_

#include "alt_types.h"
#include "system.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"

/*typedef enum drawingOpp_En {
    EMPTY_RECT,
    EMPTY_ELLIPSE
}drawingOpp;*/
typedef struct pixelValue_S{
    int x;
    int y;
    alt_u8 color;
}pixelValue;
//drawingOpp lastDrawingOpp;

typedef struct lastDrawingVar_S {
    //drawingOpp lastDrawingOpp;
    alt_u8 lastDrawnPixelMemory[2300];
    pixelValue lastPixelMemory[2400];
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

int draw_empty_ellipse(int x_center, int y_center, int x_radius, int y_radius, 
						int color, alt_up_pixel_buffer_dma_dev* pixel_buffer, 
						int erasePreviousWork, lastDrawingVar* lastDrawingData);


void flood_fill_zone(int startX, int startY, int fillColor, alt_up_pixel_buffer_dma_dev* pixel_buffer);
 
char flood_fill_sub(int x, int y, int fillColor, int initialColor, alt_up_pixel_buffer_dma_dev* pixel_buffer);

void fill_to_edge_zone(int startX, int startY, int fillColor, alt_up_pixel_buffer_dma_dev* pixel_buffer);

char fill_to_edge_sub(int x, int y, int fillColor, alt_up_pixel_buffer_dma_dev* pixel_buffer);

void soft_draw_line(int x1, int y1,
    				int x2, int y2,
    				int color, int erasePreviousWork, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer);

void soft_copy_paste(int x1_copy, int y1_copy, int x2_copy, int y2_copy, int x1_paste, int y1_paste, int cut, 
					lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer);
#endif  // SOFT_DRAW_H_

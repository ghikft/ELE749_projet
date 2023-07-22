#include "softwareDraw.h"
#include "alt_types.h"
#include "system.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include <stdio.h>

#define TRUE	1
#define FALSE	0

//extern alt_up_pixel_buffer_dma_dev *pixel_buffer;

//alt_u8 lastDrawnOp = lastDrawingOpp;

//void erase_last_drawn_shape()
void soft_emptyRect_draw(int x_left, int y_top,
	int x_right, int y_bottom,
	int color, int erasePreviousWork, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer)
{
	/**************************************************************************
	 * Get the color value of a specific pixel
	 **************************************************************************
	 * Parameters
	 * x : x coordinate of the pixel
	 * y : y coordinate of the pixel
	 *
	 * Valeur de retour
	 * specified pixel color
	 *
	 * Side effects
	 * none
	 *************************************************************************/
	
	int tempReorder;
	int x;
	int y;
	int iter=0;
	
	//if in process of dragging the rectangle to desired size, supress the old rectangle first
	if (erasePreviousWork == TRUE) {
		x = lastDrawingData->lastFirstPointX;
		y = lastDrawingData->lastFirstPointY;
		
		while (x < lastDrawingData->lastSecondPointX) {
			alt_up_pixel_buffer_dma_draw(pixel_buffer, lastDrawingData->lastDrawnPixelMemory[iter], x, y);
			x++;
		}
		while (y < lastDrawingData->lastSecondPointY) {
			alt_up_pixel_buffer_dma_draw(pixel_buffer, lastDrawingData->lastDrawnPixelMemory[iter], x, y);
			y++;
		}
		while (x > x_left) {
			alt_up_pixel_buffer_dma_draw(pixel_buffer, lastDrawingData->lastDrawnPixelMemory[iter], x, y);
			x--;
		}
		while (y > y_top) {
			alt_up_pixel_buffer_dma_draw(pixel_buffer, lastDrawingData->lastDrawnPixelMemory[iter], x, y);
			y--;
		}
	}
	//reorder point so the x_left coordinate are always smaller than x_right and y_top smaller than y_bottom
	if (x_left > x_right) {
		tempReorder = x_left;
		x_left = x_right;
		x_right = tempReorder;
		lastDrawingData->lastFirstPointX = x_left;
		lastDrawingData->lastSecondPointX = x_right;
	}
	if (y_top > y_bottom) {
		tempReorder = y_top;
		y_top = y_bottom;
		y_bottom = tempReorder;
		lastDrawingData->lastFirstPointY = y_top;
		lastDrawingData->lastSecondPointY = y_bottom;
	}
	//Draw the new rectangle
	if (erasePreviousWork == FALSE) {
		//printf("draw rect\n\r");
		x = x_left;
		y = y_top;
		//Start by saving the pixel where the rectangle will be drawn clock wise from left-top corner
		//then draw right after saving the pixel.
		lastDrawingData->numberOfPixelForLastDraw = 0;
		
		while (x < x_right) {
			lastDrawingData->lastDrawnPixelMemory[lastDrawingData->numberOfPixelForLastDraw] = get_pixel_color2(x, y);
			alt_up_pixel_buffer_dma_draw(pixel_buffer, color, x, y);
			//printf("loop1 %d\n\r", x);
			x++;
			lastDrawingData->numberOfPixelForLastDraw++;
		}
		//printf("loop1 end x %d\n\r",x);
		while (y < y_bottom) {
			lastDrawingData->lastDrawnPixelMemory[lastDrawingData->numberOfPixelForLastDraw] = get_pixel_color2(x, y);
			alt_up_pixel_buffer_dma_draw(pixel_buffer, color, x, y);
			//printf("loop2 %d\n\r", y);
			y++;
			lastDrawingData->numberOfPixelForLastDraw++;
		}
		//printf("loop2 end y %d\n\r", y);
		while (x > x_left) {
			lastDrawingData->lastDrawnPixelMemory[lastDrawingData->numberOfPixelForLastDraw] = get_pixel_color2(x, y);
			alt_up_pixel_buffer_dma_draw(pixel_buffer, color, x, y);
			//printf("loop3 %d\n\r", x);
			x--;
			lastDrawingData->numberOfPixelForLastDraw++;
		}
		//printf("loop3 end x %d\n\r", x);
		while (y > y_top) {
			lastDrawingData->lastDrawnPixelMemory[lastDrawingData->numberOfPixelForLastDraw] = get_pixel_color2(x, y);
			alt_up_pixel_buffer_dma_draw(pixel_buffer, color, x, y);
			//printf("loop4 %d\n\r", y);
			y--;
			lastDrawingData->numberOfPixelForLastDraw++;
		}
		//printf("loop4 end y %d\n\r", y);
	}
	
	//printf("nb pixel %d\n\r", lastDrawingData->numberOfPixelForLastDraw++);
}

unsigned char get_pixel_color2(int x, int y) {
	/**************************************************************************
	 * Get the color value of a specific pixel
	 **************************************************************************
	 * Parameters
	 * x : x coordinate of the pixel
	 * y : y coordinate of the pixel
	 *
	 * Valeur de retour
	 * specified pixel color
	 *
	 * Side effects
	 * none
	 *************************************************************************/
	unsigned char* pixel;
	unsigned char color;
	//pixel = (unsigned char *)ONCHIP_MEM_BASE+(y<<9)+x;
	pixel = (unsigned char*)ONCHIP_MEM_BASE + (y * 640) + x;
	color = *pixel;
	return color;
}
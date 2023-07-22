#include "softwareDraw.h"
#include "alt_types.h"
#include "system.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"

#define TRUE	1
#define FALSE	0

//extern alt_up_pixel_buffer_dma_dev *pixel_buffer;

//alt_u8 lastDrawnOp = lastDrawingOpp;

//void erase_last_drawn_shape()
void soft_emptyRect_draw(int x_left, int y_top,
    int x_right, int y_bottom,
    int color,int erasePreviousWork, lastDrawingVar *lastDrawingData, alt_up_pixel_buffer_dma_dev *pixel_buffer)
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
    //if drawing a new rectangle
	if (erasePreviousWork == FALSE) {
		int x = x_left;
		int y = y_top;
		//Start by saving the pixel where the rectangle will be drawn clock wise from left-top corner
		//then draw right after saving the pixel.
		lastDrawingData->numberOfPixelForLastDraw = 0;
		while (x <= x_right) {
			lastDrawingData->lastDrawnPixelMemory[lastDrawingData->numberOfPixelForLastDraw] = get_pixel_color2(x, y);
			alt_up_pixel_buffer_dma_draw(pixel_buffer, color, x, y);
			x++;
			lastDrawingData->numberOfPixelForLastDraw++;
		}
		while (y <= y_bottom) {
			lastDrawingData->lastDrawnPixelMemory[lastDrawingData->numberOfPixelForLastDraw] = get_pixel_color2(x, y);
			alt_up_pixel_buffer_dma_draw(pixel_buffer, color, x, y);
				y--;
			lastDrawingData->numberOfPixelForLastDraw++;
		}
		while (x >= x_left) {
			lastDrawingData->lastDrawnPixelMemory[lastDrawingData->numberOfPixelForLastDraw] = get_pixel_color2(x, y);
			alt_up_pixel_buffer_dma_draw(pixel_buffer, color, x, y);
				x--;
			lastDrawingData->numberOfPixelForLastDraw++;
		}
		while (y <= y_bottom) {
			lastDrawingData->lastDrawnPixelMemory[lastDrawingData->numberOfPixelForLastDraw] = get_pixel_color2(x, y);
			alt_up_pixel_buffer_dma_draw(pixel_buffer, color, x, y);
				y++;
			lastDrawingData->numberOfPixelForLastDraw++;
		}
	}
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
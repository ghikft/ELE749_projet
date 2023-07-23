#include "softwareDraw.h"
#include "alt_types.h"
#include "system.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include <stdio.h>

#define TRUE	1
#define FALSE	0

void plot4points(int cx, int cy, int x, int y, alt_up_pixel_buffer_dma_dev* pixel_buffer, int color);

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
	
	
	//if in process of dragging the rectangle to desired size, supress the old rectangle first
	if (erasePreviousWork == TRUE) {
		if (lastDrawingData->firstErase == TRUE) {
			lastDrawingData->firstErase = FALSE;
			lastDrawingData->lastFirstPointX = x_left;
			lastDrawingData->lastSecondPointX = x_left;
			lastDrawingData->lastFirstPointY = y_top;
			lastDrawingData->lastSecondPointY = y_top;
		}
		x = lastDrawingData->lastFirstPointX;
		y = lastDrawingData->lastFirstPointY;

		int iter = 0;
		while (x < lastDrawingData->lastSecondPointX) {
			alt_up_pixel_buffer_dma_draw(pixel_buffer, lastDrawingData->lastDrawnPixelMemory[iter], x, y);
			x++;
			iter++;
		}
		while (y < lastDrawingData->lastSecondPointY) {
			alt_up_pixel_buffer_dma_draw(pixel_buffer, lastDrawingData->lastDrawnPixelMemory[iter], x, y);
			y++;
			iter++;
		}			
		while (x > lastDrawingData->lastFirstPointX) {
			alt_up_pixel_buffer_dma_draw(pixel_buffer, lastDrawingData->lastDrawnPixelMemory[iter], x, y);
			x--;
			iter++;
		}			
		while (y > lastDrawingData->lastFirstPointY) {
			alt_up_pixel_buffer_dma_draw(pixel_buffer, lastDrawingData->lastDrawnPixelMemory[iter], x, y);
			y--;
			iter++;
		}
	}
	
	//reorder point so the x_left coordinate are always smaller than x_right and y_top smaller than y_bottom
	if((x_left!=x_right)&&(y_top!=y_bottom)){
		if (x_left > x_right) {
			tempReorder = x_left;
			x_left = x_right;
			x_right = tempReorder;
		}	
		lastDrawingData->lastFirstPointX = x_left;
		lastDrawingData->lastSecondPointX = x_right;
	
		if (y_top > y_bottom) {
			tempReorder = y_top;
			y_top = y_bottom;
			y_bottom = tempReorder;
		}
		lastDrawingData->lastFirstPointY = y_top;
		lastDrawingData->lastSecondPointY = y_bottom;
	
	//Draw the new rectangle
	//if (erasePreviousWork == FALSE) {
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

void init_lastDrawingVar(lastDrawingVar* lastDrawingData) {
	lastDrawingData->firstErase = TRUE;
	lastDrawingData->lastFirstPointX = 0;
	lastDrawingData->lastFirstPointY = 0;
	lastDrawingData->lastSecondPointX = 0;
	lastDrawingData->lastSecondPointY = 0;
}

void draw_empty_ellipse(int x_center, int y_center, int x_radius, int y_radius, 
						int color, alt_up_pixel_buffer_dma_dev* pixel_buffer){
	
	int x,y;
	int xChange, yChange;
	int ellipseError;
	int twoASquare, twoBSquare;
	int stoppingX, stoppingY;
	
	//setup of work variables for first loop
	twoASquare = 2*x_radius*x_radius;
	twoBSquare = 2*y_radius*y_radius;
	x = x_radius;
	y = 0;
	xChange = y_radius*y_radius*(1-2*x_radius);
	yChange = x_radius*x_radius;
	ellipseError = 0;
	stoppingX = twoBSquare*x_radius;
	stoppingY = 0;

	//1st point set of the quadrant of the ellipse
	while(stoppingX >= stoppingY){
		printf("L1 x: %d, y: %d",x,y);
		plot4points(x_center,y_center,x,y,pixel_buffer,color);
		y++;
		stoppingY = stoppingY+twoASquare;
		ellipseError = ellipseError+yChange;
		yChange = yChange+twoASquare;
		if((2*ellipseError+xChange)>0){
			x--;
			stoppingX = stoppingX-twoBSquare;
			ellipseError = ellipseError+xChange;
			xChange = xChange+twoBSquare;
		}
	}

	//setup of work variable for second loop
	x=0;
	y=y_radius;
	xChange = y_radius*y_radius;
	yChange = x_radius*x_radius*(1-2*y_radius);
	ellipseError = 0;
	stoppingX=0;
	stoppingY = twoASquare*y_radius;

	//2nd point set of the quadrant of the ellipse
	while (stoppingX <= stoppingY) {
		printf("L2 x: %d, y: %d",x,y);
		plot4points(x_center,y_center,x,y,pixel_buffer,color);
		x++;
		stoppingX=stoppingX+twoBSquare;
		ellipseError=ellipseError+xChange;
		xChange =xChange+twoBSquare;
		if((2*ellipseError+yChange)>0){
			y--;
			stoppingY=stoppingY-twoASquare;
			ellipseError=ellipseError+yChange;
			yChange= yChange+twoASquare;
		}
	}
}


void plot4points(int cx, int cy, int x, int y, alt_up_pixel_buffer_dma_dev* pixel_buffer, int color){
	alt_up_pixel_buffer_dma_draw(pixel_buffer,color, cx+x,cy+y); //Q1
	alt_up_pixel_buffer_dma_draw(pixel_buffer,color, cx-x,cy+y); //Q2
	alt_up_pixel_buffer_dma_draw(pixel_buffer,color, cx-x,cy-y); //Q3
	alt_up_pixel_buffer_dma_draw(pixel_buffer,color, cx+x,cy-y); //Q4 
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
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
						int color, alt_up_pixel_buffer_dma_dev* pixel_buffer, 
						int erasePreviousWork, lastDrawingVar* lastDrawingData){
	//Erase previous ellipse here TODO
	if(erasePreviousWork){
		if(lastDrawingData->lastSecondPointX !=0){
			if(lastDrawingData->lastSecondPointY !=0){
				//variables for drawing ellipse
				int x_e,y_e;
				int xChange_e, yChange_e;
				int ellipseError_e;
				int twoASquare_e, twoBSquare_e;
				int stoppingX_e, stoppingY_e;

				//setup of work variables for first loop
				twoASquare_e = 2*lastDrawingData->lastSecondPointX*lastDrawingData->lastSecondPointX;
				twoBSquare_e = 2*lastDrawingData->lastSecondPointY*lastDrawingData->lastSecondPointY;
				x_e = lastDrawingData->lastSecondPointX;
				y_e = 0;
				xChange_e = lastDrawingData->lastSecondPointY*lastDrawingData->lastSecondPointY*(1-2*lastDrawingData->lastSecondPointX);
				yChange_e = lastDrawingData->lastSecondPointX*lastDrawingData->lastSecondPointX;
				ellipseError_e = 0;
				stoppingX_e = twoBSquare_e*lastDrawingData->lastSecondPointX;
				stoppingY_e = 0;

				//1st point set of the quadrant of the ellipse
				int iter =0;
				while(stoppingX_e >= stoppingY_e){
					//printf("Erase L1 StopX: %d, StopY: %d\n\r", stoppingX_e, stoppingY_e);
					alt_up_pixel_buffer_dma_draw(pixel_buffer,lastDrawingData->lastDrawnPixelMemory[iter], lastDrawingData->lastFirstPointX+x_e,lastDrawingData->lastFirstPointY+y_e); //Q1
					iter++;
					alt_up_pixel_buffer_dma_draw(pixel_buffer,lastDrawingData->lastDrawnPixelMemory[iter], lastDrawingData->lastFirstPointX-x_e,lastDrawingData->lastFirstPointY+y_e); //Q2
					iter++;
					alt_up_pixel_buffer_dma_draw(pixel_buffer,lastDrawingData->lastDrawnPixelMemory[iter], lastDrawingData->lastFirstPointX-x_e,lastDrawingData->lastFirstPointY-y_e); //Q3
					iter++;
					alt_up_pixel_buffer_dma_draw(pixel_buffer,lastDrawingData->lastDrawnPixelMemory[iter], lastDrawingData->lastFirstPointX+x_e,lastDrawingData->lastFirstPointY-y_e); //Q4
					iter++;
					y_e++;
					stoppingY_e = stoppingY_e+twoASquare_e;
					ellipseError_e = ellipseError_e+yChange_e;
					yChange_e = yChange_e+twoASquare_e;
					if((2*ellipseError_e+xChange_e)>0){
						x_e--;
						stoppingX_e = stoppingX_e-twoBSquare_e;
						ellipseError_e = ellipseError_e+xChange_e;
						xChange_e = xChange_e+twoBSquare_e;
					}
				}

				//setup of work variable for second loop
				x_e=0;
				y_e=lastDrawingData->lastSecondPointY;
				xChange_e = lastDrawingData->lastSecondPointY*lastDrawingData->lastSecondPointY;
				yChange_e = lastDrawingData->lastSecondPointX*lastDrawingData->lastSecondPointX*(1-2*lastDrawingData->lastSecondPointY);
				ellipseError_e = 0;
				stoppingX_e=0;
				stoppingY_e = twoASquare_e*lastDrawingData->lastSecondPointY;

				//2nd point set of the quadrant of the ellipse
				while (stoppingX_e <= stoppingY_e) {
					//printf("Erase L2 StopX: %d, StopY: %d\n\r", stoppingX_e, stoppingY_e);
					alt_up_pixel_buffer_dma_draw(pixel_buffer,lastDrawingData->lastDrawnPixelMemory[iter], lastDrawingData->lastFirstPointX+x_e,lastDrawingData->lastFirstPointY+y_e); //Q1
					iter++;
					alt_up_pixel_buffer_dma_draw(pixel_buffer,lastDrawingData->lastDrawnPixelMemory[iter], lastDrawingData->lastFirstPointX-x_e,lastDrawingData->lastFirstPointY+y_e); //Q2
					iter++;
					alt_up_pixel_buffer_dma_draw(pixel_buffer,lastDrawingData->lastDrawnPixelMemory[iter], lastDrawingData->lastFirstPointX-x_e,lastDrawingData->lastFirstPointY-y_e); //Q3
					iter++;
					alt_up_pixel_buffer_dma_draw(pixel_buffer,lastDrawingData->lastDrawnPixelMemory[iter], lastDrawingData->lastFirstPointX+x_e,lastDrawingData->lastFirstPointY-y_e); //Q4
					iter++;
					x_e++;
					stoppingX_e=stoppingX_e+twoBSquare_e;
					ellipseError_e=ellipseError_e+xChange_e;
					xChange_e =xChange_e+twoBSquare_e;
					if((2*ellipseError_e+yChange_e)>0){
						y_e--;
						stoppingY_e=stoppingY_e-twoASquare_e;
						ellipseError_e=ellipseError_e+yChange_e;
						yChange_e= yChange_e+twoASquare_e;
					}
				}
			}
		}
	}
	//Variable setup for lastDraw struct
	lastDrawingData->numberOfPixelForLastDraw = 0;
	lastDrawingData->lastFirstPointX = x_center;
	lastDrawingData->lastFirstPointY = y_center;
	lastDrawingData->lastSecondPointX = x_radius;
	lastDrawingData->lastSecondPointY = y_radius;
	if (x_radius>0){
		if(y_radius>0){
			//variables for drawing ellipse
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
				//printf("L1");
				lastDrawingData->lastDrawnPixelMemory[lastDrawingData->numberOfPixelForLastDraw] = get_pixel_color2(x_center+x,y_center+y);
				alt_up_pixel_buffer_dma_draw(pixel_buffer,color, x_center+x,y_center+y); //Q1
				lastDrawingData->numberOfPixelForLastDraw ++;
				lastDrawingData->lastDrawnPixelMemory[lastDrawingData->numberOfPixelForLastDraw] = get_pixel_color2( x_center-x,y_center+y);
				alt_up_pixel_buffer_dma_draw(pixel_buffer,color, x_center-x,y_center+y); //Q2
				lastDrawingData->numberOfPixelForLastDraw ++;
				lastDrawingData->lastDrawnPixelMemory[lastDrawingData->numberOfPixelForLastDraw] = get_pixel_color2( x_center-x,y_center-y);
				alt_up_pixel_buffer_dma_draw(pixel_buffer,color, x_center-x,y_center-y); //Q3
				lastDrawingData->numberOfPixelForLastDraw ++;
				lastDrawingData->lastDrawnPixelMemory[lastDrawingData->numberOfPixelForLastDraw] = get_pixel_color2( x_center+x,y_center-y);
				alt_up_pixel_buffer_dma_draw(pixel_buffer,color, x_center+x,y_center-y); //Q4
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
			//printf("L2");
			lastDrawingData->lastDrawnPixelMemory[lastDrawingData->numberOfPixelForLastDraw] = get_pixel_color2(x_center+x,y_center+y);
			alt_up_pixel_buffer_dma_draw(pixel_buffer,color, x_center+x,y_center+y); //Q1
			lastDrawingData->numberOfPixelForLastDraw ++;
			lastDrawingData->lastDrawnPixelMemory[lastDrawingData->numberOfPixelForLastDraw] = get_pixel_color2( x_center-x,y_center+y);
			alt_up_pixel_buffer_dma_draw(pixel_buffer,color, x_center-x,y_center+y); //Q2
			lastDrawingData->numberOfPixelForLastDraw ++;
			lastDrawingData->lastDrawnPixelMemory[lastDrawingData->numberOfPixelForLastDraw] = get_pixel_color2( x_center-x,y_center-y);
			alt_up_pixel_buffer_dma_draw(pixel_buffer,color, x_center-x,y_center-y); //Q3
			lastDrawingData->numberOfPixelForLastDraw ++;
			lastDrawingData->lastDrawnPixelMemory[lastDrawingData->numberOfPixelForLastDraw] = get_pixel_color2( x_center+x,y_center-y);
			alt_up_pixel_buffer_dma_draw(pixel_buffer,color, x_center+x,y_center-y); //Q4
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
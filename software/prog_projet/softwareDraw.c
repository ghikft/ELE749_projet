/* Description
* Library used to for the 2d drawing
* Author: Nicolas & Robin
*********************************************************************/
#include "softwareDraw.h"
#include "alt_types.h"
#include "system.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include <stdio.h>
#include "project_variables.h"

#define TRUE	1
#define FALSE	0

#define H_RES 640
#define V_RES 480

int absoluteV(int x);
void soft_draw_line_high(int x1, int y1,
    				int x2, int y2,
    				int color, int erasePreviousWork, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer);

void soft_draw_line_low(int x1, int y1,
    				int x2, int y2,
    				int color, int erasePreviousWork, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer);


uint32_t soft_empty_rectangle_draw(int x_left, int y_top,
	int x_right, int y_bottom,
	int color, int erasePreviousWork, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer)
{
	/**************************************************************************
	 * soft_empty_rectangle_draw
	 **************************************************************************
	 * Parameters
	 * x_left	: x coordinate of the top left corner
	 * y_top	: y coordinate of the top left corner
	 * x_right	: x coordinate of the bottom right corner
	 * y_bottom : y coordinate of the botom right corner
	 * color	: color to dra the empty rectangle
	 * erasePreviousWork	: Delete the previous rectangle stored in lastDrawingData 1=erase 0 = not erase
	 * lastDrawingData		: Structure that save multiple variable used to keep track of previous 
	 *						  shape drawn during the interactive draw
	 * pixel_buffer is the pointer used to write in the pixel_buffer of the video pipeline
	 *
	 * Return value
	 * return 0 if invalid coordinate otherwise return the number of pixel drawn
	 *
	 * Side effects
	 * x_left can'T equal x_right. it is the same for the y top and bottom
	 * Draw an empty rectangle with two coordinate. 
	 *************************************************************************/
	
	int tempReorder;
	int x;
	int y;
	//check if it is required to erase the rectangle but avoid doing so
	//if it it the first time de rectangle is drawn with (firstErase)
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
		//erase the rectangle with stored pixel value under it
		for (int j = 0; j < lastDrawingData->numberOfPixelForLastDraw; j++) {
			alt_up_pixel_buffer_dma_draw(pixel_buffer, lastDrawingData->lastPixelMemory[j].color,
				lastDrawingData->lastPixelMemory[j].x, lastDrawingData->lastPixelMemory[j].y); 
		}
	}
	lastDrawingData->numberOfPixelForLastDraw = 0;
	
	//only draw the rectangle if the dimension are 2 or greater for horizontal and vertical
	if ((x_left != x_right) && (y_top != y_bottom)) {
		//reorder point so the x_left coordinate are always smaller 
		//than x_right and y_top smaller than y_bottom
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
		//Start by saving the pixel where the rectangle will be drawn then draw the pixel.
		//It is done clock-wise from left-top corner
		x = x_left;
		y = y_top;
		
		lastDrawingData->numberOfPixelForLastDraw = 0;
		uint32_t nbPixels = 0;
		//draw top line
		while (x < x_right) {
			lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].color = get_pixel_color2(x, y);
			lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].x = x;
			lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].y = y; 
			alt_up_pixel_buffer_dma_draw(pixel_buffer, color, x, y);
			x++;
			lastDrawingData->numberOfPixelForLastDraw++;
			nbPixels++;
		}
		//draw right veritcal line
		while (y < y_bottom) {
			lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].color = get_pixel_color2(x, y);
			lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].x = x;
			lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].y = y;
			alt_up_pixel_buffer_dma_draw(pixel_buffer, color, x, y);
			y++;
			lastDrawingData->numberOfPixelForLastDraw++;
			nbPixels++;
		}
		//draw bottom line
		while (x > x_left) {
			lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].color = get_pixel_color2(x, y);
			lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].x = x;
			lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].y = y;
			alt_up_pixel_buffer_dma_draw(pixel_buffer, color, x, y);
			x--;
			lastDrawingData->numberOfPixelForLastDraw++;
			nbPixels++;
		}
		//draw left line
		while (y > y_top) {
			lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].color = get_pixel_color2(x, y);
			lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].x = x;
			lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].y = y;
			alt_up_pixel_buffer_dma_draw(pixel_buffer, color, x, y);
			y--;
			lastDrawingData->numberOfPixelForLastDraw++;
			nbPixels++;
		}
		return nbPixels;
	}
	return 0;
}

void init_last_drawing_Var(lastDrawingVar* lastDrawingData) {
	/**************************************************************************
	 * init_last_drawing_Var
	 **************************************************************************
	 * Parameters
	 *lastDrawingData		: Structure that save multiple variable used to keep track of previous
	 *						  shape drawn during the interactive draw
	  *
	 * Return value
	 * none
	 *
	 * Side effects
	 * none
	 *************************************************************************/
	lastDrawingData->firstErase = TRUE;
	lastDrawingData->lastFirstPointX = 0;
	lastDrawingData->lastFirstPointY = 0;
	lastDrawingData->lastSecondPointX = 0;
	lastDrawingData->lastSecondPointY = 0;
	lastDrawingData->numberOfPixelForLastDraw = 0;
}

int draw_empty_ellipse(int x_center, int y_center, int x_radius, int y_radius, 
						int color, alt_up_pixel_buffer_dma_dev* pixel_buffer, 
						int erasePreviousWork, lastDrawingVar* lastDrawingData){

	/**************************************************************************
	 * draw_empty_ellipse
	 **************************************************************************
	 * Parameters
	 * x_center	: x coordinate of center of the ellipse
	 * y_center	: y coordinate of center of the ellipse
	 * x_radius	: radius of the ellipse in X
	 * y_radius : radius of the ellipse in Y
	 * color	: color to draw the empty ellipse
	 * erasePreviousWork	: Delete the previous rectangle stored in lastDrawingData 1=erase 0 = not erase
	 * lastDrawingData		: Structure that save multiple variable used to keep track of previous 
	 *						  shape drawn during the interactive draw
	 * pixel_buffer is the pointer used to write in the pixel_buffer of the video pipeline
	 *
	 * Return value
	 * return 0 if ellipse was not drawn
	 * return 1 if ellipse was drawn
	 *
	 * Side effects
	 * none
	 *************************************************************************/
	
	//flip the radiuses if they are negative
	if(x_radius<0){
		x_radius = x_radius*-1;
	}
	if(y_radius<0){
		y_radius=y_radius*-1;
	}

	int ratioOK = 1;
	if (x_radius>y_radius){
		if (x_radius/y_radius >3){
			ratioOK = 0;
		}
	}
	else{
		if(y_radius/x_radius >3){
			ratioOK = 0;
		}
	}

	//Erase previous ellipse here TODO
	if(erasePreviousWork){
		for (int j=0; j<lastDrawingData->numberOfPixelForLastDraw;j++){
			alt_up_pixel_buffer_dma_draw(pixel_buffer,lastDrawingData->lastPixelMemory[j].color, 
										lastDrawingData->lastPixelMemory[j].x ,lastDrawingData->lastPixelMemory[j].y); //Q3
		}
	}
	//Variable setup for lastDraw struct
	lastDrawingData->numberOfPixelForLastDraw = 0;
	if (x_radius>4 && y_radius>4 && ratioOK){
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
		while(stoppingX > stoppingY){
			if ((x>0 && y>0)||erasePreviousWork==0){
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].x = x_center+x;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].y = y_center+y;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].color = get_pixel_color2(x_center+x,y_center+y);
				alt_up_pixel_buffer_dma_draw(pixel_buffer,color, x_center+x,y_center+y); //Q1
				lastDrawingData->numberOfPixelForLastDraw ++;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].x = x_center-x;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].y = y_center+y;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].color = get_pixel_color2(x_center-x,y_center+y);
				alt_up_pixel_buffer_dma_draw(pixel_buffer,color, x_center-x,y_center+y); //Q2
				lastDrawingData->numberOfPixelForLastDraw ++;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].x = x_center-x;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].y = y_center-y;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].color = get_pixel_color2(x_center-x,y_center-y);
				alt_up_pixel_buffer_dma_draw(pixel_buffer,color, x_center-x,y_center-y); //Q3
				lastDrawingData->numberOfPixelForLastDraw ++;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].x = x_center+x;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].y = y_center-y;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].color = get_pixel_color2(x_center+x,y_center-y);
				alt_up_pixel_buffer_dma_draw(pixel_buffer,color, x_center+x,y_center-y); //Q4
				lastDrawingData->numberOfPixelForLastDraw ++;
			}
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
		while (stoppingX < stoppingY) {
			//printf("L2");
			if ((x>0 && y>0)||erasePreviousWork==0){
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].x = x_center+x;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].y = y_center+y;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].color = get_pixel_color2(x_center+x,y_center+y);
				alt_up_pixel_buffer_dma_draw(pixel_buffer,color, x_center+x,y_center+y); //Q1
				lastDrawingData->numberOfPixelForLastDraw ++;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].x = x_center-x;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].y = y_center+y;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].color = get_pixel_color2(x_center-x,y_center+y);
				alt_up_pixel_buffer_dma_draw(pixel_buffer,color, x_center-x,y_center+y); //Q2
				lastDrawingData->numberOfPixelForLastDraw ++;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].x = x_center-x;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].y = y_center-y;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].color = get_pixel_color2(x_center-x,y_center-y);
				alt_up_pixel_buffer_dma_draw(pixel_buffer,color, x_center-x,y_center-y); //Q3
				lastDrawingData->numberOfPixelForLastDraw ++;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].x = x_center+x;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].y = y_center-y;
				lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].color = get_pixel_color2(x_center+x,y_center-y);
				alt_up_pixel_buffer_dma_draw(pixel_buffer,color, x_center+x,y_center-y); //Q4
				lastDrawingData->numberOfPixelForLastDraw ++;
			}
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
		return 1;
	}
	else{
		return 0;
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

void flood_fill_zone(int startX, int startY, int fillColor, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	/**************************************************************************
	 * flood fill a zone
	 **************************************************************************
	 * Parameters
	 * startX : x coordinate of the pixel
	 * startY : y coordinate of the pixel
	 * fillColor : color to fill
	 *
	 * Valeur de retour
	 * none
	 *
	 * Side effects
	 * none
	 *************************************************************************/
	int initialColor = get_pixel_color2(startX, startY);
	//printf("initial color %d\n\r",initialColor);
	if (fillColor == initialColor) {
		//printf("FILL early exit\n\r");
	}
	else {
		//printf("lunch recursion\n\r");
		flood_fill_sub(startX, startY, fillColor, initialColor, pixel_buffer);
		//printf("END_FILL\n\r");
	}
}

char flood_fill_sub(int x, int y, int fillColor, int initialColor, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	/**************************************************************************
	 * flood fill a zone of a the color of the selected pixel
	 **************************************************************************
	 * Parameters
	 * startX : x coordinate of the pixel
	 * startY : y coordinate of the pixel
	 * fillColor : color to fill
	 * initialColor : color to replace
	 *
	 * Valeur de retour
	 * none
	 *
	 * Side effects
	 * none
	 *************************************************************************/
	
	//check if outside drawing zone, if the current color is diffiernt from the initailColor to replace
	//or if the currentColor is the same as the one we are filling with
	if (x <= DRAWING_ZONE_LEFT_LIMIT || x > RIGHT_LIMIT || y<TOP_LIMIT || y> BOTTOM_LIMIT) {
		//printf("outside zone\n\r");
		return 0;
	}
	else if (get_pixel_color2(x, y) != initialColor) {
		//printf("not the same color as init\n\r");
		return 0;
	}
	else if (get_pixel_color2(x, y) == fillColor) {
		//printf("same as fill\n\r");
		return 0;
	}
	//replace the pixel value with the fill call
	alt_up_pixel_buffer_dma_draw(pixel_buffer, fillColor, x, y);

	//call itself for the adjacent for pixel(no diagonal)
	flood_fill_sub(x + 1, y, fillColor, initialColor, pixel_buffer);
	flood_fill_sub(x - 1, y, fillColor, initialColor, pixel_buffer);
	flood_fill_sub(x, y + 1, fillColor, initialColor, pixel_buffer);
	flood_fill_sub(x, y - 1, fillColor, initialColor, pixel_buffer);
	return 1;
}

void fill_to_edge_zone(int startX, int startY, int fillColor, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	/**************************************************************************
	 * wraper for fill_to_edge_sub
	 **************************************************************************
	 * Parameters
	 * startX : x coordinate of the pixel
	 * startY : y coordinate of the pixel
	 * fillColor : color to fill
	 *
	 * Valeur de retour
	 * none
	 *
	 * Side effects
	 * none
	 *************************************************************************/
	fill_to_edge_sub(startX, startY, fillColor, pixel_buffer);
}

char fill_to_edge_sub(int x, int y, int fillColor, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	/**************************************************************************
	 * flood fill a zone to until it encounters a specific colour
	 **************************************************************************
	 * Parameters
	 * startX : x coordinate of the pixel
	 * startY : y coordinate of the pixel
	 * fillColor : color to fill
	 *
	 * Valeur de retour
	 * none
	 *
	 * Side effects
	 * none
	 *************************************************************************/

	//check if outside drawing zone, if the current color is diffiernt from the initailColor to replace
	//or if the currentColor is the same as the one we are filling with
	if (x <= LEFT_LIMIT || x > RIGHT_LIMIT || y<TOP_LIMIT || y> BOTTOM_LIMIT) {
		//printf("outside zone\n\r");
		return 0;
	}
	//else if (get_pixel_color2(x, y) != initialColor) {
		//printf("not the same color as init\n\r");
		//return 0;
	//}
	else if (get_pixel_color2(x, y) == fillColor) {
		//printf("same as fill\n\r");
		return 0;
	}
	//replace the pixel value with the fill call
	alt_up_pixel_buffer_dma_draw(pixel_buffer, fillColor, x, y);

	//call itself for the adjacent for pixel(no diagonal)
	fill_to_edge_sub(x + 1, y, fillColor, pixel_buffer);
	fill_to_edge_sub(x - 1, y, fillColor, pixel_buffer);
	fill_to_edge_sub(x, y + 1, fillColor, pixel_buffer);
	fill_to_edge_sub(x, y - 1, fillColor, pixel_buffer);
	return 1;
}

void soft_draw_line(int x1, int y1,
    				int x2, int y2,
    				int color, int erasePreviousWork, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer){
	/**************************************************************************
	 * Draws a line between 2 points
	 **************************************************************************
	 * Parameters
	 * x1 : x coordinate of the first point
	 * y1 : y coordinate of the first point
	 * x2 : x coordinate of the second point
	 * y2 : y coordinate of the second point
	 * color : color of the line
	 * erasePreviousWork : Delete the previous rectangle stored in lastDrawingData 1=erase 0 = not erase
	 * lastDrawingData : Structure that save multiple variable used to keep track of previous 
	 *					 shape drawn during the interactive draw
	 * pixel_buffer is the pointer used to write in the pixel_buffer of the video pipeline
	 *
	 * Valeur de retour
	 * none
	 *
	 * Side effects
	 * none
	 *************************************************************************/
	//erase previous work
	if(erasePreviousWork){
		for (int j=0; j<lastDrawingData->numberOfPixelForLastDraw;j++){
			alt_up_pixel_buffer_dma_draw(pixel_buffer,lastDrawingData->lastPixelMemory[j].color, 
			lastDrawingData->lastPixelMemory[j].x ,lastDrawingData->lastPixelMemory[j].y); //Q3
		}
	}
	
	lastDrawingData->numberOfPixelForLastDraw=0;

	//logic to flip the 2 points and to call the correct function
	if (absoluteV(y2-y1) < absoluteV(x2-x1)){
		if(x1>x2){
			soft_draw_line_low(x2,y2,x1,y1,color,erasePreviousWork,lastDrawingData,pixel_buffer);
		}
		else{
			soft_draw_line_low(x1,y1,x2,y2,color,erasePreviousWork,lastDrawingData,pixel_buffer);
		}
	}
	else{
		if(y1>y2){
			soft_draw_line_high(x2,y2,x1,y1,color,erasePreviousWork,lastDrawingData,pixel_buffer);
		}
		else{
			soft_draw_line_high(x1,y1,x2,y2,color,erasePreviousWork,lastDrawingData,pixel_buffer);
		}
	}
	
}

void soft_draw_line_low(int x1, int y1,
    				int x2, int y2,
    				int color, int erasePreviousWork, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer){
	/**************************************************************************
	 * Draws a line between 2 points in the bottom quadrants
	 **************************************************************************
	 * Parameters
	 * x1 : x coordinate of the first point
	 * y1 : y coordinate of the first point
	 * x2 : x coordinate of the second point
	 * y2 : y coordinate of the second point
	 * color : color of the line
	 * erasePreviousWork : Delete the previous rectangle stored in lastDrawingData 1=erase 0 = not erase
	 * lastDrawingData : Structure that save multiple variable used to keep track of previous 
	 *					 shape drawn during the interactive draw
	 * pixel_buffer is the pointer used to write in the pixel_buffer of the video pipeline
	 *
	 * Valeur de retour
	 * none
	 *
	 * Side effects
	 * none
	 *************************************************************************/
	
	int dx = x2-x1;
	int dy = y2-y1;

	int yi=1;
	if(dy<0){
		yi=-1;
		dy=-dy;
	}
	int d = (2*dy)-dx;
	int x,y;
	y=y1;
	for(x=x1;x<x2;x++){
		//save pixel data to erase later
		lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].x = x;
		lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].y = y;
		lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].color = get_pixel_color2(x,y);
		lastDrawingData->numberOfPixelForLastDraw++;
		alt_up_pixel_buffer_dma_draw(pixel_buffer, color, x, y);
		if(d>0){
			y=y+yi;
			d=d+(2*(dy-dx));
		}
		else{
			d=d+2*dy;
		}
	}

}

void soft_draw_line_high(int x1, int y1,
    				int x2, int y2,
    				int color, int erasePreviousWork, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer){
/**************************************************************************
 * Draws a line between 2 points in the top quadrants
 **************************************************************************
	* Parameters
	* x1 : x coordinate of the first point
	* y1 : y coordinate of the first point
	* x2 : x coordinate of the second point
	* y2 : y coordinate of the second point
	* color : color of the line
	* erasePreviousWork : Delete the previous rectangle stored in lastDrawingData 1=erase 0 = not erase
	* lastDrawingData : Structure that save multiple variable used to keep track of previous 
	*					 shape drawn during the interactive draw
	* pixel_buffer is the pointer used to write in the pixel_buffer of the video pipeline
	*
	* Valeur de retour
	* none
	*
	* Side effects
	* none
	*************************************************************************/
	int dx = x2-x1;
	int dy = y2-y1;

	int xi=1;
	if(dx<0){
		xi=-1;
		dx=-dx;
	}
	int d = (2*dx)-dy;
	int x,y;
	x=x1;
	for(y=y1;y<y2;y++){
		//save pixel data to erase later
		lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].x = x;
		lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].y = y;
		lastDrawingData->lastPixelMemory[lastDrawingData->numberOfPixelForLastDraw].color = get_pixel_color2(x,y);
		lastDrawingData->numberOfPixelForLastDraw++;
		alt_up_pixel_buffer_dma_draw(pixel_buffer, color, x, y);
		if(d>0){
			x=x+xi;
			d=d+(2*(dx-dy));
		}
		else{
			d=d+2*dx;
		}
	}

}

int absoluteV(int x){
	/**************************************************************************
	 * Calculate absolute value
	 **************************************************************************
	 * Parameters
	 * x : value to calculate the absolute value on
	 *
	 * Valeur de retour
	 * absolute value of x
	 *
	 * Side effects
	 * none
	 *************************************************************************/
	if(x<0){
		return x*-1;
	}
	else{
		return x;
	}
}

void soft_copy_paste(int x1_copy, int y1_copy, int x2_copy, int y2_copy, int x1_paste, int y1_paste, int cut, char color, alt_up_pixel_buffer_dma_dev* pixel_buffer){
	/**************************************************************************
	 * copy paste a section of the screen to another
	 **************************************************************************
	 * Parameters
	 * x1_copy : x coordinate of the first point
	 * y1_copy : y coordinate of the first point
	 * x2_copy : x coordinate of the second point
	 * y2_copy : y coordinate of the second point
	 * x1_paste : x coordinate of the top corner of the paste
	 * y1_paste : y coordinate of the top corner of the paste
	 * cut : 1 to cut and paste, 0 to copy paste
	 * color : color of the fill if cut
	 * pixel_buffer is the pointer used to write in the pixel_buffer of the video pipeline
	 *
	 * Valeur de retour
	 * none
	 *
	 * Side effects
	 * none
	 *************************************************************************/
	char copyMem[307200];
	int nbPts=0;

	//flip x and y points if out of order
	if (x1_copy>x2_copy){
		int tempX = x2_copy;
		x2_copy = x1_copy;
		x1_copy = tempX;
	}
	if (y1_copy>y2_copy){
		int tempY = y2_copy;
		y2_copy = y1_copy;
		y1_copy = tempY;
	}

	nbPts = ((x2_copy-x1_copy-1)*(y2_copy-y1_copy-1));
	int x_cpy, y_cpy;
	x_cpy = x1_copy+1;
	y_cpy = y1_copy+1;
	//copy pixel values to mem
	for (int i=0;i<nbPts;i++){
		copyMem[i] = get_pixel_color2(x_cpy,y_cpy);
		if (x_cpy == (x2_copy-1)){
			x_cpy = x1_copy+1;
			y_cpy++;
		}
		else{
			x_cpy++;
		}
	}
	//erase values if cut paste
	x_cpy = x1_copy+1;
	y_cpy = y1_copy+1;
	if (cut){
		for (int i=0;i<nbPts;i++){
			alt_up_pixel_buffer_dma_draw(pixel_buffer, color,x_cpy,y_cpy);
			if (x_cpy == (x2_copy-1)){
				x_cpy = x1_copy+1;
				y_cpy++;
			}
			else{
				x_cpy++;
			}
		}
	}

	//paste loop
	int x_paste, y_paste;
	x_paste = x1_paste;
	y_paste = y1_paste;
	for (int i=0;i<nbPts;i++){
		if(x_paste>x1_paste+((x2_copy-1)-(x1_copy+1))){
			x_paste = x1_paste;
			y_paste++;
		}
		if(x_paste < H_RES && y_paste < V_RES){
			alt_up_pixel_buffer_dma_draw(pixel_buffer, copyMem[i],x_paste,y_paste);
		}
		
		x_paste++;
	}
}
/*
 * main.c
 * nouvelle valeur timeout ps2 0x001fffff
 *  Created on: Jun 12, 2023
 *      Author: Nicolas et robin
 */
/* C Library */
#include <stdio.h>		// printf()
#include <string.h> 	// sprintf()
#include <time.h> 		// time(NULL)
#include <stdlib.h> 	// srand, rand
#include <stdint.h>
#include <time.h> 		// time(NULL)


/* Altera-specific */
#include "alt_types.h"
#include "system.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "alt_types.h"
#include "sys/alt_irq.h"
#include "sys/alt_timestamp.h"

# include "sys/alt_stdio.h"
/*Module Hardware*/
#include "recfiller.h"
#include "hwellipse.h"
#include "project_variables.h"

/*Module software*/
#include "softwareDraw.h"
#include "nios_draw.h"
#include "userIO.h"
#include "snake.h"
#include "pong.h"
/* Mouse driver */
#include "ps2_mouse.h"
#include "io.h"

#define TIMER_CONVERSION 50000
#define TIMER_STAT_REG_OFT		0 	// status register
#define TIMER_CTRL_REG_OFT		1 	// control register
#define TIMER_PERIODL_REG_OFT 	2 	// period register, bits 15:0
#define TIMER_PERIODH_REG_OFT 	3 	// period register, bits 31:16


void test_USER_IO(void) {
	char switchStatus = 0;
	char buttonStatus = 0;
	char remoteStatus = 0;
	get_switche(SWITCHE_BASE, &switchStatus);
	get_button(BUTTON_BASE, &buttonStatus);
	get_remote(REMOTE_BASE, &remoteStatus);

	printf("Return button: %d   |   Mode_switche: %d\n\r",buttonStatus,switchStatus);
	printf("Pong REMOTE|   UP: %d   |   DOWN: %d\n\r", (remoteStatus & 0x20), (remoteStatus & 0x10));
	printf("Snake REMOTE|   UP: %d   |   DOWN: %d   |   LEFT: %d   |   RIGHT: %d\n\r", (remoteStatus & 0x08), (remoteStatus & 0x04), (remoteStatus & 0x02), (remoteStatus & 0x01));
}

#define NUMBER_OF_TEST_LOOP 1000
void empty_software_rectangle_test(int numberIter,alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	alt_timestamp_start();
	uint32_t nbPixels = 0;
	lastDrawingVar lastDrawingData;
	init_last_drawing_Var(&lastDrawingData);
	uint32_t temp0 = alt_timestamp();
	int nbDrawMiss = 0;
	uint32_t intermediatePixelCount = 0;
	
	for (int j = 0; j < numberIter; j++) {
		//generate random number
		int x1 = rand() % 640;
		int y1 = rand() % 480;
		int x2 = rand() % 640;
		int y2 = rand() % 480;
		int color = rand() % 255;
		intermediatePixelCount = soft_empty_rectangle_draw(x1, y1, x2, y2, color, NOT_ERASE_PREVIOUS_WORK, &lastDrawingData, pixel_buffer);
		if (intermediatePixelCount == 0) {
			j--;//make the test draw another rectangle since the dimension where invalid
			nbDrawMiss++;
		}
		else nbPixels += intermediatePixelCount;
	}
	
	uint32_t temp1 = alt_timestamp();
	uint32_t numberOfCycle = temp1 - temp0;
	float totalTime = (float)numberOfCycle * 2.0e-8;
	//print Result
	printf("Random EMPTY RECTANGLE test: Number of draw missed %d\n\r",nbDrawMiss);
	printf("NUmber of rectangle: %d   |   number of cycle: %lu   |   Total time(s): %.2f  |   Pixel/s: %0.1f   |   cycle/pixel : %.2f\n\n\r",
		numberIter, numberOfCycle, totalTime, (float)nbPixels/ totalTime, (float)numberOfCycle /nbPixels);
}

void filled_software_rectangle_test(int numberIter, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	alt_timestamp_start();
	uint32_t nbPixels = 0;
	lastDrawingVar lastDrawingData;
	init_last_drawing_Var(&lastDrawingData);
	int nbDrawMiss = 0;
	uint32_t temp0 = alt_timestamp();
	for (int j = 0; j < numberIter; j++) {
		//generate random number
		int x1 = rand() % 640;
		int y1 = rand() % 480;
		int x2 = rand() % 640;
		int y2 = rand() % 480;
		int color = rand() % 255;
		int width;
		int lenght;
		
		int fill_x;
		int fill_y;
		if (x1 > x2) {
			width = x1 - x2;
			fill_x = x1 - 1;
		}
		else {
			width = x2 - x1;
			fill_x = x1 + 1;
		}
		if (y1 > y2) {
			lenght = y1 - y2;
			fill_y = y1 - 1;
		}
		else {
			lenght = y2 - y1;
			fill_y = y1 + 1;
		}
			//fill the rectangle if drawn the our fill zone function
		if (soft_empty_rectangle_draw(x1, y1, x2, y2, color, NOT_ERASE_PREVIOUS_WORK, &lastDrawingData, pixel_buffer)) {
			fill_to_edge_zone(fill_x,fill_y, color, pixel_buffer);
			nbPixels = nbPixels + (lenght * width);
		}
		else {
			j--;//make the test draw another rectangle since the dimension where invalid
			nbDrawMiss ++;
		}
			
		
	}
	uint32_t temp1 = alt_timestamp();
	uint32_t numberOfCycle = temp1 - temp0;
	float totalTime = (float)numberOfCycle * 2.0e-8;
	//print Result
	printf("Random FILLED RECTANGLE test:  Number of draw missed %d\n\r", nbDrawMiss);
	printf("NUmber of rectangle: %d   |   number of cycle: %lu   |   Total time(s): %.2f  |   Pixel/s: %0.1f   |   cycle/pixel : %.2f\n\n\r",
		numberIter, numberOfCycle, totalTime, (float)nbPixels / totalTime, (float)numberOfCycle / nbPixels);
}

void software_line_test(int numberIter, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	alt_timestamp_start();
	uint32_t nbPixels = 0;
	lastDrawingVar lastDrawingData;
	init_last_drawing_Var(&lastDrawingData);
	uint32_t temp0 = alt_timestamp();	
	
	for (int j = 0; j < numberIter; j++) {
		//generate random number
		int x1 = rand() % 640;
		int y1 = rand() % 480;
		int x2 = rand() % 640;
		int y2 = rand() % 480;
		int color = rand() % 255;
		soft_draw_line(x1, y1, x2, y2, color, NOT_ERASE_PREVIOUS_WORK, &lastDrawingData, pixel_buffer);
		nbPixels = nbPixels + lastDrawingData.numberOfPixelForLastDraw;
		
	}

	uint32_t temp1 = alt_timestamp();
	uint32_t numberOfCycle = temp1 - temp0;
	float totalTime = (float)numberOfCycle * 2.0e-8;
	//print Result
	//printf("t0: %lu    t1: %lu\n\r", temp0, temp1);
	printf("Random LINE test: Number of draw missed %d\n\r", 0);
	printf("NUmber of rectangle: %d   |   number of cycle: %lu   |   Total time(s): %.2f  |   Pixel/s: %0.1f   |   cycle/pixel : %.2f\n\n\r",
		numberIter, numberOfCycle, totalTime, (float)nbPixels / totalTime, (float)numberOfCycle / nbPixels);
}

void software_empty_ellipse_test(int numberIter, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	alt_timestamp_start();
	uint32_t nbPixels = 0;
	lastDrawingVar lastDrawingData;
	init_last_drawing_Var(&lastDrawingData);
	int nbDrawMiss = 0;
	uint32_t temp0 = alt_timestamp();
	for (int j = 0; j < numberIter; j++) {
		//generate random number
		int centerX = rand() % 640;
		int centerY = rand() % 480;
		int radiusX = rand() % 640;
		int radiusY = rand() % 480;
		int color = rand() % 255;
		
		//check if the ellipse is drawn or retry it if not
		if (draw_empty_ellipse(centerX, centerY, radiusX, radiusY, color, pixel_buffer,
			NOT_ERASE_PREVIOUS_WORK,&lastDrawingData)){
			
			nbPixels = nbPixels + lastDrawingData.numberOfPixelForLastDraw;
		}
		else {
			j--;//make the test draw another rectangle since the dimension where invalid
			nbDrawMiss++;
		}


	}
	uint32_t temp1 = alt_timestamp();
	uint32_t numberOfCycle = temp1 - temp0;
	float totalTime = (float)numberOfCycle * 2.0e-8;
	//print Result
	printf("Random EMPTY ELLIPSE test:  Number of draw missed %d\n\r", nbDrawMiss);
	printf("NUmber of rectangle: %d   |   number of cycle: %lu   |   Total time(s): %.2f  |   Pixel/s: %0.1f   |   cycle/pixel : %.2f\n\n\r",
		numberIter, numberOfCycle, totalTime, (float)nbPixels / totalTime, (float)numberOfCycle / nbPixels);
}

void software_copy_paste_test(int numberIter, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	alt_timestamp_start();
	uint32_t nbPixels = 0;
	uint32_t temp0 = alt_timestamp();
	for (int j = 0; j < numberIter; j++) {
		//generate random number
		int copyX1 = rand() % 640;
		int copyY1 = rand() % 480;
		int copyX2 = rand() % 640;
		int copyY2 = rand() % 480;

		int pasteX = rand() % 640;
		int pasteY = rand() % 480;
		int color = rand() % 255;
		int width;
		int lenght;
		soft_copy_paste(copyX1, copyY1, copyX2, copyY2, pasteX, pasteY, 0, color, pixel_buffer);
		if (copyX1 > copyX2) {
			
		}
		//calculate the number of pixel drawn
		if (copyX1 > copyX2) {
			width = copyX1 - copyX2;
		}
		else width = copyX2 - copyX1;
		if (copyY1 > copyY2) {
			lenght = copyY1 - copyY2;
		}
		else lenght = copyY2 - copyY1;

		nbPixels = nbPixels + (uint32_t)(width * lenght);	
		
	}
	uint32_t temp1 = alt_timestamp();
	uint32_t numberOfCycle = temp1 - temp0;
	float totalTime = (float)numberOfCycle * 2.0e-8;
	//print Result
	printf("Random COPY PASTE test:  Number of draw missed %d\n\r", 0);
	printf("NUmber of rectangle: %d   |   number of cycle: %lu   |   Total time(s): %.2f  |   Pixel/s: %0.1f   |   cycle/pixel : %.2f\n\n\r",
		numberIter, numberOfCycle, totalTime, (float)nbPixels / totalTime, (float)numberOfCycle / nbPixels);
}

void software_cut_paste_test(int numberIter, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	alt_timestamp_start();
	uint32_t nbPixels = 0;
	uint32_t temp0 = alt_timestamp();
	for (int j = 0; j < numberIter; j++) {
		//generate random number
		int copyX1 = rand() % 640;
		int copyY1 = rand() % 480;
		int copyX2 = rand() % 640;
		int copyY2 = rand() % 480;

		int pasteX = rand() % 640;
		int pasteY = rand() % 480;
		int color = rand() % 255;
		int width;
		int lenght;
		soft_copy_paste(copyX1, copyY1, copyX2, copyY2, pasteX, pasteY, 1, color, pixel_buffer);
		
		//calculate the number of pixel drawn
		if (copyX1 > copyX2) {
			width = copyX1 - copyX2;
		}
		else width = copyX2 - copyX1;
		if (copyY1 > copyY2) {
			lenght = copyY1 - copyY2;
		}
		else lenght = copyY2 - copyY1;
		//two time because there it draw a copy and replace the cut by another value
		nbPixels = nbPixels + (uint32_t)((width * lenght)*2);

	}
	
	uint32_t temp1 = alt_timestamp();
	uint32_t numberOfCycle = temp1 - temp0;
	float totalTime = (float)numberOfCycle * 2.0e-8;
	//print Result
	printf("Random CUT PASTE test:  Number of draw missed %d\n\r", 0);
	printf("NUmber of rectangle: %d   |   number of cycle: %lu   |   Total time(s): %.2f  |   Pixel/s: %0.1f   |   cycle/pixel : %.2f\n\n\r",
		numberIter, numberOfCycle, totalTime, (float)nbPixels / totalTime, (float)numberOfCycle / nbPixels);
}
context_t timer_context;
alt_up_ps2_dev* tim;

//#define IO_TEST
#define NIOS_DRAW_FUNC
//#define SNAKE_TEST
//#define PONG_TEST
int main(void){	

	/* PIXEL BUFFER setup and background display */
	alt_up_pixel_buffer_dma_dev *pixel_buffer;
	pixel_buffer=alt_up_pixel_buffer_dma_open_dev(PIXEL_BUFFER_DMA_0_NAME);
	
	if(pixel_buffer == NULL){
		alt_putstr("pixel buff is dead \n\r");
	}else{
		alt_putstr("pixel buff ok\n\r");
	}
	printf("clear screen\n");
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer,0);
	printf("draw background\n");
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0,0,640,480,BACKGROUD_COLOR,0);
	printf("start ps2\n");
	ps2_init(); 		// from ps2_mouse.h
	printf("init complete\n");

#ifdef PONG_TEST
	play_pong(pixel_buffer);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0,0,640,480,BACKGROUD_COLOR,0);
#endif
#ifdef SNAKE_TEST
	play_snake(pixel_buffer);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0,0,640,480,BACKGROUD_COLOR,0);
#endif
#ifdef IO_TEST
	while (1) {
		test_USER_IO();
		usleep(1000000);
	}
#endif
#ifdef DRAW_CARACTERISATION	
	//caracterisation test 
	filled_software_rectangle_test(100, pixel_buffer);
	empty_software_rectangle_test(1000,pixel_buffer);
	software_copy_paste_test(100, pixel_buffer);
	software_line_test(1000, pixel_buffer);
	software_empty_ellipse_test(1000, pixel_buffer);
	software_cut_paste_test(100, pixel_buffer);

#endif
#ifdef NIOS_DRAW_FUNC
	nios_draw(pixel_buffer);
#endif // NIOS_DRAW_FUNC
}
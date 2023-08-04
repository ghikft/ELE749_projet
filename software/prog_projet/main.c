/*
 * main.c
 * nouvelle valeur timeout ps2 0x001fffff
 *  Created on: Jun 12, 2023
 *      Author: ghikf et robin
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
/* Mouse driver */
#include "ps2_mouse.h"
#include "io.h"

#define TIMER_CONVERSION 50000
#define TIMER_STAT_REG_OFT		0 	// status register
#define TIMER_CTRL_REG_OFT		1 	// control register
#define TIMER_PERIODL_REG_OFT 	2 	// period register, bits 15:0
#define TIMER_PERIODH_REG_OFT 	3 	// period register, bits 31:16






//Screen border limts
//#define TOP_LIMIT 0
//#define LEFT_LIMIT 0
//#define RIGHT_LIMIT 639
//#define BOTTOM_LIMIT 479
//#define DRAWING_ZONE_LEFT_LIMIT 61

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
//void timer_write_period(alt_u32 timerBase, alt_u32 period)
//{
//	/**************************************************************************
//	 * start the timer in continuous mode with interrupts
//	 **************************************************************************
//	 * Parametres
//	 * timerBase: Base address of timer
//	 * period: desired period in ms
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * None
//	 *
//	 *************************************************************************/
//	period = period * TIMER_CONVERSION;
//	alt_u16 high, low;
//	/* split 32 bits period into two 16 bits */
//	high = (alt_u16)(period >> 16);
//	low = (alt_u16)(period & 0x0000FFFF);
//	/* write period */
//	IOWR(timerBase, TIMER_PERIODH_REG_OFT, high);
//	IOWR(timerBase, TIMER_PERIODL_REG_OFT, low);
//	/* setup timer for start in continuous mode w/ interrupt */
//	IOWR(timerBase, TIMER_CTRL_REG_OFT, 0x0007); // bits 0, 1, 2 actives
//}
//static void timer_0_ISR(void* context, alt_u32 id) 
////void timer_0_ISR(void* context)
//{
//	/**************************************************************************
//	 * Interrupt handler for timer 0, flashes the LEDs at the timer period
//	 **************************************************************************
//	 * Parametres
//	 * *context: conext of ISR
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * None
//	 *
//	 *************************************************************************/
//	//tim = alt_up_ps2_open_dev(TIMER_0_NAME);		//Connexion au module PS/2
//	context_t* ctxt = context;					//Structure qui contient le contexte pour
//												//communiquer avec l'interruption
//	 
//
//	///static alt_u8 ledPattern = 0x01; // intial template
//
//	
//}
//
//void start_timer(alt_u32 timerBase)
//{
//	/**************************************************************************
//	 * start the timer in continuous mode with interrupts
//	 **************************************************************************
//	 * Parametres
//	 * timerBase: Base address of timer
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * None
//	 *
//	 *************************************************************************/
//
//	IOWR(timerBase, TIMER_CTRL_REG_OFT, 0b0111);
//}
//
//void stop_timer(alt_u32 timerBase)
//{
//	/**************************************************************************
//	 * Stop the timer
//	 **************************************************************************
//	 * Parametres
//	 * timerBase: Base address of timer
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * turns off the interrupt,
//	 * turns off continuous
//	 *
//	 *************************************************************************/
//
//	IOWR(timerBase, TIMER_CTRL_REG_OFT, 0b1000);
//}






#define NIOS_DRAW_FUNC
int main(void)
{	
	alt_u32 period = 8;
	//printf("debut main\n\r");
	alt_putstr("\n\r\n\r\n\rdebut du main \n\r");
	// Variables pour souris PS2
	int x_mov = 0;
	int y_mov = 0;
	int x_pos = 0;
	int y_pos = 0;
	unsigned char left_btn = 0;
	unsigned char right_btn = 0;
	//char drawingColor = BLACK;
	char pos_msg[100];
	unsigned char lastRight = 0;
	unsigned char lastLeft = 0;
	int lastCursorColor = 0;
	char startUsingTool=0;
	Point firstPoint, secondPoint;
	Cursor currentCursor;
	Cursor lastCursor;
	int drawFirstTimeAround = 1;
	tool currentTool = NO_TOOL;
	tool lastTool = NO_TOOL;
	currentCursor.x = 100;
	currentCursor.y = 100;
	lastCursor.x = 100;
	lastCursor.y = 100;
	mode programMode = DRAWING_MODE;
	char startButtonPressed = 0; 
	int selectedColor = BLACK;
	int cpyRngSelected =0;
	int drawCursor = 1;
	alt_u8 cursorMem[25];

	process_cursor_pos(&currentCursor, &x_pos, &y_pos);
	//Stop timer and setup the interrupt, then start with 100ms period (default)
	//stop_timer(TIMER_0_BASE);
	//timer_write_period(TIMER_0_BASE, period);
	//start_timer(TIMER_0_BASE);
	//alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ, timer_0_ISR, 0x0, 0x0);
	//if (alt_irq_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, 0x0, timer_0_ISR) == 0){
	//	alt_putstr("ISR REGISTERED\n\r");
	//}
	//alt_irq_register(PS2_0_IRQ, (void*)(&ps2_context), ps2_isr);
	//alt_irq_register(TIMER_0_IRQ, (void*)(&timer_context), (void*)timer_0_ISR);
	//timer_write_period(TIMER_0_BASE, period);
	//start_timer(TIMER_0_BASE);
	//Stop timer and setup the interrupt, then start with 100ms period (default)
	//stop_timer(TIMER_0_BASE);
	//init recfiller
	//recfiller_init(640, 480);
	//Init cursor at the top left of the drawing zone

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
	//recfiller_draw_rectangle(10, 10, 40, 60, 128);
	//init random number generator
	srand(time(NULL));
	//start the time stamp timer
	alt_timestamp_start();

	//TBD probablu useless
	firstPoint.x = 100;
	firstPoint.y = 100;
	secondPoint.x = 110;
	secondPoint.y = 110;

	
	//caracterisation test 
	filled_software_rectangle_test(100, pixel_buffer);
	empty_software_rectangle_test(1000,pixel_buffer);
	software_copy_paste_test(100, pixel_buffer);
	software_line_test(1000, pixel_buffer);
	software_empty_ellipse_test(1000, pixel_buffer);
	software_cut_paste_test(100, pixel_buffer);	
	
#ifdef NIOS_DRAW_IN_MAIN	

	//initiate the drawing zone and tool bar
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 62, 0, 640, 480, BACKGROUD_COLOR, 0);
	draw_tool_bar(currentTool , &lastDrawingData, pixel_buffer);
	draw_color_palette(selectedColor, &lastDrawingData, pixel_buffer);
	init_last_drawing_Var(&lastDrawingData);
	cursor_save(&currentCursor, cursorMem);
	/* main loop */
	while (1) {
		// process screen drawing during vertical blank
		if (!alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer)) {
			// process ps2 events during vertical blank
			if (ps2_process(&left_btn, &right_btn, &x_mov, &y_mov)) {
				x_pos += x_mov;
				y_pos -= y_mov;
			}  
			




			//if using tool only get the new cursor position
			if (startUsingTool == 1) {
				process_cursor_pos(&currentCursor, &x_pos, &y_pos);				
			}
			else {
				//cursor updated at the end of the loop
			}

			//need to press the start button before being able to do anything else
			if (startButtonPressed == 0) {
				start_button(currentTool, &startButtonPressed,&left_btn, &lastLeft, cursorMem/*&lastCursorColor*/, &currentCursor, &lastDrawingData, pixel_buffer);
				//draw_icon(currentTool, 1, &lastDrawingData, pixel_buffer);
				draw_color_palette(selectedColor, &lastDrawingData, pixel_buffer);
			}
			else {
				//Check for tool selection if not using a tool and cursor inside the tool bar
				
				tool_selection(&currentCursor, &currentTool, startUsingTool, &selectedColor, &left_btn, &lastDrawingData, pixel_buffer);
				/* process clicks */
				//
				if (left_btn) { //Draw during left click
					//if (currentCursor.x > DRAWING_ZONE_LEFT_LIMIT) {//limit the usage of tool while in the 
					if (currentTool == PENCIL) {
						lastLeft = 1;						
						if (startUsingTool==0){
							cursor_erase(&currentCursor, cursorMem, pixel_buffer);
							lastCursor.x = currentCursor.x;
							lastCursor.y = currentCursor.y;
							startUsingTool = 1;
						}
						alt_up_pixel_buffer_dma_draw_line(pixel_buffer,lastCursor.x,lastCursor.y,currentCursor.x,currentCursor.y,selectedColor,0);
						lastCursor.x = currentCursor.x;
						lastCursor.y = currentCursor.y;
						lastCursorColor = selectedColor;//was DRAW_COLOR
					}
					else if(currentTool == COLOR_SAMPLE){
						if(currentCursor.x>DRAWING_ZONE_LEFT_LIMIT){
							if (startUsingTool == 0){
								startUsingTool = 1;
								lastLeft = 1;
								cursor_erase(&currentCursor, cursorMem, pixel_buffer);
								selectedColor = get_pixel_color(currentCursor.x,currentCursor.y);
								printf("COLOR PICKER: %d", selectedColor);
							}
						}
					}
					else if (currentTool == EMPTY_RECTANGLE|| currentTool == FILLED_RECTANGLE) {
						if (startUsingTool == 0) {
							cursor_erase(&currentCursor, cursorMem, pixel_buffer);
							firstPoint.x = currentCursor.x;
							firstPoint.y = currentCursor.y;
							startUsingTool = 1;
							lastLeft = 1;
						}
						else {
							soft_empty_rectangle_draw(firstPoint.x, firstPoint.y,
								currentCursor.x, currentCursor.y,
								selectedColor, 1, &lastDrawingData, pixel_buffer);
						}
					}
					else if(currentTool == CPY_PASTE || currentTool == CUT_PASTE){
						if(currentCursor.x>DRAWING_ZONE_LEFT_LIMIT){
							if (startUsingTool == 0 && cpyRngSelected == 0) {
								//alt_up_pixel_buffer_dma_draw(pixel_buffer, lastCursorColor, currentCursor.x, currentCursor.y);
								cursor_erase(&currentCursor, &cursorMem, pixel_buffer);
								printf("first point at: X:%d Y:%d\n\r", currentCursor.x, currentCursor.y);
								firstPoint.x = currentCursor.x;
								firstPoint.y = currentCursor.y;
								startUsingTool = 1;
								lastLeft = 1;
								drawCursor = 0;
								soft_empty_rectangle_draw(firstPoint.x, firstPoint.y,
									currentCursor.x, currentCursor.y,
									0, 1, &lastDrawingData, pixel_buffer);
							}
							else if (cpyRngSelected){
								cpyRngSelected = 0;
								if (currentTool == CUT_PASTE){
									soft_empty_rectangle_draw(0, 0, 0, 0,1, 1, &lastDrawingData, pixel_buffer);
									soft_copy_paste(firstPoint.x,firstPoint.y,secondPoint.x,secondPoint.y,currentCursor.x,currentCursor.y,1,selectedColor, pixel_buffer);
									//soft_empty_rectangle_draw(0, 0, 0, 0,1, 1, &lastDrawingData, pixel_buffer);
								}
								else{
									soft_empty_rectangle_draw(0, 0, 0, 0,1, 1, &lastDrawingData, pixel_buffer);
									soft_copy_paste(firstPoint.x,firstPoint.y,secondPoint.x,secondPoint.y,currentCursor.x,currentCursor.y,0,0, pixel_buffer);
									//soft_empty_rectangle_draw(0, 0, 0, 0,1, 1, &lastDrawingData, pixel_buffer);
								}
								while(left_btn){
									ps2_process(&left_btn, &right_btn, &x_mov, &y_mov);
									process_cursor_pos(&currentCursor, &x_pos, &y_pos);	
								}
								startUsingTool = 0;
							}
							else {
								lastCursorColor = get_pixel_color(currentCursor.x,currentCursor.y);
								alt_up_pixel_buffer_dma_draw(pixel_buffer, lastCursorColor, currentCursor.x, currentCursor.y);
								soft_empty_rectangle_draw(firstPoint.x, firstPoint.y,
									currentCursor.x, currentCursor.y,
									0, 1, &lastDrawingData, pixel_buffer);
							}
						}
					}
					else if (currentTool == EMPTY_ELLIPSE|| currentTool == FILLED_ELLIPSE) {		//elipse
						if (startUsingTool == 0) {
							//alt_up_pixel_buffer_dma_draw(pixel_buffer, lastCursorColor, currentCursor.x, currentCursor.y);
							cursor_erase(&currentCursor, &cursorMem, pixel_buffer);
							printf("first point at: X:%d Y:%d\n\r", currentCursor.x, currentCursor.y);
							firstPoint.x = currentCursor.x;
							firstPoint.y = currentCursor.y;
							startUsingTool = 1;
							lastLeft = 1;
							draw_empty_ellipse(firstPoint.x, firstPoint.y,
								currentCursor.x - firstPoint.x, currentCursor.y - firstPoint.y,
								selectedColor, pixel_buffer, 0, &lastDrawingData);
						}
						else {
							lastLeft = 1;
							draw_empty_ellipse(firstPoint.x, firstPoint.y,
								currentCursor.x - firstPoint.x, currentCursor.y - firstPoint.y,
								selectedColor, pixel_buffer, 1, &lastDrawingData);
						}
					}
					else if (currentTool == FILL) {
						if (startUsingTool == 0) {
							startUsingTool = 1;
							lastLeft = 1;
							firstPoint.x = currentCursor.x;
							firstPoint.y = currentCursor.y;
							cursor_erase(&currentCursor, cursorMem, pixel_buffer);
							//printf("\n\n\nSTART_FILL first click\n\r");
							//printf("selected color: %d\n\r", selectedColor);						
						}
					}
					//}//clear in an exception it only activable when cursor is on the icon
					else if (currentTool == CLEAR) {
						if (currentCursor.x <= 29 && currentCursor.y < 464 && currentCursor.y > 437) {
							if (startUsingTool == 0) {
								startUsingTool = 1;
								lastLeft = 1;
							}
						}
					}
					else if(currentTool == LINE){
						if (startUsingTool == 0) {
							//alt_up_pixel_buffer_dma_draw(pixel_buffer, lastCursorColor, currentCursor.x, currentCursor.y);
							cursor_erase(&currentCursor, &cursorMem, pixel_buffer);
							firstPoint.x = currentCursor.x;
							firstPoint.y = currentCursor.y;
							startUsingTool = 1;
							lastLeft = 1;
							soft_draw_line(firstPoint.x,firstPoint.y,currentCursor.x,currentCursor.y,selectedColor,0,&lastDrawingData,pixel_buffer);
						}
						else{
							lastLeft = 1;
							soft_draw_line(firstPoint.x,firstPoint.y,currentCursor.x,currentCursor.y,selectedColor,1,&lastDrawingData,pixel_buffer);
						}
					}
					else startUsingTool = 0;//for testing while some tool are not created
				}
				else if (right_btn) { //erase whole screen if right click
					//alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 62, 0, 640, 480, BACKGROUD_COLOR, 0);
					//draw_tool_bar(&lastDrawingData, pixel_buffer);
					//draw_icon(currentTool, 1, &lastDrawingData, pixel_buffer);
					//if (!lastRight) {
					printf("right click (X:%d,Y:%d), CLEAR SCREEN\n\r", currentCursor.x, currentCursor.y);
					//}
					//lastRight = 1;
				}
				else { //clear click flags
					if (lastLeft) {
						alt_putstr("left released, STOP DRAWING\n\r");
						//rectangle
						if (currentTool == PENCIL ) {
							if (startUsingTool){
								startUsingTool = 0;
								lastLeft = 0;
								cursor_save(&currentCursor, cursorMem);
							}
						}
						else if(currentTool == COLOR_SAMPLE){
							if (startUsingTool){
								startUsingTool = 0;
								lastLeft = 0;
							}
						}
						else if(currentTool==EMPTY_RECTANGLE||currentTool==FILLED_RECTANGLE){
							if (startUsingTool == 1) {
								lastCursorColor = selectedColor;
								startUsingTool = 0;
								secondPoint.x = currentCursor.x;
								secondPoint.y = currentCursor.y;

								//draw the last rectangle
								soft_empty_rectangle_draw(firstPoint.x, firstPoint.y,
									secondPoint.x, secondPoint.y,
									selectedColor, 1, &lastDrawingData, pixel_buffer);
								//soft_empty_rectangle_draw(firstPoint.x, firstPoint.y,
								//	secondPoint.x, secondPoint.y,
								//	selectedColor, 0, &lastDrawingData, pixel_buffer);
								if (currentTool == FILLED_RECTANGLE) {
									alt_up_pixel_buffer_dma_draw_box(pixel_buffer, firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y, selectedColor, 0);
								}
								//save the background at new position to avoid missing a pixel
								cursor_save(&currentCursor, cursorMem);
								lastDrawingData.firstErase = 1;
								lastLeft = 0;
							}
						}
						else if(currentTool == CPY_PASTE || currentTool == CUT_PASTE){
							if (startUsingTool == 1) {
								cpyRngSelected = 1;
								secondPoint.x = currentCursor.x;
								secondPoint.y = currentCursor.y;
								//lastCursorColor = get_pixel_color(secondPoint.x,secondPoint.y);
								printf("second point at: X:%d Y:%d\n\r", currentCursor.x, currentCursor.y);
								soft_empty_rectangle_draw(0, 0, 0, 0,
									selectedColor, 1, &lastDrawingData, pixel_buffer);
								
								//lastCursorColor = selectedColor;
								//alt_up_pixel_buffer_dma_draw(pixel_buffer, lastCursorColor, currentCursor.x, currentCursor.y);
								lastDrawingData.firstErase = 1;
								lastLeft = 0;
								drawCursor = 1;
								//draw_icon(currentTool, 1, &lastDrawingData, pixel_buffer);
							}
							
						}
						else if (currentTool == EMPTY_ELLIPSE) {
							//Ellipse
							if (startUsingTool == 1) {
								startUsingTool = 0;
								secondPoint.x = currentCursor.x;
								secondPoint.y = currentCursor.y;
								lastCursorColor = get_pixel_color(secondPoint.x,secondPoint.y);
								printf("second point at: X:%d Y:%d\n\r", currentCursor.x, currentCursor.y);
								draw_empty_ellipse(firstPoint.x, firstPoint.y,
									currentCursor.x - firstPoint.x, currentCursor.y - firstPoint.y,
									selectedColor, pixel_buffer, 1, &lastDrawingData);
								draw_empty_ellipse(firstPoint.x, firstPoint.y,
									currentCursor.x - firstPoint.x, currentCursor.y - firstPoint.y,
									selectedColor, pixel_buffer, 0, &lastDrawingData);
								cursor_save(&currentCursor, cursorMem);
								//	soft_empty_rectangle_draw(firstPoint.x, firstPoint.y,
								//	secondPoint.x, secondPoint.y,
								//	DRAW_COLOR, 0, &lastDrawingData, pixel_buffer);

								//draw_icon(currentTool, 1, &lastDrawingData, pixel_buffer);
								lastDrawingData.firstErase = 1;
								lastLeft = 0;
							}
							
						}
						else if (currentTool == LINE){
							//draw final line
							if (startUsingTool == 1) {
								startUsingTool = 0;
								secondPoint.x = currentCursor.x;
								secondPoint.y = currentCursor.y;
								lastCursorColor = get_pixel_color(secondPoint.x,secondPoint.y);
								soft_draw_line(firstPoint.x,firstPoint.y,secondPoint.x,secondPoint.y,selectedColor,1,&lastDrawingData,pixel_buffer);
								lastDrawingData.firstErase = 1;
								lastLeft = 0;
							}
						}
						else if (currentTool == FILLED_ELLIPSE) {
							if (startUsingTool == 1) {
								startUsingTool = 0;
								secondPoint.x = currentCursor.x;
								secondPoint.y = currentCursor.y;
								lastCursorColor = get_pixel_color(secondPoint.x, secondPoint.y);
								printf("second point at: X:%d Y:%d\n\r", currentCursor.x, currentCursor.y);
								draw_empty_ellipse(firstPoint.x, firstPoint.y,
									currentCursor.x - firstPoint.x, currentCursor.y - firstPoint.y,
									selectedColor, pixel_buffer, 1, &lastDrawingData);
								if (draw_empty_ellipse(firstPoint.x, firstPoint.y,
									currentCursor.x - firstPoint.x, currentCursor.y - firstPoint.y,
									selectedColor, pixel_buffer, 0, &lastDrawingData)) {
								cursor_save(&currentCursor, cursorMem);
								/* draw_empty_ellipse(firstPoint.x, firstPoint.y,
									currentCursor.x - firstPoint.x-1, currentCursor.y - firstPoint.y-1,
									selectedColor, pixel_buffer, 0, &lastDrawingData);
								draw_empty_ellipse(firstPoint.x, firstPoint.y,
									currentCursor.x - firstPoint.x-2, currentCursor.y - firstPoint.y-2,
									selectedColor, pixel_buffer, 0, &lastDrawingData); */
									//	soft_empty_rectangle_draw(firstPoint.x, firstPoint.y,
									//	secondPoint.x, secondPoint.y,
									//	DRAW_COLOR, 0, &lastDrawingData, pixel_buffer);
									fill_to_edge_zone(firstPoint.x, firstPoint.y, selectedColor, pixel_buffer);
								}
								//draw_icon(currentTool, 1, &lastDrawingData, pixel_buffer);
								lastDrawingData.firstErase = 1;
								lastLeft = 0;
							}

						}
						else if (currentTool == FILL) {
							startUsingTool = 0;
							lastLeft = 0;
							//clear the cursor and launch the fill operation
							//alt_up_pixel_buffer_dma_draw(pixel_buffer, lastCursorColor, firstPoint.x, firstPoint.y);
							cursor_erase(&firstPoint, cursorMem,pixel_buffer);
							flood_fill_zone(firstPoint.x, firstPoint.y, selectedColor, pixel_buffer);
							cursor_save(&firstPoint, cursorMem);
							//update the last cursor color beacause of the fill
							lastCursorColor = selectedColor;
						}
						if (currentTool == CLEAR) {
							lastLeft = 0;
							startUsingTool = 0;
							alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 62, 0, 640, 480, BACKGROUD_COLOR, 0);
							//draw_tool_bar(currentTool, &lastDrawingData, pixel_buffer);
							//draw_icon(currentTool, 1, &lastDrawingData, pixel_buffer);
						}
						draw_tool_bar(currentTool, &lastDrawingData, pixel_buffer);
						draw_color_palette(selectedColor, &lastDrawingData, pixel_buffer);
						//lastLeft = 0;
					}
					if(currentTool == CPY_PASTE || currentTool == CUT_PASTE){
						if(cpyRngSelected){
							//cursor_erase(&currentCursor, &cursorMem, pixel_buffer);
							printf("entered condition \n\r");
							int rngX = currentCursor.x+(secondPoint.x-firstPoint.x);
							int rngY = currentCursor.y+(secondPoint.y-firstPoint.y);
							printf("x: %d y: %d\n\r", rngX,rngY);
							soft_empty_rectangle_draw(currentCursor.x, currentCursor.y, rngX, rngY,
							BLACK, 1, &lastDrawingData, pixel_buffer);
						}	
					}
					//lastLeft = 0;
					lastRight = 0;
				}
			}



			
			if(drawCursor){
				//draw the cursor on top of everithing (last drawing operation)
				cursor_draw(startUsingTool, &currentCursor,cursorMem, &x_pos, &y_pos, pixel_buffer);
			}
			// vertical refresh
			alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
		}

	}
	return 0;
#endif
#ifdef NIOS_DRAW_FUNC
	//nios_draw(pixel_buffer);
#endif // NIOS_DRAW_FUNC
}
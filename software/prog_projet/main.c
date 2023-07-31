/*
 * main.c
 * nouvelle valeur timeout ps2 0x001fffff
 *  Created on: Jun 12, 2023
 *      Author: ghikf et robin
 */
/* C Library */
#include <stdio.h>		// printf()
#include <string.h> 	// sprintf()


/* Altera-specific */
#include "alt_types.h"
#include "system.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "alt_types.h"
#include "sys/alt_irq.h"

# include "sys/alt_stdio.h"
/*Module Hardware*/
#include "recfiller.h"
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



context_t timer_context;
alt_up_ps2_dev* tim;
void timer_write_period(alt_u32 timerBase, alt_u32 period)
{
	/**************************************************************************
	 * start the timer in continuous mode with interrupts
	 **************************************************************************
	 * Parametres
	 * timerBase: Base address of timer
	 * period: desired period in ms
	 *
	 * Return
	 * None
	 *
	 * Side effects
	 * None
	 *
	 *************************************************************************/
	period = period * TIMER_CONVERSION;
	alt_u16 high, low;
	/* split 32 bits period into two 16 bits */
	high = (alt_u16)(period >> 16);
	low = (alt_u16)(period & 0x0000FFFF);
	/* write period */
	IOWR(timerBase, TIMER_PERIODH_REG_OFT, high);
	IOWR(timerBase, TIMER_PERIODL_REG_OFT, low);
	/* setup timer for start in continuous mode w/ interrupt */
	IOWR(timerBase, TIMER_CTRL_REG_OFT, 0x0007); // bits 0, 1, 2 actives
}
static void timer_0_ISR(void* context, alt_u32 id) 
//void timer_0_ISR(void* context)
{
	/**************************************************************************
	 * Interrupt handler for timer 0, flashes the LEDs at the timer period
	 **************************************************************************
	 * Parametres
	 * *context: conext of ISR
	 *
	 * Return
	 * None
	 *
	 * Side effects
	 * None
	 *
	 *************************************************************************/
	//tim = alt_up_ps2_open_dev(TIMER_0_NAME);		//Connexion au module PS/2
	context_t* ctxt = context;					//Structure qui contient le contexte pour
												//communiquer avec l'interruption
	 

	///static alt_u8 ledPattern = 0x01; // intial template

	
}

void start_timer(alt_u32 timerBase)
{
	/**************************************************************************
	 * start the timer in continuous mode with interrupts
	 **************************************************************************
	 * Parametres
	 * timerBase: Base address of timer
	 *
	 * Return
	 * None
	 *
	 * Side effects
	 * None
	 *
	 *************************************************************************/

	IOWR(timerBase, TIMER_CTRL_REG_OFT, 0b0111);
}

void stop_timer(alt_u32 timerBase)
{
	/**************************************************************************
	 * Stop the timer
	 **************************************************************************
	 * Parametres
	 * timerBase: Base address of timer
	 *
	 * Return
	 * None
	 *
	 * Side effects
	 * turns off the interrupt,
	 * turns off continuous
	 *
	 *************************************************************************/

	IOWR(timerBase, TIMER_CTRL_REG_OFT, 0b1000);
}






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
	stop_timer(TIMER_0_BASE);
	//timer_write_period(TIMER_0_BASE, period);
	//start_timer(TIMER_0_BASE);
	//alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ, timer_0_ISR, 0x0, 0x0);
	//if (alt_irq_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, 0x0, timer_0_ISR) == 0){
	//	alt_putstr("ISR REGISTERED\n\r");
	//}
	//alt_irq_register(PS2_0_IRQ, (void*)(&ps2_context), ps2_isr);
	alt_irq_register(TIMER_0_IRQ, (void*)(&timer_context), (void*)timer_0_ISR);
	timer_write_period(TIMER_0_BASE, period);
	//start_timer(TIMER_0_BASE);
	//Stop timer and setup the interrupt, then start with 100ms period (default)
	stop_timer(TIMER_0_BASE);
	//init recfiller
	recfiller_init(640, 480);
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
	//recfiller_draw_rectangle(0, 0, 40, 60, 0);
	
	//TBD probablu useless
	firstPoint.x = 100;
	firstPoint.y = 100;
	secondPoint.x = 110;
	secondPoint.y = 110;
	
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
	nios_draw(pixel_buffer);
#endif // NIOS_DRAW_FUNC
}
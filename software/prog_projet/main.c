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

/*Module software*/
#include "softwareDraw.h"
/* Mouse driver */
#include "ps2_mouse.h"
#include "io.h"
#define TIMER_CONVERSION 50000
#define TIMER_STAT_REG_OFT		0 	// status register
#define TIMER_CTRL_REG_OFT		1 	// control register
#define TIMER_PERIODL_REG_OFT 	2 	// period register, bits 15:0
#define TIMER_PERIODH_REG_OFT 	3 	// period register, bits 31:16



typedef struct Cursor{
	int x;
	int y;
} Cursor;

typedef struct Point {
	int x;
	int y;
} Point;
//Screen border limts
#define TOP_LIMIT 10
#define LEFT_LIMIT 0
#define RIGHT_LIMIT 639
#define BOTTOM_LIMIT 479
#define SCALE_FACTOR 0.1
#define SCALE_FACTOR_INV 10
//Colors
#define BACKGROUD_COLOR 128//0xEB
#define DRAW_COLOR 0
#define CURSOR_COLOR 0xFF
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

unsigned char get_pixel_color(int x, int y){
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
	unsigned char *pixel;
	unsigned char color;
	//pixel = (unsigned char *)ONCHIP_MEM_BASE+(y<<9)+x;
	pixel = (unsigned char *)ONCHIP_MEM_BASE +(y*640)+x;
	color = *pixel;
	return color;
}
void process_cursor_pos(Cursor *currentCursor, int *x_pos, int *y_pos ) {
	if (*x_pos > RIGHT_LIMIT * SCALE_FACTOR_INV) {
		currentCursor->x = RIGHT_LIMIT;
		*x_pos = RIGHT_LIMIT * SCALE_FACTOR_INV;
	}
	else if (*x_pos < LEFT_LIMIT * SCALE_FACTOR_INV) {
		currentCursor->x = LEFT_LIMIT;
		*x_pos = LEFT_LIMIT * SCALE_FACTOR_INV;
	}
	else {
		currentCursor->x = *x_pos * SCALE_FACTOR;
	}
	//printf("4\n");
	if (*y_pos > BOTTOM_LIMIT * SCALE_FACTOR_INV) {
		currentCursor->y = BOTTOM_LIMIT;
		*y_pos = BOTTOM_LIMIT * SCALE_FACTOR_INV;
	}
	else if (*y_pos < TOP_LIMIT * SCALE_FACTOR_INV) {
		currentCursor->y = TOP_LIMIT;
		*y_pos = TOP_LIMIT * SCALE_FACTOR_INV;
	}
	else {
		currentCursor->y = *y_pos * SCALE_FACTOR;
	}
}
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

	char pos_msg[100];
	int lastRight = 0;
	int lastLeft = 0;
	int lastColor = 0;
	char startUsingTool=0;
	Point firstPoint, secondPoint;
	Cursor currentCursor;
	currentCursor.x = 0;
	currentCursor.y = 0;

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
	

	/* CHAR BUFFER setup and static display */
	//alt_up_char_buffer_dev *char_buffer;
	//char_buffer = alt_up_char_buffer_open_dev("/dev/character_buffer_with_dma");
	/*if (char_buffer == NULL) {
		printf("char buff is dead\n\r");
	}else{
		printf("char buff ok\n\r");
	}*/
	//alt_up_char_buffer_init(char_buffer);
	//sprintf(pos_msg, "NiosDraw 1.42.69 - Nicolas Gagnier - Robin Galipeau");
	//alt_up_char_buffer_string(char_buffer, pos_msg, 0,5);

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
	firstPoint.x = 100;
	firstPoint.y = 100;
	secondPoint.x = 110;
	secondPoint.y = 110;
	//soft_emptyRect_draw(firstPoint.x, firstPoint.y,
	//					secondPoint.x, secondPoint.y,
	//					DRAW_COLOR, 0, &lastDrawingData, pixel_buffer);
	//printf("recfill %d\n", recfiller_draw_rectangle(1, 1, 10, 10, 44));
	/* main loop */
	
	while (1) {

		// process ps2 events during vertical blank
		if (!alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer)) {
			//printf("1\n");
			//ledPattern ^= 0x03; // inverse 2 LSB
			//IOWR(LEDS_BASE, 0, ledPattern); // Write template to LEDs
			// process PS2 events
			if (ps2_process(&left_btn, &right_btn, &x_mov, &y_mov)) {
				x_pos += x_mov;
				y_pos -= y_mov;
				//printf("2\n");
			}
			//recfiller_draw_rectangle(0, 0, 40, 60, 0);
            
			/* Manage cursor */
			if (startUsingTool == 0) {
				//erase old cursor
				alt_up_pixel_buffer_dma_draw(pixel_buffer, lastColor, currentCursor.x, currentCursor.y);
				//printf("3\n");
				//Apply scaling and verify cursor is within the boundarys of the screen
				process_cursor_pos(&currentCursor, &x_pos, &y_pos);
				/*if (x_pos > RIGHT_LIMIT * SCALE_FACTOR_INV) {
					currentCursor.x = RIGHT_LIMIT;
					x_pos = RIGHT_LIMIT*SCALE_FACTOR_INV;
				}else if (x_pos < LEFT_LIMIT*SCALE_FACTOR_INV){
					currentCursor.x = LEFT_LIMIT;
					x_pos = LEFT_LIMIT*SCALE_FACTOR_INV;
				}else{
					currentCursor.x = x_pos*SCALE_FACTOR;
				}
				//printf("4\n");
				if(y_pos> BOTTOM_LIMIT*SCALE_FACTOR_INV){
					currentCursor.y = BOTTOM_LIMIT;
					y_pos = BOTTOM_LIMIT*SCALE_FACTOR_INV;
				}else if(y_pos < TOP_LIMIT*SCALE_FACTOR_INV){
					currentCursor.y = TOP_LIMIT;
					y_pos = TOP_LIMIT*SCALE_FACTOR_INV;
				}else{
					currentCursor.y = y_pos*SCALE_FACTOR;
				}*/
				//Save the last cursor pixel color for next turn in the loop
				lastColor = get_pixel_color(currentCursor.x, currentCursor.y);
				//Draw cursor
				alt_up_pixel_buffer_dma_draw(pixel_buffer, CURSOR_COLOR, currentCursor.x, currentCursor.y);
			}
			else {
				process_cursor_pos(&currentCursor, &x_pos, &y_pos);
			}
			/* process clicks */
			if (left_btn){ //Draw during left click
				/*alt_up_pixel_buffer_dma_draw(pixel_buffer, DRAW_COLOR, currentCursor.x, currentCursor.y);
				lastColor = DRAW_COLOR;
				if(!lastLeft){
					alt_putstr("left clik, DRAWING\n\r");
					printf("Drawing at: X:%d Y:%d\n\r",currentCursor.x, currentCursor.y);
				}else{
					printf("Drawing at: X:%d Y:%d\n\r",currentCursor.x, currentCursor.y);
				}*/
				if (startUsingTool == 0) {
					printf("first point at: X:%d Y:%d\n\r", currentCursor.x, currentCursor.y);
					firstPoint.x = currentCursor.x;
					firstPoint.y = currentCursor.y;
					startUsingTool = 1;
					lastLeft = 1;
				}
				lastLeft = 1;
				//soft_emptyRect_draw(firstPoint.x, firstPoint.y,
				//		currentCursor.x, currentCursor.y,
				//		DRAW_COLOR, 1, &lastDrawingData, pixel_buffer);

			}else if(right_btn){ //erase whole screen if right click
				alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0,0,640,480,BACKGROUD_COLOR,0);
				if(!lastRight){
					printf("right click (X:%d,Y:%d), CLEAR SCREEN\n\r",currentCursor.x, currentCursor.y);
				}
				lastRight = 1;
			}
			else{ //clear click flags
				if(lastLeft){
					alt_putstr("left released, STOP DRAWING\n\r");
					if (startUsingTool == 1) {
						startUsingTool = 0;
						secondPoint.x = currentCursor.x;
						secondPoint.y = currentCursor.y;
						printf("second point at: X:%d Y:%d\n\r", currentCursor.x, currentCursor.y);
						//soft_emptyRect_draw(firstPoint.x, firstPoint.y,
						//	secondPoint.x, secondPoint.y,
						//	DRAW_COLOR, 0, &lastDrawingData, pixel_buffer);
					}
				}
				lastLeft = 0;
				lastRight = 0;
			}
			draw_empty_ellipse(200, 200, 60, 100,DRAW_COLOR, pixel_buffer);
			// send new position to char buff
            //sprintf(pos_msg, "X:%d Y:%d  ", currentCursor.x, currentCursor.y);
            //alt_up_char_buffer_string(char_buffer, pos_msg, 60,59);

			// vertical refresh
			alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
		}
	}
	return 0;
}
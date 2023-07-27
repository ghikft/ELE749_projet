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
/* Mouse driver */
#include "ps2_mouse.h"
#include "io.h"
#define TIMER_CONVERSION 50000
#define TIMER_STAT_REG_OFT		0 	// status register
#define TIMER_CTRL_REG_OFT		1 	// control register
#define TIMER_PERIODL_REG_OFT 	2 	// period register, bits 15:0
#define TIMER_PERIODH_REG_OFT 	3 	// period register, bits 31:16

typedef enum mode {
	DRAWING_MODE,
	SNAKE_MODE,
	PONG_MODE
}mode;
typedef enum tool {
	NO_TOOL,
	PENCIL,
	EMPTY_RECTANGLE,
	FILLED_RECTANGLE,
	EMPTY_ELLIPSE,
	FILLED_ELLIPSE,
	LINE,
	CPY_PASTE,
	FILL,
	CUT_PASTE,
	COLOR_SAMPLE,
	PONG,
	SNAKE,
	COLOR_SELECTION,
	CLEAR
}tool;

typedef struct Cursor{
	int x;
	int y;
} Cursor;

typedef struct Point {
	int x;
	int y;
} Point;
//Screen border limts
#define TOP_LIMIT 0
#define LEFT_LIMIT 0
#define RIGHT_LIMIT 639
#define BOTTOM_LIMIT 479
#define DRAWING_ZONE_LEFT_LIMIT 61

#define SCALE_FACTOR 0.1
#define SCALE_FACTOR_INV 10
//Colors
#define BACKGROUD_COLOR WHITE//128//0xEB
#define DRAW_COLOR 0
#define CURSOR_COLOR BLACK
#define TOOL_BOX_BACKGROUND_COLOR 0b11011111
#define SELECTION_COLOR 128

//#define BLACK 0
#define ERASE_PREVIOUS_WORK 1
#define NOT_ERASE_PREVIOUS_WORK 0

#define ICON_LEFT_X 2
#define ICON_TOP_Y 2
#define ICON_WIDTH 26
#define ICON_HEIGHT 26
#define ICON_SPACING 2

#define IN_TOOL_BAR 1
#define NOT_IN_TOOL_BAR 0

#define TOOL_SELECTED 1
#define TOOL_NOT_SELECTED 0

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

void draw_color_palette(int selectedColor, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	
	//draw the selected color
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 3, 147, SECOND_COLUMN_X_END, 174, selectedColor, 0);
	//draw the color palette
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, FIRST_COLUMN_X_START, 176, FIRST_COLUMN_X_END, 203, BLACK, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, SECOND_COLUMN_X_START, 176, SECOND_COLUMN_X_END, 203, WHITE, 0);
	
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, FIRST_COLUMN_X_START, 205, FIRST_COLUMN_X_END, 232, LILLA, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, SECOND_COLUMN_X_START, 205, SECOND_COLUMN_X_END, 232, PURPLE, 0);

	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, FIRST_COLUMN_X_START, 234, FIRST_COLUMN_X_END, 261, RED, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, SECOND_COLUMN_X_START, 234, SECOND_COLUMN_X_END, 261, BROWN, 0);

	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, FIRST_COLUMN_X_START, 263, FIRST_COLUMN_X_END, 290, SKY_BLUE, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, SECOND_COLUMN_X_START, 263, SECOND_COLUMN_X_END, 290, BLUE, 0);

	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, FIRST_COLUMN_X_START, 292, FIRST_COLUMN_X_END, 319, LIGHT_PINK, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, SECOND_COLUMN_X_START, 292, SECOND_COLUMN_X_END, 319, PINK, 0);

	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, FIRST_COLUMN_X_START, 321, FIRST_COLUMN_X_END, 348, YELLOW, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, SECOND_COLUMN_X_START, 321, SECOND_COLUMN_X_END, 348, ORANGE, 0);

	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, FIRST_COLUMN_X_START, 350, FIRST_COLUMN_X_END, 377, DARK_GREEN, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, SECOND_COLUMN_X_START, 350, SECOND_COLUMN_X_END, 377, NEON_GREEN, 0);

	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, FIRST_COLUMN_X_START, 379, FIRST_COLUMN_X_END, 406, FLUO_GREEN, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, SECOND_COLUMN_X_START, 379, SECOND_COLUMN_X_END, 406, DARK_BLUE, 0);

	//draw frame around the current color
	soft_emptyRect_draw(3, 147, SECOND_COLUMN_X_END, 174, NOT_ERASE_PREVIOUS_WORK, BLACK, lastDrawingData, pixel_buffer);
}
void draw_selection_Frame(int x1, int y1, int x2, int y2, char selected,
	lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	//draw selection perimiter
	if (selected) {
		soft_emptyRect_draw(x1, y1, x2, y2, SELECTION_COLOR, NOT_ERASE_PREVIOUS_WORK, lastDrawingData, pixel_buffer);
		soft_emptyRect_draw(x1+1, y1+1, x2-1, y2-1, SELECTION_COLOR, NOT_ERASE_PREVIOUS_WORK, lastDrawingData, pixel_buffer);
	}
	else {
		soft_emptyRect_draw(x1, y1, x2, y2, TOOL_BOX_BACKGROUND_COLOR, NOT_ERASE_PREVIOUS_WORK, lastDrawingData, pixel_buffer);
		soft_emptyRect_draw(x1 + 1, y1 + 1, x2 -1, y2 -1, BLACK, NOT_ERASE_PREVIOUS_WORK, lastDrawingData, pixel_buffer);
	}
}
void draw_icon(tool icon, char selected,
	lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer) {

	switch (icon) {
	case EMPTY_RECTANGLE:
		draw_selection_Frame(2, 2, 29, 29, selected, lastDrawingData, pixel_buffer);
		//draw icon
		soft_emptyRect_draw(6, 11, 25, 20, BLACK, NOT_ERASE_PREVIOUS_WORK, lastDrawingData, pixel_buffer);
		break;
	case FILLED_RECTANGLE:
		draw_selection_Frame(31, 2, 58, 29, selected, lastDrawingData, pixel_buffer);
		//draw icon
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer,35 ,11, 55, 20, BLACK, 0);
		break;
	case EMPTY_ELLIPSE:
		//draw selection perimiter
		draw_selection_Frame(2, 31, 29, 58, selected, lastDrawingData, pixel_buffer);
		//draw icon
		draw_empty_ellipse(16, 45, 8, 4, BLACK, pixel_buffer, NOT_ERASE_PREVIOUS_WORK, lastDrawingData);
		break;
	case FILLED_ELLIPSE:
		//draw selection perimiter
		draw_selection_Frame(31, 31, 58, 58, selected, lastDrawingData, pixel_buffer);
		//draw icon
		draw_empty_ellipse(45, 45, 8, 4, BLACK, pixel_buffer, NOT_ERASE_PREVIOUS_WORK, lastDrawingData);
		break;
	case LINE:
		//draw selection perimiter
		draw_selection_Frame(2, 60, 29, 87, selected, lastDrawingData, pixel_buffer);
		//draw icon
		draw_empty_ellipse(16, 74, 8, 4, BLACK, pixel_buffer, NOT_ERASE_PREVIOUS_WORK, lastDrawingData);
		break;	
	case FILL:
		//draw selection perimiter
		draw_selection_Frame(31, 60, 58, 87, selected, lastDrawingData, pixel_buffer);
		//draw icon
		draw_empty_ellipse(45, 74, 8, 4, BLACK, pixel_buffer, NOT_ERASE_PREVIOUS_WORK, lastDrawingData);
		break;
	case COLOR_SAMPLE:
		//draw selection perimiter
		draw_selection_Frame(2, 89, 29, 116, selected, lastDrawingData, pixel_buffer);
		//draw icon
		draw_empty_ellipse(16, 103, 8, 4, BLACK, pixel_buffer, NOT_ERASE_PREVIOUS_WORK, lastDrawingData);
		break;
	case CPY_PASTE:
		//draw selection perimiter
		draw_selection_Frame(31, 89, 58, 116, selected, lastDrawingData, pixel_buffer);
		//draw icon
		draw_empty_ellipse(45, 103, 8, 4, BLACK, pixel_buffer, NOT_ERASE_PREVIOUS_WORK, lastDrawingData);
		break;	
	case CUT_PASTE:
		//draw selection perimiter
		draw_selection_Frame(2, 118, 29, 145, selected, lastDrawingData, pixel_buffer);
		//draw icon
		draw_empty_ellipse(16, 132, 8, 4, BLACK, pixel_buffer, NOT_ERASE_PREVIOUS_WORK, lastDrawingData);
		break;
	case PENCIL:
		//draw selection perimiter
		draw_selection_Frame(31, 118, 58, 145, selected, lastDrawingData, pixel_buffer);
		//draw icon
		draw_empty_ellipse(45, 132, 8, 4, BLACK, pixel_buffer, NOT_ERASE_PREVIOUS_WORK, lastDrawingData);
		break;	
	case PONG:
		//draw selection perimiter
		draw_selection_Frame(2, 408, 29, 435, selected, lastDrawingData, pixel_buffer);
		//draw icon
		draw_empty_ellipse(16, 422, 8, 4, BLACK, pixel_buffer, NOT_ERASE_PREVIOUS_WORK, lastDrawingData);
		break;
	case SNAKE:
		//draw selection perimiter
		draw_selection_Frame(31, 408, 58, 435, selected, lastDrawingData, pixel_buffer);
		//draw icon
		draw_empty_ellipse(45, 422, 8, 4, BLACK, pixel_buffer, NOT_ERASE_PREVIOUS_WORK, lastDrawingData);
		break;
	case CLEAR:
		//draw selection perimiter
		draw_selection_Frame(2, 437, 29, 464, selected, lastDrawingData, pixel_buffer);
		//draw icon
		draw_empty_ellipse(16, 451, 8, 4, BLACK, pixel_buffer, NOT_ERASE_PREVIOUS_WORK, lastDrawingData);
		break;
	default:
		break;
	}
	

}
void draw_tool_bar(tool currentTool, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	//draw tool bar background
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, LEFT_LIMIT, TOP_LIMIT, DRAWING_ZONE_LEFT_LIMIT, BOTTOM_LIMIT, TOOL_BOX_BACKGROUND_COLOR, 0);
	//Draw frame
	soft_emptyRect_draw(LEFT_LIMIT, TOP_LIMIT, DRAWING_ZONE_LEFT_LIMIT, BOTTOM_LIMIT,NOT_ERASE_PREVIOUS_WORK, BLACK, lastDrawingData, pixel_buffer);
	soft_emptyRect_draw(LEFT_LIMIT+1, TOP_LIMIT+1, DRAWING_ZONE_LEFT_LIMIT-1, BOTTOM_LIMIT-1, NOT_ERASE_PREVIOUS_WORK, BLACK, lastDrawingData, pixel_buffer);
	draw_icon(EMPTY_RECTANGLE, 0, lastDrawingData, pixel_buffer);
	draw_icon(FILLED_RECTANGLE, 0, lastDrawingData, pixel_buffer);
	draw_icon(EMPTY_ELLIPSE, 0, lastDrawingData, pixel_buffer);
	draw_icon(FILLED_ELLIPSE, 0, lastDrawingData, pixel_buffer);
	draw_icon(LINE, 0, lastDrawingData, pixel_buffer);
	draw_icon(FILL, 0, lastDrawingData, pixel_buffer);
	draw_icon(COLOR_SAMPLE, 0, lastDrawingData, pixel_buffer);
	draw_icon(CPY_PASTE, 0, lastDrawingData, pixel_buffer);
	draw_icon(CUT_PASTE, 0, lastDrawingData, pixel_buffer);
	draw_icon(PENCIL, 0, lastDrawingData, pixel_buffer);
	draw_icon(PONG, 0, lastDrawingData, pixel_buffer);
	draw_icon(SNAKE, 0, lastDrawingData, pixel_buffer);
	draw_icon(CLEAR, 0, lastDrawingData, pixel_buffer);
	//draw_selection_Frame(2, 147, 29, 174, 0, lastDrawingData, pixel_buffer);//current color
	//draw_selection_Frame(2, 176, 29, 203, 0, lastDrawingData, pixel_buffer);
	//draw_selection_Frame(2, 205, 29, 232, 0, lastDrawingData, pixel_buffer);
	//draw_selection_Frame(2, 234, 29, 261, 0, lastDrawingData, pixel_buffer);
	//draw_selection_Frame(2, 263, 29, 290, 0, lastDrawingData, pixel_buffer);
	//draw_selection_Frame(2, 292, 29, 319, 0, lastDrawingData, pixel_buffer);
	//draw_selection_Frame(2, 321, 29, 348, 0, lastDrawingData, pixel_buffer);
	//draw_selection_Frame(2, 350, 29, 377, 0, lastDrawingData, pixel_buffer);
	//draw_selection_Frame(2, 379, 29, 406, 0, lastDrawingData, pixel_buffer);
	//draw_selection_Frame(2, 408, 29, 435, 0, lastDrawingData, pixel_buffer);//hauteur pour snake et pong
	//draw_selection_Frame(2, 437, 29, 464, 0, lastDrawingData, pixel_buffer);//position clear

	//Draw the current selected tool
	draw_icon(currentTool, 1, lastDrawingData, pixel_buffer);
}

void toolSelection(Cursor* currentCursor, tool* currentTool, tool* lastTool, int startUsingTool, 
	int* selectedColor,char left_btn, lastDrawingVar *lastDrawingData,
	alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	static int lastColor = BLACK;
	//inside the tool bar zone and no tool currently in use
	if (startUsingTool == 0 && currentCursor->x < DRAWING_ZONE_LEFT_LIMIT && left_btn) {
		//in first or second column of tool
		if (currentCursor->x <= 29 /* && left_btn*/) {
			if (currentCursor->y < 29) *currentTool = EMPTY_RECTANGLE;
			else if (currentCursor->y < 58) *currentTool = EMPTY_ELLIPSE;
			else if (currentCursor->y < 87) *currentTool = LINE;
			else if (currentCursor->y < 116) *currentTool = COLOR_SAMPLE;
			else if (currentCursor->y < 145) *currentTool = CUT_PASTE;
			//color selection
			else if (currentCursor->y < 203 && currentCursor->y > 176) *selectedColor = BLACK;
			else if (currentCursor->y < 232) *selectedColor = LILLA;
			else if (currentCursor->y < 261) *selectedColor = RED;
			else if (currentCursor->y < 290) *selectedColor = SKY_BLUE;
			else if (currentCursor->y < 319) *selectedColor = LIGHT_PINK;
			else if (currentCursor->y < 348) *selectedColor = YELLOW;
			else if (currentCursor->y < 377) *selectedColor = DARK_GREEN;
			else if (currentCursor->y < 406) *selectedColor = FLUO_GREEN;
			//game and clear selection
			else if (currentCursor->y < 435 && currentCursor->y > 408) *currentTool = PONG;
			else if (currentCursor->y < 464 && currentCursor->y > 437) *currentTool = CLEAR;

			

		}
		//second column
		else if (currentCursor->x >= 32 /* && left_btn*/) {
			if (currentCursor->y < 29) *currentTool = FILLED_RECTANGLE;
			else if (currentCursor->y < 58) *currentTool = FILLED_ELLIPSE;
			else if (currentCursor->y < 87) *currentTool = FILL;
			else if (currentCursor->y < 116) *currentTool = CPY_PASTE;
			else if (currentCursor->y < 145) *currentTool = PENCIL;
			//color selection
			else if (currentCursor->y < 203 && currentCursor->y > 176) *selectedColor = WHITE;
			else if (currentCursor->y < 232) *selectedColor = PURPLE;
			else if (currentCursor->y < 261) *selectedColor = BROWN;
			else if (currentCursor->y < 290) *selectedColor = BLUE;
			else if (currentCursor->y < 319) *selectedColor = PINK;
			else if (currentCursor->y < 348) *selectedColor = ORANGE;
			else if (currentCursor->y < 377) *selectedColor = NEON_GREEN;
			else if (currentCursor->y < 406) *selectedColor = DARK_BLUE;
			//game selection
			else if (currentCursor->y < 435 && currentCursor->y > 408) *currentTool = SNAKE;
		}
		//clear old selection and select the new tool
		if (*lastTool != *currentTool) {
			draw_icon(*lastTool, TOOL_NOT_SELECTED, lastDrawingData, pixel_buffer);
			*lastTool = *currentTool;
			draw_icon(*currentTool, TOOL_SELECTED, lastDrawingData, pixel_buffer);
		}
		if (lastColor != *selectedColor) {
			lastColor = *selectedColor;
			draw_color_palette(*selectedColor, lastDrawingData, pixel_buffer);
		}		
	}
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

void start_button(tool currentTool, char* startButtonPressed,unsigned char* left_btn, unsigned char* lastLeft, int* lastCursorColor,
	Cursor* currentCursor, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 290, 220, 350, 260, 16, 0);
	//if cursor over the button
	if (*left_btn) {
		//if cursor over the start button while left clic register the clic
		if (currentCursor->x > 290 && currentCursor->x < 350) {
			if (currentCursor->y > 220 && currentCursor->y < 260) {
				*lastLeft = 1;				
			}
		}
	}
	else//when left button release, change flag to indicate start has been pressed then draw the drawing app UI
	{
		if (*lastLeft) {			
			*startButtonPressed = 1;
			*lastLeft = 0;
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 62, 0, 640, 480, BACKGROUD_COLOR, 0);
			draw_tool_bar(currentTool, lastDrawingData, pixel_buffer);
			//alt_up_pixel_buffer_dma_draw(pixel_buffer, BACKGROUD_COLOR, currentCursor.x, currentCursor.y);
			*lastCursorColor = BACKGROUD_COLOR;
		}
	}
	
}

void cursor_draw(char startUsingTool, int *lastCursorColor, Cursor *currentCursor, int* x_pos, int* y_pos, alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	/* Manage cursor when not using tool*/
	if (startUsingTool == 0) {
		//erase old cursor
		alt_up_pixel_buffer_dma_draw(pixel_buffer, *lastCursorColor, currentCursor->x, currentCursor->y);

		//Apply scaling and verify cursor is within the boundarys of the screen
		process_cursor_pos(currentCursor, x_pos, y_pos);
		//Save the last cursor pixel color for next turn in the loop
		*lastCursorColor = get_pixel_color(currentCursor->x, currentCursor->y);
		//Draw cursor
		alt_up_pixel_buffer_dma_draw(pixel_buffer, CURSOR_COLOR, currentCursor->x, currentCursor->y);
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
	//char drawingColor = BLACK;
	char pos_msg[100];
	unsigned char lastRight = 0;
	unsigned char lastLeft = 0;
	int lastCursorColor = 0;
	char startUsingTool=0;
	Point firstPoint, secondPoint;
	Cursor currentCursor;
	tool currentTool = NO_TOOL;
	tool lastTool = NO_TOOL;
	currentCursor.x = 0;
	currentCursor.y = 0;
	mode programMode = DRAWING_MODE;
	char startButtonPressed = 0; 
	int selectedColor = BLACK;

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
	firstPoint.x = 100;
	firstPoint.y = 100;
	secondPoint.x = 110;
	secondPoint.y = 110;
	//soft_emptyRect_draw(firstPoint.x, firstPoint.y,
	//					secondPoint.x, secondPoint.y,
	//					DRAW_COLOR, 0, &lastDrawingData, pixel_buffer);
	//printf("recfill %d\n", recfiller_draw_rectangle(1, 1, 10, 10, 44));
	/* main loop */

	//initiate the drawing zone and tool bar
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 62, 0, 640, 480, BACKGROUD_COLOR, 0);
	draw_tool_bar(currentTool , &lastDrawingData, pixel_buffer);
	draw_color_palette(selectedColor, &lastDrawingData, pixel_buffer);
	//draw_icon(currentTool, 1, &lastDrawingData, pixel_buffer);
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
			/* Manage cursor when not using tool*/
			/*if (startUsingTool == 0) {
				//erase old cursor
				alt_up_pixel_buffer_dma_draw(pixel_buffer, lastCursorColor, currentCursor.x, currentCursor.y);
				
				//Apply scaling and verify cursor is within the boundarys of the screen
				process_cursor_pos(&currentCursor, &x_pos, &y_pos);
				//Save the last cursor pixel color for next turn in the loop
				lastCursorColor = get_pixel_color(currentCursor.x, currentCursor.y);
				//Draw cursor
				alt_up_pixel_buffer_dma_draw(pixel_buffer, CURSOR_COLOR, currentCursor.x, currentCursor.y);
			}
			else {//only get cursor position when using tool
				process_cursor_pos(&currentCursor, &x_pos, &y_pos);
			}*/

			//need to press the start button before being able to do anything else
			if (startButtonPressed == 0) {
				start_button(currentTool, &startButtonPressed,&left_btn, &lastLeft, &lastCursorColor, &currentCursor, &lastDrawingData, pixel_buffer);
				//draw_icon(currentTool, 1, &lastDrawingData, pixel_buffer);
				draw_color_palette(selectedColor, &lastDrawingData, pixel_buffer);
			}
			else {
				//Check for tool selection if not using a tool and cursor inside the tool bar
				
				toolSelection(&currentCursor, &currentTool, &lastTool, startUsingTool, &selectedColor, left_btn, &lastDrawingData, pixel_buffer);
				/* process clicks */
				//
				if (left_btn) { //Draw during left click
					//if (currentCursor.x > DRAWING_ZONE_LEFT_LIMIT) {//limit the usage of tool while in the 
					if (currentTool == PENCIL) {
						startUsingTool = 1;
						lastLeft = 1;
						alt_up_pixel_buffer_dma_draw(pixel_buffer, selectedColor, currentCursor.x, currentCursor.y);
						lastCursorColor = selectedColor;//was DRAW_COLOR
					}
					else if (currentTool == EMPTY_RECTANGLE) {
						//alt_up_pixel_buffer_dma_draw(pixel_buffer, selectedColor, currentCursor.x, currentCursor.y);
						//lastCursorColor = selectedColor;// DRAW_COLOR;
						/*if (!lastLeft) {
							alt_putstr("left clik, DRAWING\n\r");
							printf("Drawing at: X:%d Y:%d\n\r",currentCursor.x, currentCursor.y);
						}else{
							printf("Drawing at: X:%d Y:%d\n\r",currentCursor.x, currentCursor.y);
						}*/
						//rectangle
						if (startUsingTool == 0) {
							alt_up_pixel_buffer_dma_draw(pixel_buffer, lastCursorColor, currentCursor.x, currentCursor.y);
							printf("first point at: X:%d Y:%d\n\r", currentCursor.x, currentCursor.y);
							firstPoint.x = currentCursor.x;
							firstPoint.y = currentCursor.y;
							startUsingTool = 1;
							lastLeft = 1;
						}
						else {
							soft_emptyRect_draw(firstPoint.x, firstPoint.y,
								currentCursor.x, currentCursor.y,
								selectedColor, 1, &lastDrawingData, pixel_buffer);
						}
					}
					else if (currentTool == EMPTY_ELLIPSE) {		//elipse
						if (startUsingTool == 0) {
							alt_up_pixel_buffer_dma_draw(pixel_buffer, lastCursorColor, currentCursor.x, currentCursor.y);
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
					//}//clear in an exception it only activable when cursor is on the icon
					else if (currentTool == CLEAR) {
						if (currentCursor.x <= 29 && currentCursor.y < 464 && currentCursor.y > 437) {
							if (startUsingTool == 0) {
								startUsingTool = 1;
								lastLeft = 1;
							}
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
							startUsingTool = 0;
							lastLeft = 0;
							//draw_icon(currentTool, 1, &lastDrawingData, pixel_buffer);
						}
						else if(currentTool==EMPTY_RECTANGLE){
							if (startUsingTool == 1) {
								lastCursorColor = selectedColor;
								startUsingTool = 0;
								secondPoint.x = currentCursor.x;
								secondPoint.y = currentCursor.y;
								printf("second point at: X:%d Y:%d\n\r", currentCursor.x, currentCursor.y);
								soft_emptyRect_draw(firstPoint.x, firstPoint.y,
									secondPoint.x, secondPoint.y,
									selectedColor, 1, &lastDrawingData, pixel_buffer);
								soft_emptyRect_draw(firstPoint.x, firstPoint.y,
									secondPoint.x, secondPoint.y,
									selectedColor, 0, &lastDrawingData, pixel_buffer);
								lastDrawingData.firstErase = 1;
								lastLeft = 0;
								//draw_icon(currentTool, 1, &lastDrawingData, pixel_buffer);
							}
						}
						else if (currentTool == EMPTY_ELLIPSE) {
							//Ellipse
							if (startUsingTool == 1) {
								startUsingTool = 0;
								secondPoint.x = currentCursor.x;
								secondPoint.y = currentCursor.y;
								printf("second point at: X:%d Y:%d\n\r", currentCursor.x, currentCursor.y);
								/*draw_empty_ellipse(firstPoint.x, firstPoint.y,
									currentCursor.x - firstPoint.x, currentCursor.y - firstPoint.y,
									selectedColor, pixel_buffer, 1, &lastDrawingData);
								draw_empty_ellipse(firstPoint.x, firstPoint.y,
									currentCursor.x - firstPoint.x, currentCursor.y - firstPoint.y,
									selectedColor, pixel_buffer, 0, &lastDrawingData);*/
								//	soft_emptyRect_draw(firstPoint.x, firstPoint.y,
								//	secondPoint.x, secondPoint.y,
								//	DRAW_COLOR, 0, &lastDrawingData, pixel_buffer);

								//draw_icon(currentTool, 1, &lastDrawingData, pixel_buffer);
								lastDrawingData.firstErase = 1;
								lastLeft = 0;
							}
							
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
					
					//lastLeft = 0;
					lastRight = 0;
				}
			}
			

			//draw the cursor on top of everithing (last drawing operation)
			cursor_draw(startUsingTool, &lastCursorColor, &currentCursor, &x_pos, &y_pos, pixel_buffer);
			// vertical refresh
			alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
		}
	}
	return 0;
}
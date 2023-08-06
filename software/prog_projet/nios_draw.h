/* Description
* Library used to drive the nios draw application
* Autor: Nicolas & Robin
*********************************************************************/
#ifndef NIOS_DRAW_H_
#define NIOS_DRAW_H_
#include "project_variables.h"
#include "softwareDraw.h"


void draw_icon(tool icon, char selected,
	lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer);

void tool_selection(Cursor* currentCursor, tool* currentTool, int startUsingTool,
	int* selectedColor, char* left_btn, lastDrawingVar* lastDrawingData,
	alt_up_pixel_buffer_dma_dev* pixel_buffer);

void draw_tool_bar(tool currentTool, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer);

void draw_icon_array(int start_x, int start_y, int row, int col, int* image, alt_up_pixel_buffer_dma_dev* pixel_buffer);

void draw_selection_Frame(int x1, int y1, int x2, int y2, char selected,
	lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer);

void draw_color_palette(int selectedColor, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer);

void cursor_draw_sprite(Cursor* coordinate, alt_up_pixel_buffer_dma_dev* pixel_buffer);

void cursor_erase(Cursor* coordinate, alt_u8* cursorMem, alt_up_pixel_buffer_dma_dev* pixel_buffer);

void cursor_save(Cursor* coordinate, alt_u8* cursorMem);

unsigned char get_pixel_color(int x, int y);

void cursor_draw(char startUsingTool, Cursor* currentCursor, alt_u8* cursorMem, int* x_pos, int* y_pos, alt_up_pixel_buffer_dma_dev* pixel_buffer);

void start_button(tool currentTool, char* startButtonPressed, unsigned char* left_btn, unsigned char* lastLeft, alt_u8* cursorMem,
	Cursor* currentCursor, lastDrawingVar* lastDrawingData, alt_up_pixel_buffer_dma_dev* pixel_buffer);

void process_cursor_pos(Cursor* currentCursor, int* x_pos, int* y_pos);

void nios_draw(alt_up_pixel_buffer_dma_dev* pixel_buffer);

#endif /* NIOS_DRAW_H_ */
/******recfiller.h *********************************************************
*Description
* Librairie pour controler l'accelerateur de rectangles
*********************************************************************/
#ifndef PROJECT_VARIABLES_H
#define PROJECT_VARIABLES_H

//RRR GGG BB
#define PINK 0b11100011//
#define LIGHT_PINK 0b11101111//
#define RED	0b11100000	//
#define PURPLE 0b10000011 //
#define LILLA 0b01101011 //
#define BLUE 0b000000111//
#define SKY_BLUE 0b01011111//
#define DARK_BLUE 0b00000011
#define FLUO_GREEN 0b00011100
#define DARK_GREEN 0b00001000
#define NEON_GREEN 0b10011100
#define YELLOW 0b11111100//
#define ORANGE 0b11101000//
#define WHITE 0b11111111	//
#define BLACK 0b00000000	//
#define BROWN 0b10001000	//

#define FIRST_COLUMN_X_START 2
#define FIRST_COLUMN_X_END 29
#define SECOND_COLUMN_X_START 31
#define SECOND_COLUMN_X_END 58

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

typedef struct Cursor {
	int x;
	int y;
} Cursor;

typedef struct Point {
	int x;
	int y;
} Point;
#endif  // PROJECT_VARIABLES_H
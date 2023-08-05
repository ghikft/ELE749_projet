#ifndef PONG_H_
#define PONG_H_

#include "project_variables.h"
#include "softwareDraw.h"
#include <stdio.h>		// printf()
#include <string.h> 	// sprintf()
#include <time.h> 		// time(NULL)
#include <stdlib.h> 	// srand, rand
#include <stdint.h>
#include <time.h> 		// time(NULL)
#include "system.h"
#include "userIO.h"

#define TOP_BAR_HEIGHT 20
#define PLAY_AREA_WIDTH 640
#define PLAY_AREA_HEIGHT 480-TOP_BAR_HEIGHT
#define PADDLE_HEIGHT 9
#define SQUARE_WIDTH_PONG 10

#define PADDLE_COLOUR WHITE
#define BACKGROUD_COLOUR BLACK
#define BALL_COLOUR WHITE

#define SPEED_INCREASE 0.25
#define SPIN 0.1
#define INIT_X_SPEED 0.5
#define INIT_Y_SPEED 0

//directions
#define  UP     1
#define  DOWN   2
#define  NONE   0

void play_pong(alt_up_pixel_buffer_dma_dev* pixel_buffer);


#endif
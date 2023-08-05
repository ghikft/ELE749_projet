#ifndef SNAKE_H_
#define SNAKE_H_

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


#define PLAY_AREA_X 480
#define PLAY_AREA_Y 480
#define BLACKBAR_WIDTH 80
#define SQUARE_SIZE 12
#define GRID_COLOR BLACK

#define X_MAX 640
#define Y_MAX 480

//board defines
#define SNAKE_HEAD NEON_GREEN
#define SNAKE_BODY BROWN
#define APPLE RED
#define SNAKE_INIT_LEN 5

#define SQUARE_WIDTH 12

//directions
#define  UP     1
#define  DOWN   2
#define  LEFT   3
#define  RIGHT  4

void play_snake(alt_up_pixel_buffer_dma_dev* pixel_buffer);

#endif
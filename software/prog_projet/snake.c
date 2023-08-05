#include "snake.h"

//global vars
int board[40][40];

typedef struct snake_position{
    int x;
    int y;
}snake_position;

snake_position snake[PLAY_AREA_X*PLAY_AREA_Y];
snake_position apple;
int snakeLength = 0;

//Private functions5
void draw_grid(alt_up_pixel_buffer_dma_dev* pixel_buffer, int black_bars);
void setup_snake(void);
void draw_snake_in_board(void);
void clear_board(void);
void draw_board(alt_up_pixel_buffer_dma_dev* pixel_buffer);
void generate_apple(void);

void play_snake(alt_up_pixel_buffer_dma_dev* pixel_buffer){
    setup_snake();
    draw_grid(pixel_buffer,1);
    draw_board(pixel_buffer);
}

void draw_grid(alt_up_pixel_buffer_dma_dev* pixel_buffer, int black_bars){
    //draw black bars
    if(black_bars){
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0,0,BLACKBAR_WIDTH,PLAY_AREA_Y,BLACK,0);
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer, PLAY_AREA_X+BLACKBAR_WIDTH,0,X_MAX,Y_MAX,GRID_COLOR,0);
    }
    //drawgrid
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, BLACKBAR_WIDTH,0,PLAY_AREA_X+BLACKBAR_WIDTH,PLAY_AREA_Y,WHITE,0);
    //vertical lines
    int line_x = 80;
    while(line_x <= BLACKBAR_WIDTH+PLAY_AREA_X){
        alt_up_pixel_buffer_dma_draw_vline(pixel_buffer, line_x,0,Y_MAX,BLACK,0);
        line_x += 12;
    }
    //horizontal lines
    int line_y = 0;
    while(line_y <= PLAY_AREA_Y){
        alt_up_pixel_buffer_dma_draw_hline(pixel_buffer,BLACKBAR_WIDTH,PLAY_AREA_X+BLACKBAR_WIDTH,line_y,GRID_COLOR,0);
        line_y += 12;
    }
}

void setup_snake(void){
    srand(time(NULL));
	//start the time stamp timer
	alt_timestamp_start();
    //init board
    clear_board();
    //create inital snake
    snakeLength = 0;
    for (int i=0;i<SNAKE_INIT_LEN;i++){
        snake[i].x=19;
        snake[i].y=19+i;
        snakeLength++;
    }
    draw_snake_in_board();
    generate_apple();
}

void draw_snake_in_board(void){
    for(int i=0;i<snakeLength;i++){
        if(i==0){
            board[snake[i].x][snake[i].y] = SNAKE_HEAD;
            //printf("head x: %d y: %d\n\r",snake[i].x, snake[i].y);
        }else{
            board[snake[i].x][snake[i].y] = SNAKE_BODY;
            //printf("body x: %d y: %d\n\r",snake[i].x, snake[i].y);
        }
    }
}

void clear_board(void){
    for(int i=0;i<40;i++){
        for(int j=0;j<40;j++){
            board[i][j] = WHITE;
        }
    }
}

void draw_board(alt_up_pixel_buffer_dma_dev* pixel_buffer){
    draw_grid(pixel_buffer,0);
    for(int i=0;i<40;i++){
        for(int j=0;j<40;j++){
            if(board[i][j] != WHITE){
                int x = BLACKBAR_WIDTH+(i*SQUARE_WIDTH)+1;
                int y = (j*SQUARE_WIDTH)+1;
                printf("i: %d j: %d\n\r",i,j);
                printf("x: %d y: %d\n\r",x,y);

                alt_up_pixel_buffer_dma_draw_box(pixel_buffer,x,y,x+SQUARE_WIDTH-2,y+SQUARE_WIDTH-2,board[i][j],0);
            }
        }
    }
}

void generate_apple(void){
    apple.x = rand() % 40;
    apple.y = rand() % 40;

    board[apple.y][apple.x] = APPLE;
}
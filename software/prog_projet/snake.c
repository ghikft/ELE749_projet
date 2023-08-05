#include "snake.h"

//global vars
int board[40][40];
int board_old[40][40];

typedef struct snake_position{
    int x;
    int y;
}snake_position;

snake_position snake[PLAY_AREA_X*PLAY_AREA_Y];

snake_position apple;
int snakeLength = 0;
int previousDirection = UP;
int direction = UP;
int dead = 0;

//Private functions
void draw_grid(alt_up_pixel_buffer_dma_dev* pixel_buffer, int black_bars);
void setup_snake(void);
void draw_snake_in_board(void);
void clear_board(void);
void draw_board(alt_up_pixel_buffer_dma_dev* pixel_buffer);
void generate_apple(void);
void process_snake(void);
void snake_io(char* snakeUp, char* snakeDown, char* snakeLeft, char* snakeRight);
void process_snake_up(void);
void process_snake_down(void);
void process_snake_left(void);
void process_snake_right(void);

void play_snake(alt_up_pixel_buffer_dma_dev* pixel_buffer){
    float totalTime = 0;
    uint32_t temps1 = 0;
    temps1 = alt_timestamp();
    totalTime = (float)temps1*2.0e-8;
    //butons
    char upB, downB, leftB, rightB;

    setup_snake();
    draw_grid(pixel_buffer,1);
    while(1){
        if (!alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer)) {
            //get timer
            temps1 = alt_timestamp();
            totalTime = (float)temps1*2.0e-8;
            snake_io(&upB,&downB,&leftB,&rightB);
            if(upB==0){
                direction = UP;
            }
            else if(downB==0){
                direction = DOWN;
            }
            else if(leftB==0){
                direction = LEFT;
            }
            else if(rightB==0){
                direction = RIGHT;
            }
            //draw new board
            if (totalTime >= 0.25){
                alt_timestamp_start();
                process_snake();
                draw_snake_in_board();
                draw_board(pixel_buffer);
            }
            //is dead
            if (dead){
                dead = 0;
                direction = UP;
                previousDirection = UP;
                setup_snake();
                draw_grid(pixel_buffer,1);
            }

            alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
        }
    }
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
    //draw_grid(pixel_buffer,0);
    for(int i=0;i<40;i++){
        for(int j=0;j<40;j++){
            if(board[i][j] != board_old[i][j]){
                board_old[i][j] == board[i][j];
                int x = BLACKBAR_WIDTH+(i*SQUARE_WIDTH)+1;
                int y = (j*SQUARE_WIDTH)+1;
                //printf("i: %d j: %d\n\r",i,j);
                //printf("x: %d y: %d\n\r",x,y);

                alt_up_pixel_buffer_dma_draw_box(pixel_buffer,x,y,x+SQUARE_WIDTH-2,y+SQUARE_WIDTH-2,board[i][j],0);
            }
        }
    }
}

void generate_apple(void){

    apple.x = rand() % 40;
    apple.y = rand() % 40;

    for(int i=0;i<snakeLength;i++){
        if ((apple.x == snake[i].x) && (apple.y == snake[i].y)){
            generate_apple();
            break;
        }
    }

    board[apple.x][apple.y] = APPLE;
}

void process_snake(void){
    switch (direction)
    {
    case UP:
        if(previousDirection != DOWN){
            previousDirection = direction;
            process_snake_up();
        }
        else{
            direction = DOWN;
            process_snake_down();
        }
        break;
    case DOWN:
        if(previousDirection != UP){
            previousDirection = direction;
            process_snake_down();
        }
        else{
            direction = UP;
            process_snake_up();
        }
        break;
    case LEFT:
        if(previousDirection != RIGHT){
            previousDirection = direction;
            process_snake_left();
        }
        else{
            direction = RIGHT;
            process_snake_right();
        }
        break;
    case RIGHT:
        if(previousDirection != LEFT){
            previousDirection = direction;
            process_snake_right();
        }
        else{
            direction = LEFT;
            process_snake_left();
        }
        break;
    default:
        break;
    }
}

void process_snake_up(void){
    int x_next = snake[0].x;
    int y_next = snake[0].y-1;
    int appleHit = 0;

    for(int i=0;i<snakeLength-1;i++){
        if((x_next==snake[i].x)&&(y_next==snake[i].y)){
            dead = 1;
        }
    }

    if ((x_next==apple.x)&&(y_next==apple.y)){
        snake[snakeLength+1]=snake[snakeLength];
        appleHit = 1;
    }

    for (int i=snakeLength;i>=0;i--){
        if (i==0){
            if(y_next<0){
                dead = 1;
            }
            else{
                snake[0].x = x_next;
                snake[0].y = y_next;
            }
        }
        else if(i==snakeLength){
            board[snake[snakeLength].x][snake[snakeLength].y] = WHITE;
            snake[i]=snake[i-1];
        }
        else{
            snake[i]=snake[i-1];
        }
    }
    if (appleHit){
        snakeLength++;
        generate_apple();
    }
}

void process_snake_down(void){
    int x_next = snake[0].x;
    int y_next = snake[0].y+1;
    int appleHit = 0;

    for(int i=0;i<snakeLength-1;i++){
        if((x_next==snake[i].x)&&(y_next==snake[i].y)){
            dead = 1;
        }
    }

    if ((x_next==apple.x)&&(y_next==apple.y)){
        snake[snakeLength+1]=snake[snakeLength];
        appleHit = 1;
    }
    
    for (int i=snakeLength;i>=0;i--){
        if (i==0){
            if(y_next>39){
                dead = 1;
            }
            else{
                snake[0].x = x_next;
                snake[0].y = y_next;
            }
        }
        else if(i==snakeLength){
            board[snake[snakeLength].x][snake[snakeLength].y] = WHITE;
            snake[i]=snake[i-1];
        }
        else{
            snake[i]=snake[i-1];
        }
    }

    if (appleHit){
        snakeLength++;
        generate_apple();
    }
}

void process_snake_left(void){
    int x_next = snake[0].x-1;
    int y_next = snake[0].y;
    int appleHit = 0;

    for(int i=0;i<snakeLength-1;i++){
        if((x_next==snake[i].x)&&(y_next==snake[i].y)){
            dead = 1;
        }
    }

    if ((x_next==apple.x)&&(y_next==apple.y)){
        snake[snakeLength+1]=snake[snakeLength];
        appleHit = 1;
    }
    
    for (int i=snakeLength;i>=0;i--){
        if (i==0){
            if(x_next<0){
                dead = 1;
            }
            else{
                snake[0].x = x_next;
                snake[0].y = y_next;
            }
        }
        else if(i==snakeLength){
            board[snake[snakeLength].x][snake[snakeLength].y] = WHITE;
            snake[i]=snake[i-1];
        }
        else{
            snake[i]=snake[i-1];
        }
    }

    if (appleHit){
        snakeLength++;
        generate_apple();
    }
}

void process_snake_right(void){
    int x_next = snake[0].x+1;
    int y_next = snake[0].y;
    int appleHit = 0;

    for(int i=0;i<snakeLength-1;i++){
        if((x_next==snake[i].x)&&(y_next==snake[i].y)){
            dead = 1;
        }
    }

    if ((x_next==apple.x)&&(y_next==apple.y)){
        snake[snakeLength+1]=snake[snakeLength];
        appleHit = 1;
    }

    for (int i=snakeLength;i>=0;i--){
        if (i==0){
            if(x_next>39){
                dead = 1;
            }
            else{
                snake[0].x = x_next;
                snake[0].y = y_next;
            }
        }
        else if(i==snakeLength){
            board[snake[snakeLength].x][snake[snakeLength].y] = WHITE;
            snake[i]=snake[i-1];
        }
        else{
            snake[i]=snake[i-1];
        }
    }

    if (appleHit){
        snakeLength++;
        generate_apple();
    }
}

void snake_io(char* snakeUp, char* snakeDown, char* snakeLeft, char* snakeRight) {
	char remoteStatus = 0;
	get_remote(REMOTE_BASE, &remoteStatus);

	*snakeUp = remoteStatus & 0x08;
	*snakeDown = remoteStatus & 0x04;

	*snakeLeft = remoteStatus & 0x02;
	*snakeRight = remoteStatus & 0x01;
}
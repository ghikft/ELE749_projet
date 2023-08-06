#include "snake.h"

typedef struct snake_position{
    int x;
    int y;
}snake_position;

//global vars
int board[40][40];
int board_old[40][40];
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
void exitButton_io(char* exitButton);

void play_snake(alt_up_pixel_buffer_dma_dev* pixel_buffer){
    float totalTime = 0;
    uint32_t temps1 = 0;
    temps1 = alt_timestamp();
    totalTime = (float)temps1*2.0e-8;
    //butons
    char upB, downB, leftB, rightB;
    char exitButton = 0;
    int exitGame = 0;

    setup_snake();
    draw_grid(pixel_buffer,1);
    while(exitGame == 0){
        if (!alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer)) {
            //get timer
            temps1 = alt_timestamp();
            totalTime = (float)temps1*2.0e-8;
            //get buttons on remote and board
            snake_io(&upB,&downB,&leftB,&rightB);
            exitButton_io(&exitButton);
            //if exit button is pressed set exit flag to 1
            if(exitButton == 0){
                exitGame = 1;
            }
            //process the direction of the snake
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
//draw empty snake grid on screen
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
//setup fonction for the game
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
//draw snake in the 40x40 board
void draw_snake_in_board(void){
    for(int i=0;i<snakeLength;i++){
        //snake head different color
        if(i==0){
            board[snake[i].x][snake[i].y] = SNAKE_HEAD;
        }else{ //snake body
            board[snake[i].x][snake[i].y] = SNAKE_BODY;
        }
    }
}
//erase everything in the 40x40 board
void clear_board(void){
    //set every cell to white on board
    for(int i=0;i<40;i++){
        for(int j=0;j<40;j++){
            board[i][j] = WHITE;
        }
    }
}
//draws the 40x40 board on screen
void draw_board(alt_up_pixel_buffer_dma_dev* pixel_buffer){
    //convert the 40x40 grid of snake into somthing that can be displayed
    for(int i=0;i<40;i++){
        for(int j=0;j<40;j++){
            //only draw what changed
            if(board[i][j] != board_old[i][j]){
                board_old[i][j] = board[i][j];
                //calculate square position based on grid and square size in pixel
                int x = BLACKBAR_WIDTH+(i*SQUARE_WIDTH)+1;
                int y = (j*SQUARE_WIDTH)+1;
                //draw square in pixel buffer
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer,x,y,x+SQUARE_WIDTH-2,y+SQUARE_WIDTH-2,board[i][j],0);
            }
        }
    }
}
//creates an apple for the snake
void generate_apple(void){
    //generate random coordinates for the apple
    apple.x = rand() % 40;
    apple.y = rand() % 40;
    //check if apple is on snake
    for(int i=0;i<snakeLength;i++){
        if ((apple.x == snake[i].x) && (apple.y == snake[i].y)){
            generate_apple();
            break;
        }
    }
    //place apple in board
    board[apple.x][apple.y] = APPLE;
}
//process the snake movements
void process_snake(void){
    switch (direction)
    {
    case UP:
        if(previousDirection != DOWN){ //prevent going back on itself
            previousDirection = direction;
            process_snake_up();
        }
        else{
            direction = DOWN;
            process_snake_down();
        }
        break;
    case DOWN:
        if(previousDirection != UP){ //prevent going back on itself
            previousDirection = direction;
            process_snake_down();
        }
        else{
            direction = UP;
            process_snake_up();
        }
        break;
    case LEFT:
        if(previousDirection != RIGHT){ //prevent going back on itself
            previousDirection = direction;
            process_snake_left();
        }
        else{
            direction = RIGHT;
            process_snake_right();
        }
        break;
    case RIGHT:
        if(previousDirection != LEFT){ //prevent going back on itself
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
//process the up movement of the snake
void process_snake_up(void){
    //calculate next head position
    int x_next = snake[0].x;
    int y_next = snake[0].y-1;
    int appleHit = 0;
    //check if snake went back on itself
    for(int i=0;i<snakeLength-1;i++){
        if((x_next==snake[i].x)&&(y_next==snake[i].y)){
            dead = 1;
        }
    }
    //check if snake ate the apple
    if ((x_next==apple.x)&&(y_next==apple.y)){
        snake[snakeLength+1]=snake[snakeLength];
        appleHit = 1;
    }
    //moves every square of the snake
    for (int i=snakeLength;i>=0;i--){
        //special case for the head, takes the new calculated position
        if (i==0){
            if(y_next<0){
                dead = 1;
            }
            else{
                snake[0].x = x_next;
                snake[0].y = y_next;
            }
        }
        //erase last square of the snake on board
        else if(i==snakeLength){
            board[snake[snakeLength].x][snake[snakeLength].y] = WHITE;
            snake[i]=snake[i-1];
        }
        //every square other than the head takes the value of the next one
        else{
            snake[i]=snake[i-1];
        }
    }
    //generate new apple if snake ate it
    if (appleHit){
        snakeLength++;
        generate_apple();
    }
}
//process the down movement of the snake
void process_snake_down(void){
    //calculate next head position
    int x_next = snake[0].x;
    int y_next = snake[0].y+1;
    int appleHit = 0;
    //check if snake went back on itself
    for(int i=0;i<snakeLength-1;i++){
        if((x_next==snake[i].x)&&(y_next==snake[i].y)){
            dead = 1;
        }
    }
    //check if snake ate the apple
    if ((x_next==apple.x)&&(y_next==apple.y)){
        snake[snakeLength+1]=snake[snakeLength];
        appleHit = 1;
    }
    //moves every square of the snake
    for (int i=snakeLength;i>=0;i--){
        //special case for the head, takes the new calculated position
        if (i==0){
            if(y_next>39){
                dead = 1;
            }
            else{
                snake[0].x = x_next;
                snake[0].y = y_next;
            }
        }
        //erase last square of the snake on board
        else if(i==snakeLength){
            board[snake[snakeLength].x][snake[snakeLength].y] = WHITE;
            snake[i]=snake[i-1];
        }
        //every square other than the head takes the value of the next one
        else{
            snake[i]=snake[i-1];
        }
    }
    //generate new apple if snake ate it
    if (appleHit){
        snakeLength++;
        generate_apple();
    }
}
//process the left movement of the snake
void process_snake_left(void){
    //calculate next head position
    int x_next = snake[0].x-1;
    int y_next = snake[0].y;
    int appleHit = 0;
    //check if snake went back on itself
    for(int i=0;i<snakeLength-1;i++){
        if((x_next==snake[i].x)&&(y_next==snake[i].y)){
            dead = 1;
        }
    }
    //check if snake ate the apple
    if ((x_next==apple.x)&&(y_next==apple.y)){
        snake[snakeLength+1]=snake[snakeLength];
        appleHit = 1;
    }
    //moves every square of the snake
    for (int i=snakeLength;i>=0;i--){
        //special case for the head, takes the new calculated position
        if (i==0){
            if(x_next<0){
                dead = 1;
            }
            else{
                snake[0].x = x_next;
                snake[0].y = y_next;
            }
        }
        //erase last square of the snake on board
        else if(i==snakeLength){
            board[snake[snakeLength].x][snake[snakeLength].y] = WHITE;
            snake[i]=snake[i-1];
        }
        //every square other than the head takes the value of the next one
        else{
            snake[i]=snake[i-1];
        }
    }
    //generate new apple if snake ate it
    if (appleHit){
        snakeLength++;
        generate_apple();
    }
}
//process the right movement of the snake
void process_snake_right(void){
    //calculate next head position
    int x_next = snake[0].x+1;
    int y_next = snake[0].y;
    int appleHit = 0;
    //check if snake went back on itself
    for(int i=0;i<snakeLength-1;i++){
        if((x_next==snake[i].x)&&(y_next==snake[i].y)){
            dead = 1;
        }
    }
    //check if snake ate the apple
    if ((x_next==apple.x)&&(y_next==apple.y)){
        snake[snakeLength+1]=snake[snakeLength];
        appleHit = 1;
    }
    //moves every square of the snake
    for (int i=snakeLength;i>=0;i--){
        //special case for the head, takes the new calculated position
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
            //erase last square of the snake on board
            board[snake[snakeLength].x][snake[snakeLength].y] = WHITE;
            snake[i]=snake[i-1];
        }
        //every square other than the head takes the value of the next one
        else{
            snake[i]=snake[i-1];
        }
    }
    //generate new apple if snake ate it
    if (appleHit){
        snakeLength++;
        generate_apple();
    }
}
//gets the value of the buttons on the remote
void snake_io(char* snakeUp, char* snakeDown, char* snakeLeft, char* snakeRight) {
	char remoteStatus = 0;
    //get the status of the buttons on the remote
	get_remote(REMOTE_BASE, &remoteStatus);
    //set the value of the button to the proper var
	*snakeUp = remoteStatus & 0x08;
	*snakeDown = remoteStatus & 0x04;

	*snakeLeft = remoteStatus & 0x02;
	*snakeRight = remoteStatus & 0x01;
}
//gets the value of the stop game button on the board
void exitButton_io(char* exitButton){
    char buttonStatus = 0;
    get_button(BUTTON_BASE, &buttonStatus);
    *exitButton = buttonStatus & 0x01;
}
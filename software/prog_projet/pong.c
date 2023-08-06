#include "pong.h"

typedef struct position{
    int x;
    int y;
}position;

typedef struct ballType{
    float x;
    float y;
    float x_speed;
    float y_speed;
}ballType;

//global variables
int board_pong[46][63];
int board_old_pong[46][63];

position leftPaddle[PADDLE_HEIGHT];
position rightPaddle[PADDLE_HEIGHT];
ballType ball;

int directionP1 = NONE;
int directionP2 = NONE;

int p1Score=0;
int p2Score=0;

int p1Scored=0;
int p2Scored=0;

//private functions
void draw_board_pong(alt_up_pixel_buffer_dma_dev* pixel_buffer);
void create_paddles(void);
void draw_paddles_in_board(void);
void draw_play_area(alt_up_pixel_buffer_dma_dev* pixel_buffer);
void clear_board_pong(void);
void setup_pong(alt_up_pixel_buffer_dma_dev* pixel_buffer);
void pong_io(char* P1up, char* P1down, char* P2up, char* P2down);
void process_paddle_left(void);
void process_paddle_right(void);
void process_paddles(void);
void create_ball(void);
void draw_ball_in_board(void);
void process_ball(void);
void show_score(alt_up_pixel_buffer_dma_dev* pixel_buffer);
void exitButton_io_pong(char* exitButton);

//public functions
void play_pong(alt_up_pixel_buffer_dma_dev* pixel_buffer){
    float totalTime = 0;
    uint32_t temps1 = 0;
    temps1 = alt_timestamp();
    totalTime = (float)temps1*2.0e-8;
    //butons
    char p1Up, p1Down, p2Up, p2Down;
    char exitButton = 0;
    int exitGame = 0;

    setup_pong(pixel_buffer);
    while (exitGame == 0){
        if (!alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer)) {

            //get timer
            temps1 = alt_timestamp();
            totalTime = (float)temps1*2.0e-8;
            //if a player scored
            if (p1Scored || p2Scored){
                //if a player has scored the his 7th point, reset the game
                if((p1Score>6)||(p2Score>6)){
                    p1Score = 0;
                    p2Score = 0;
                    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0,0,PLAY_AREA_WIDTH,TOP_BAR_HEIGHT-1,WHITE,0);
                    //draw_play_area(pixel_buffer);
                }
                //display player score at the top of the screen
                show_score(pixel_buffer);
                //reset the board in between points
                clear_board_pong();
                create_paddles();
                draw_paddles_in_board();
                create_ball();
                //sends the inital ball to the person who has not scored
                if(p1Scored){
                    p1Scored = 0;
                    ball.x_speed = INIT_X_SPEED;
                }else{
                    p2Scored = 0;
                    ball.x_speed = -INIT_X_SPEED;
                }
                usleep(10000);
                temps1 = alt_timestamp();
            }
            //get buttons states
            pong_io(&p1Up,&p1Down,&p2Up,&p2Down);
            exitButton_io_pong(&exitButton);
            if(exitButton == 0){
                exitGame = 1;
            }
            //process player 1 paddle direction
            if (p1Up==0){
                directionP1 = UP;
            }
            else if(p1Down == 0){
                directionP1 = DOWN;
            }
            else{
                directionP1 = NONE;
            }
            //process player 2 paddle direction
            if (p2Up==0){
                directionP2 = UP;
            }
            else if(p2Down == 0){
                directionP2 = DOWN;
            }
            else{
                directionP2 = NONE;
            }
            //refresh screen every 50ms
            if (totalTime>0.05){
                alt_timestamp_start();
                process_paddles();
                draw_paddles_in_board();
                process_ball();
                draw_ball_in_board();
                draw_board_pong(pixel_buffer);
                //printf("xspeed: %f\n\r",ball.x_speed);
            }
           
            alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
        }
    }
    
}

//	/**************************************************************************
//	 * initial setup for the pong game
//	 **************************************************************************
//	 * Parametres
//   * pixel_buffer: pointer to the pixel buffer
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * pixel_buffer will be mofified
//	 *************************************************************************/
void setup_pong(alt_up_pixel_buffer_dma_dev* pixel_buffer){
    srand(time(NULL));
	//start the time stamp timer
	alt_timestamp_start();
    //init board
    clear_board_pong();
    draw_play_area(pixel_buffer);
    create_paddles();
    draw_paddles_in_board();
    create_ball();
}

//	/**************************************************************************
//	 * erase everything in the 46x63 board
//	 **************************************************************************
//	 * Parametres
//   * None
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * None
//	 *************************************************************************/
void clear_board_pong(void){
    //set every cell to white on board
    for(int i=0;i<46;i++){
        for(int j=0;j<64;j++){
            board_pong[i][j] = BLACK;
        }
    }
}

//	/**************************************************************************
//	 * create both paddles for each player
//	 **************************************************************************
//	 * Parametres
//   * None
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * None
//	 *************************************************************************/
void create_paddles(void){
    for(int i=0;i<PADDLE_HEIGHT;i++){
        //player 1
        leftPaddle[i].x = 0;
        leftPaddle[i].y = 18+i;
        //player 2
        rightPaddle[i].x = 62;
        rightPaddle[i].y = 18+i;
    }
}

//	/**************************************************************************
//	 * draw the paddles in the 46x63 board
//	 **************************************************************************
//	 * Parametres
//   * None
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * None
//	 *************************************************************************/
void draw_paddles_in_board(void){
    for(int i =0;i<PADDLE_HEIGHT;i++){
        board_pong[leftPaddle[i].y][leftPaddle[i].x] = PADDLE_COLOUR;
        board_pong[rightPaddle[i].y][rightPaddle[i].x] = PADDLE_COLOUR;
    }
}

//	/**************************************************************************
//	 * draw the play area
//	 **************************************************************************
//	 * Parametres
//   * None
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * None
//	 *************************************************************************/
void draw_play_area(alt_up_pixel_buffer_dma_dev* pixel_buffer){
    //top white bar
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0,0,PLAY_AREA_WIDTH,TOP_BAR_HEIGHT,WHITE,0);
    //black play area
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0,20,640,480,BLACK,0);
}

//	/**************************************************************************
//	 * draws the 46x63 board on the screen
//	 **************************************************************************
//	 * Parametres
//   * pixel_buffer: pointer to the pixel buffer
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * pixel_buffer will be mofified
//	 *************************************************************************/
void draw_board_pong(alt_up_pixel_buffer_dma_dev* pixel_buffer){
    //convert the 46x63 grid of snake into somthing that can be displayed
    for(int i=0;i<46;i++){
        for(int j=0;j<63;j++){
            //only draw what changed
            if(board_pong[i][j] != board_old_pong[i][j]){
                board_old_pong[i][j] = board_pong[i][j];
                //calculate square position based on grid and square size in pixel
                int x = (j*SQUARE_WIDTH_PONG)+1;
                int y = TOP_BAR_HEIGHT+(i*SQUARE_WIDTH_PONG)+1;
                //draw square in pixel buffer
                alt_up_pixel_buffer_dma_draw_box(pixel_buffer,x,y,x+SQUARE_WIDTH_PONG-1,y+SQUARE_WIDTH_PONG-1,board_pong[i][j],0);
            }
        }
    }
}

//	/**************************************************************************
//	 * gets the value of the buttons of the remotes of p1 and p2
//	 **************************************************************************
//	 * Parametres
//   * char* p1up: player 1 up direction
//   * char* p1down: player 1 down direction
//   * char* p2up: player 2 up direction
//   * char* p2down: player 2 down direction
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * p1up, p1down, p2up, p2down will be modified
//	 *************************************************************************/
void pong_io(char* P1up, char* P1down, char* P2up, char* P2down) {
	char remoteStatus = 0;
	get_remote(REMOTE_BASE, &remoteStatus);
    //player 1 remote
	*P1down = remoteStatus & 0x04;
	*P1up = remoteStatus & 0x08;
    //player 2 remote
	*P2down = remoteStatus & 0x10;
	*P2up = remoteStatus & 0x20;
}

//	/**************************************************************************
//	 * process the paddle movement
//	 **************************************************************************
//	 * Parametres
//   * None
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * None
//	 *************************************************************************/
void process_paddles(void){
    process_paddle_left();
    process_paddle_right();
}

//	/**************************************************************************
//	 * process the player 1 paddle movement
//	 **************************************************************************
//	 * Parametres
//   * None
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * None
//	 *************************************************************************/
void process_paddle_left(void){
    if(directionP1 == UP){
        //calculate where the top of the paddle will be
        int y_next = leftPaddle[0].y-1;
        //prevent going of the top of the board
        if(y_next>=0){
            //move the rest of the paddle
            for (int i=PADDLE_HEIGHT-1;i>=0;i--){
                //take next calculates position
                if (i==0){
                    leftPaddle[0].y = y_next;
                }
                //erase last square
                else if(i==PADDLE_HEIGHT-1){
                    board_pong[leftPaddle[PADDLE_HEIGHT-1].y][leftPaddle[PADDLE_HEIGHT-1].x] = BLACK;
                    leftPaddle[i]=leftPaddle[i-1];
                }
                else{
                    leftPaddle[i]=leftPaddle[i-1];
                }
            }
        }
    }
    else if(directionP1 == DOWN){
        //calculate where the bottom of the paddle will be
        int y_next = leftPaddle[PADDLE_HEIGHT-1].y+1;
        //prevent going of the bottom of the board
        if(y_next<46){
            //move the rest of the paddle
            for (int i=0;i<PADDLE_HEIGHT;i++){
                //take next calculates position
                if (i==PADDLE_HEIGHT-1){
                    leftPaddle[PADDLE_HEIGHT-1].y = y_next;
                }
                //erase last square
                else if(i==0){
                    board_pong[leftPaddle[0].y][leftPaddle[0].x] = BLACK;
                    leftPaddle[i]=leftPaddle[i+1];
                }
                else{
                    leftPaddle[i]=leftPaddle[i+1];
                }
            }
        }
    }
}

//	/**************************************************************************
//	 * process the player 2 paddle movement
//	 **************************************************************************
//	 * Parametres
//   * None
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * None
//	 *************************************************************************/
void process_paddle_right(void){
    if(directionP2 == UP){
        //calculate where the top of the paddle will be
        int y_next = rightPaddle[0].y-1;
        //prevent going of the top of the board
        if(y_next>=0){
            //move the rest of the paddle
            for (int i=PADDLE_HEIGHT-1;i>=0;i--){
                //take next calculates position
                if (i==0){
                    rightPaddle[0].y = y_next;
                }
                //erase last square
                else if(i==PADDLE_HEIGHT-1){
                    board_pong[rightPaddle[PADDLE_HEIGHT-1].y][rightPaddle[PADDLE_HEIGHT-1].x] = BLACK;
                    rightPaddle[i]=rightPaddle[i-1];
                }
                else{
                    rightPaddle[i]=rightPaddle[i-1];
                }
            }
        }
    }
    else if(directionP2 == DOWN){
        //calculate where the bottom of the paddle will be
        int y_next = rightPaddle[PADDLE_HEIGHT-1].y+1;
        //prevent going of the bottom of the board
        if(y_next<46){
            //move the rest of the paddle
            for (int i=0;i<PADDLE_HEIGHT;i++){
                //take next calculates position
                if (i==PADDLE_HEIGHT-1){
                    rightPaddle[PADDLE_HEIGHT-1].y = y_next;
                }
                //erase last square
                else if(i==0){
                    board_pong[rightPaddle[0].y][rightPaddle[0].x] = BLACK;
                    rightPaddle[i]=rightPaddle[i+1];
                }
                else{
                    rightPaddle[i]=rightPaddle[i+1];
                }
            }
        }
    }
}

//	/**************************************************************************
//	 * create the ball in the middle of the board with initial speed vectors
//	 **************************************************************************
//	 * Parametres
//   * None
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * None
//	 *************************************************************************/
void create_ball(void){
    ball.x = 63/2;
    ball.y = 46/2;

    ball.x_speed = INIT_X_SPEED;
    ball.y_speed = INIT_Y_SPEED;
}

//	/**************************************************************************
//	 * draw the ball in the 43x63 board
//	 **************************************************************************
//	 * Parametres
//   * None
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * None
//	 *************************************************************************/
void draw_ball_in_board(void){
    board_pong[(int)ball.y][(int)ball.x] = BALL_COLOUR;
}

//	/**************************************************************************
//	 * process the ball movement and interactions with paddles and wall
//	 **************************************************************************
//	 * Parametres
//   * None
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * None
//	 *************************************************************************/
void process_ball(void){
    //erase previous ball
    board_pong[(int)ball.y][(int)ball.x] = BACKGROUD_COLOUR;
    //calculate next position
    float next_ball_x = ball.x + ball.x_speed;
    float next_ball_y = ball.y + ball.y_speed;
    //check if ball hits a paddle
    for(int i=0;i<PADDLE_HEIGHT;i++){
        //check if ball hits p1 paddle
        if(((int)next_ball_x <= leftPaddle[i].x)&&((int)next_ball_y==leftPaddle[i].y)){
            //flip x speed and increase speed
            ball.x_speed = ball.x_speed*-1;
            ball.x_speed += +SPEED_INCREASE;
            next_ball_x = ball.x + ball.x_speed;
            //increase y speed if not null
            if (ball.y_speed>0){
                ball.y_speed+=SPEED_INCREASE;
            }
            else if(ball.y_speed<0){
                ball.y_speed-=SPEED_INCREASE;
            }
            //add spin to the ball if paddle is moving when hit
            if(directionP1 == UP){
                printf("spin\n\r");
                ball.y_speed -= SPIN;
            }
            else if(directionP1 == DOWN){
                printf("spin\n\r");
                ball.y_speed += SPIN;
            }
        }
        //check if ball hits p2 paddle
        if(((int)next_ball_x >= rightPaddle[i].x)&&((int)next_ball_y==rightPaddle[i].y)){
            //flip x speed and increase speed
            ball.x_speed = ball.x_speed*-1;
            ball.x_speed += -SPEED_INCREASE;
            next_ball_x = ball.x + ball.x_speed;
            //increase y speed if not null
            if (ball.y_speed>0){
                ball.y_speed+=SPEED_INCREASE;
            }
            else if(ball.y_speed<0){
                ball.y_speed-=SPEED_INCREASE;
            }
            //add spin to the ball if paddle is moving when hit
            if(directionP2 == UP){
                printf("spin\n\r");
                ball.y_speed -= SPIN;
            }
            else if(directionP2 == DOWN){
                printf("spin\n\r");
                ball.y_speed += SPIN;
            }
        }
    }
    //bounce ball of the top and bottom of the screen
    if ((next_ball_y > 46)||(next_ball_y < 0)){
        ball.y_speed = ball.y_speed*-1;
        next_ball_y = ball.y + ball.y_speed;
    }
    //check if player 1 scored
    if (next_ball_x > 62){
        p1Score++;
        p1Scored = 1;
    }
    //check if p2 scored
    else if(next_ball_x < 0){
        p2Score++;
        p2Scored = 1;
    }
    //clip speed at 10
    if(ball.x_speed>10){
        ball.x_speed = 10;
    }
    if(ball.y_speed>10){
        ball.y_speed = 10;
    }
    //set new ball coordinates
    ball.x = next_ball_x;
    ball.y = next_ball_y;
}

//	/**************************************************************************
//	 * diplay the score in the form of squares in the top bar
//	 **************************************************************************
//	 * Parametres
//   * pixel_buffer: pointer to the pixel buffer
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * pixel_buffer will be modified
//	 *************************************************************************/
void show_score(alt_up_pixel_buffer_dma_dev* pixel_buffer){
    //display player1 score
    for(int i=0;i<p1Score;i++){
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer,i*10+5+5*i,5,i*10+5+5*i+SQUARE_WIDTH_PONG,15,BLACK,0);
    }
    //display player 2 score
    for(int i=0;i<p2Score;i++){
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer,PLAY_AREA_WIDTH-(i*10+5+5*i+SQUARE_WIDTH_PONG+10),5,PLAY_AREA_WIDTH-(i*10+5+5*i+10),15,BLACK,0);
    }
}

//	/**************************************************************************
//	 * get the value of the exit button
//	 **************************************************************************
//	 * Parametres
//   * char* exitButton: pointer to the exitButton variable
//	 *
//	 * Return
//	 * None
//	 *
//	 * Side effects
//	 * Value of exitButton will be modified
//	 *************************************************************************/
void exitButton_io_pong(char* exitButton){
    char buttonStatus = 0;
    get_button(BUTTON_BASE, &buttonStatus);
    *exitButton = buttonStatus & 0x01;
}
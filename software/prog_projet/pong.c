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

//public functions
void play_pong(alt_up_pixel_buffer_dma_dev* pixel_buffer){
    float totalTime = 0;
    uint32_t temps1 = 0;
    temps1 = alt_timestamp();
    totalTime = (float)temps1*2.0e-8;
    //butons
    char p1Up, p1Down, p2Up, p2Down;

    setup_pong(pixel_buffer);
    while (1){
        if (!alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer)) {

            //get timer
            temps1 = alt_timestamp();
            totalTime = (float)temps1*2.0e-8;

            if (p1Scored || p2Scored){
                if((p1Score>6)||(p2Score>6)){
                    p1Score = 0;
                    p2Score = 0;
                    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0,0,PLAY_AREA_WIDTH,TOP_BAR_HEIGHT-1,WHITE,0);
                    //draw_play_area(pixel_buffer);
                }
                show_score(pixel_buffer);
                clear_board_pong();
                create_paddles();
                draw_paddles_in_board();
                create_ball();
                if(p1Scored){
                    p1Scored = 0;
                    ball.x_speed = 0.25;
                }else{
                    p2Scored = 0;
                    ball.x_speed = -0.25;
                }
                //printf("p1 score: %d   p2 score: %d\n\r",p1Score,p2Score);
                usleep(10000);
                temps1 = alt_timestamp();
            }

            pong_io(&p1Up,&p1Down,&p2Up,&p2Down);

            if (p1Up==0){
                directionP1 = UP;
            }
            else if(p1Down == 0){
                directionP1 = DOWN;
            }
            else{
                directionP1 = NONE;
            }

            if (p2Up==0){
                directionP2 = UP;
            }
            else if(p2Down == 0){
                directionP2 = DOWN;
            }
            else{
                directionP2 = NONE;
            }

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

void clear_board_pong(void){
    for(int i=0;i<46;i++){
        for(int j=0;j<64;j++){
            board_pong[i][j] = BLACK;
        }
    }
}

void create_paddles(void){
    for(int i=0;i<PADDLE_HEIGHT;i++){
        leftPaddle[i].x = 0;
        leftPaddle[i].y = 18+i;

        rightPaddle[i].x = 62;
        rightPaddle[i].y = 18+i;
    }
}

void draw_paddles_in_board(void){
    for(int i =0;i<PADDLE_HEIGHT;i++){
        board_pong[leftPaddle[i].y][leftPaddle[i].x] = PADDLE_COLOUR;
        board_pong[rightPaddle[i].y][rightPaddle[i].x] = PADDLE_COLOUR;
    }
}

void draw_play_area(alt_up_pixel_buffer_dma_dev* pixel_buffer){
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0,0,PLAY_AREA_WIDTH,TOP_BAR_HEIGHT,WHITE,0);
    alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0,20,640,480,BLACK,0);
}

void draw_board_pong(alt_up_pixel_buffer_dma_dev* pixel_buffer){
    //draw_grid(pixel_buffer,0);
    for(int i=0;i<46;i++){
        for(int j=0;j<63;j++){
            if(board_pong[i][j] != board_old_pong[i][j]){
                board_old_pong[i][j] = board_pong[i][j];
                int x = (j*SQUARE_WIDTH_PONG)+1;
                int y = TOP_BAR_HEIGHT+(i*SQUARE_WIDTH_PONG)+1;
                //printf("i: %d j: %d\n\r",i,j);
                //printf("x: %d y: %d\n\r",x,y);

                alt_up_pixel_buffer_dma_draw_box(pixel_buffer,x,y,x+SQUARE_WIDTH_PONG-1,y+SQUARE_WIDTH_PONG-1,board_pong[i][j],0);
            }
        }
    }
}

void pong_io(char* P1up, char* P1down, char* P2up, char* P2down) {
	char remoteStatus = 0;
	get_remote(REMOTE_BASE, &remoteStatus);

	*P1down = remoteStatus & 0x04;
	*P1up = remoteStatus & 0x08;

	*P2down = remoteStatus & 0x10;
	*P2up = remoteStatus & 0x20;
}

void process_paddles(void){
    process_paddle_left();
    process_paddle_right();
}

void process_paddle_left(void){
    if(directionP1 == UP){
        int y_next = leftPaddle[0].y-1;
        if(y_next>=0){
            for (int i=PADDLE_HEIGHT-1;i>=0;i--){
                if (i==0){
                    leftPaddle[0].y = y_next;
                }
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
        int y_next = leftPaddle[PADDLE_HEIGHT-1].y+1;
        if(y_next<46){
            for (int i=0;i<PADDLE_HEIGHT;i++){
                if (i==PADDLE_HEIGHT-1){
                    leftPaddle[PADDLE_HEIGHT-1].y = y_next;
                }
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

void process_paddle_right(void){
    if(directionP2 == UP){
        int y_next = rightPaddle[0].y-1;
        if(y_next>=0){
            for (int i=PADDLE_HEIGHT-1;i>=0;i--){
                if (i==0){
                    rightPaddle[0].y = y_next;
                }
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
        int y_next = rightPaddle[PADDLE_HEIGHT-1].y+1;
        if(y_next<46){
            for (int i=0;i<PADDLE_HEIGHT;i++){
                if (i==PADDLE_HEIGHT-1){
                    rightPaddle[PADDLE_HEIGHT-1].y = y_next;
                }
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

void create_ball(void){
    ball.x = 63/2;
    ball.y = 46/2;

    ball.x_speed = INIT_X_SPEED;
    ball.y_speed = INIT_Y_SPEED;
}

void draw_ball_in_board(void){
    board_pong[(int)ball.y][(int)ball.x] = BALL_COLOUR;
}

void process_ball(void){
    board_pong[(int)ball.y][(int)ball.x] = BACKGROUD_COLOUR;
    
    float next_ball_x = ball.x + ball.x_speed;
    float next_ball_y = ball.y + ball.y_speed;
    for(int i=0;i<PADDLE_HEIGHT;i++){
        if(((int)next_ball_x <= leftPaddle[i].x)&&((int)next_ball_y==leftPaddle[i].y)){
            ball.x_speed = ball.x_speed*-1;
            ball.x_speed += +SPEED_INCREASE;
            next_ball_x = ball.x + ball.x_speed;
            if (ball.y_speed>0){
                ball.y_speed+=SPEED_INCREASE;
            }
            else if(ball.y_speed<0){
                ball.y_speed-=SPEED_INCREASE;
            }
            if(directionP1 == UP){
                printf("spin\n\r");
                ball.y_speed -= SPIN;
            }
            else if(directionP1 == DOWN){
                printf("spin\n\r");
                ball.y_speed += SPIN;
            }
        }
        if(((int)next_ball_x >= rightPaddle[i].x)&&((int)next_ball_y==rightPaddle[i].y)){
            ball.x_speed = ball.x_speed*-1;
            ball.x_speed += -SPEED_INCREASE;
            next_ball_x = ball.x + ball.x_speed;
            if (ball.y_speed>0){
                ball.y_speed+=SPEED_INCREASE;
            }
            else if(ball.y_speed<0){
                ball.y_speed-=SPEED_INCREASE;
            }
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

    if ((next_ball_y > 46)||(next_ball_y < 0)){
        ball.y_speed = ball.y_speed*-1;
        next_ball_y = ball.y + ball.y_speed;
    }

    if (next_ball_x > 62){
        p1Score++;
        p1Scored = 1;
    }
    else if(next_ball_x < 0){
        p2Score++;
        p2Scored = 1;
    }
    if(ball.x_speed>10){
        ball.x_speed = 10;
    }
    if(ball.y_speed>10){
        ball.y_speed = 10;
    }
    
    ball.x = next_ball_x;
    ball.y = next_ball_y;
}

void show_score(alt_up_pixel_buffer_dma_dev* pixel_buffer){
    for(int i=0;i<p1Score;i++){
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer,i*10+5+5*i,5,i*10+5+5*i+SQUARE_WIDTH_PONG,15,BLACK,0);
    }
    for(int i=0;i<p2Score;i++){
        alt_up_pixel_buffer_dma_draw_box(pixel_buffer,PLAY_AREA_WIDTH-(i*10+5+5*i+SQUARE_WIDTH_PONG+10),5,PLAY_AREA_WIDTH-(i*10+5+5*i+10),15,BLACK,0);
    }
}
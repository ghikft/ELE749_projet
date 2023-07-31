#include "recfiller.h"
#include "io.h"
#include "system.h"
#include <stdio.h>
#include <unistd.h>  	// usleep()


void recfiller_init(int x_size, int y_size)
{
    printf("start rec filler hw init/n\r");
    // set framebuffer address
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_FB_BASE_ADDR, ONCHIP_MEM_BASE);

    // set x_max
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_X_MAX_ADDR, x_size);

    // set y_max
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_Y_MAX_ADDR, y_size);
    printf("end rec filler hw init/n\r");
}

unsigned int recfiller_draw_rectangle(int x_left, int y_top, 
                              int x_right, int y_bottom, 
                              int color)
{
    printf("setup rec filler draw rec x_left\n\r");
    // setup rectangle parameters
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_X_LEFT_ADDR, x_left);
    printf("setup rec filler draw rec y top\n\r");
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_Y_TOP_ADDR, y_top);
    printf("setup rec filler draw rec x right\n\r");
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_X_RIGHT_ADDR, x_right);
    printf("setup rec filler draw rec y bottom\n\r");
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_Y_BOTTOM_ADDR, y_bottom);
    printf("setup rec filler draw rec color\n\r");
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_COLOR_ADDR, color);
    printf("end setup\n\r");
    printf("start1\n\r");
    // start drawing
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_START_ADDR, 1);

    // wait for completion
    volatile int result = 0;
    int count = 0;
    do {
        result = IORD_32DIRECT(RECFILLER_0_BASE, 0);
        count++;
        usleep(1000);

        if (count > 1000) {
            puts("waited for 1000 iterations");
            count = 0;
            return 0;
        }
    } while(result == 0);
    printf("end rec draw\n\r");
    result = IORD_32DIRECT(RECFILLER_0_BASE, 4);

    return result;
}
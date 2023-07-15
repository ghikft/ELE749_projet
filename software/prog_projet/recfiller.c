#include "recfiller.h"
#include "io.h"
#include "system.h"
#include <stdio.h>
#include <unistd.h>  	// usleep()


void recfiller_init(int x_size, int y_size)
{
    // set framebuffer address
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_FB_BASE_ADDR, ONCHIP_MEM_BASE);

    // set x_max
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_X_MAX_ADDR, x_size);

    // set y_max
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_Y_MAX_ADDR, y_size);
}

unsigned int recfiller_draw_rectangle(int x_left, int y_top, 
                              int x_right, int y_bottom, 
                              int color)
{
    // setup rectangle parameters
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_X_LEFT_ADDR, x_left);
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_Y_TOP_ADDR, y_top);
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_X_RIGHT_ADDR, x_right);
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_Y_BOTTOM_ADDR, y_bottom);
    IOWR_32DIRECT(RECFILLER_0_BASE, RF_COLOR_ADDR, color);

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

    result = IORD_32DIRECT(RECFILLER_0_BASE, 4);

    return result;
}
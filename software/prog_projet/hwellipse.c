#include "hwellipse.h"
#include "io.h"
#include "system.h"
#include <stdio.h>
#include <unistd.h>  	// usleep()


void hwellipse_init(int x_size, int y_size)
{
    printf("start hw ellipse hw init/n\r");
    // set framebuffer address
    IOWR_32DIRECT(HWELLIPSE_0_BASE, RF_FB_BASE_ADDR, ONCHIP_MEM_BASE);

    // set x_max
    IOWR_32DIRECT(HWELLIPSE_0_BASE, RF_X_MAX_ADDR, x_size);

    // set y_max
    IOWR_32DIRECT(HWELLIPSE_0_BASE, RF_Y_MAX_ADDR, y_size);
    printf("end hw ellipse init/n\r");
}

unsigned int hwellipse_draw_ellipse(int x_center, int y_center, 
                                    int x_radius, int y_radius, 
                                    int color)
{
    // setup rectangle parameters
    IOWR_32DIRECT(HWELLIPSE_0_BASE, RF_X_CENTER_ADDR, x_center);
    IOWR_32DIRECT(HWELLIPSE_0_BASE, RF_Y_CENTER_ADDR, y_center);
    IOWR_32DIRECT(HWELLIPSE_0_BASE, RF_X_RADIUS_ADDR, x_radius);
    IOWR_32DIRECT(HWELLIPSE_0_BASE, RF_Y_RADIUS_ADDR, y_radius);
    IOWR_32DIRECT(HWELLIPSE_0_BASE, RF_COLOR_ADDR, color);
    printf("start1\n\r");
    // start drawing
    IOWR_32DIRECT(HWELLIPSE_0_BASE, RF_START_ADDR, 1);

    // wait for completion
    volatile int result = 0;
    int count = 0;
    do {
        result = IORD_32DIRECT(HWELLIPSE_0_BASE, 0);
        count++;
        usleep(1000);

        if (count > 1000) {
            puts("waited for 1000 iterations");
            count = 0;
            return 0;
        }
    } while(result == 0);
    printf("end ellipse draw\n\r");
    result = IORD_32DIRECT(HWELLIPSE_0_BASE, 4);

    return result;
}
/******hwcursor.c *********************************************************
*Description
* Library to control the hardware cursor accelerator
*********************************************************************/
#include "hwcursor.h"
#include "io.h"
#include "system.h"
#include <stdio.h>
#include <unistd.h>  	// usleep()


void hwcursor_init(int x_size, int y_size)
{
    /**************************************************************************
     * initialise the hwcursor module
     **************************************************************************
     * Parameters
     * x_size : maximum x dimension of the pixel buffer
     * y_size : maximum y dimension of the pixel buffer
     *
     * Return
     * none
     *
     * Side effects
     * update the max_x and max_y value of the hwcurssor accelerator
     *
     * Notes
     *************************************************************************/
    printf("start hw cursor init/n\r");
    // set framebuffer address
    IOWR_32DIRECT(CURSOR_0_BASE, RF_FB_BASE_ADDR, ONCHIP_MEM_BASE);

    // set x_max
    IOWR_32DIRECT(CURSOR_0_BASE, RF_X_MAX_ADDR, x_size);

    // set y_max
    IOWR_32DIRECT(CURSOR_0_BASE, RF_Y_MAX_ADDR, y_size);
    printf("end hw cursor init/n\r");
}

unsigned int hwcursor_draw_cursor(int x, int y){
    /**************************************************************************
     * Draw the cursor at the coordinate x and y
     **************************************************************************
     * Parameters
     * x : x coordinate where to draw the cursor point
     * y : y coordinate where to draw the cursor point
     *
     * Return
     * number of pixel drawn
     *
     * Side effects
     * draw the cursor 
     *
     * Notes
     *************************************************************************/
    // setup cursor parameters
    IOWR_32DIRECT(CURSOR_0_BASE, RF_X_COORDINATE_ADDR, x);
    IOWR_32DIRECT(CURSOR_0_BASE, RF_Y_COORDINATE_ADDR, y);
    IOWR_32DIRECT(CURSOR_0_BASE, RF_COMMAND_ADDR, CMD_DRAW);
    // start drawing
    IOWR_32DIRECT(CURSOR_0_BASE, RF_START_ADDR, 1);

    // wait for completion
    volatile int result = 0;
    int count = 0;
    do {
        result = IORD_32DIRECT(CURSOR_0_BASE, 0);
        count++;
        usleep(1000);

        if (count > 1000) {
            puts("waited for 1000 iterations hw cursor draw\n\r");
            count = 0;
            return 0;
        }
    } while (result == 0);
    printf("end ellipse draw\n\r");
    result = IORD_32DIRECT(CURSOR_0_BASE, 4);

    return result;
}

unsigned int hwcursor_save_cursor(int x, int y){
    /**************************************************************************
    * save value of the pixel in a rectangle of 5x5 
    **************************************************************************
    * Parameters
    * x : x coordinate of the top left corner of the rectangle save window
    * y : y coordinate of the top left corner of the rectangle save window
    *
    * Return
    * number of pixel saved
    *
    * Side effects
    * save the pixel value in an internal memory of the cursor controller
    *
    * Notes
    *************************************************************************/
    // setup cursor parameters
    IOWR_32DIRECT(CURSOR_0_BASE, RF_X_COORDINATE_ADDR, x);
    IOWR_32DIRECT(CURSOR_0_BASE, RF_Y_COORDINATE_ADDR, y);
    IOWR_32DIRECT(CURSOR_0_BASE, RF_COMMAND_ADDR, CMD_SAVE);
    // start drawing
    IOWR_32DIRECT(CURSOR_0_BASE, RF_START_ADDR, 1);

    // wait for completion
    volatile int result = 0;
    int count = 0;
    do {
        result = IORD_32DIRECT(CURSOR_0_BASE, 0);
        count++;
        usleep(1000);

        if (count > 1000) {
            puts("waited for 1000 iterations hw cursor save\n\r");
            count = 0;
            return 0;
        }
    } while (result == 0);
    result = IORD_32DIRECT(CURSOR_0_BASE, 4);

    return result;
}

unsigned int hwcursor_erase_cursor(int x, int y){
    /**************************************************************************
    * erase the cursor at the position x and y 
    **************************************************************************
    * Parameters
    * x : x coordinate of the top left corner of the rectangle save window
    * y : y coordinate of the top left corner of the rectangle save window
    *
    * Return
    * number of pixel saved
    *
    * Side effects
    * erase the cursor at the position x and y with the stored value in 
    * the internal memory from a previous hwcursor_save_cursor call
    *
    * Notes
    *************************************************************************/
    // setup cursor parameters
    IOWR_32DIRECT(CURSOR_0_BASE, RF_X_COORDINATE_ADDR, x);
    IOWR_32DIRECT(CURSOR_0_BASE, RF_Y_COORDINATE_ADDR, y);
    IOWR_32DIRECT(CURSOR_0_BASE, RF_COMMAND_ADDR, CMD_ERASE);
    // start drawing
    IOWR_32DIRECT(CURSOR_0_BASE, RF_START_ADDR, 1);

    // wait for completion
    volatile int result = 0;
    int count = 0;
    do {
        result = IORD_32DIRECT(CURSOR_0_BASE, 0);
        count++;
        usleep(1000);

        if (count > 1000) {
            puts("waited for 1000 iterations hw cursor erase\n\r");
            count = 0;
            return 0;
        }
    } while (result == 0);
    result = IORD_32DIRECT(CURSOR_0_BASE, 4);

    return result;
}
* Description
* Library to draw with software
* ********************************************************************/
#ifndef SOFT_DRAW_H_
#define SOFT_DRAW_H_

void soft_recfiller_init(int x_size, int y_size);

unsigned int soft_recfiller_draw_rectangle(int x_left, int y_top,
    int x_right, int y_bottom,
    int color);

#endif  // SOFT_DRAW_H_

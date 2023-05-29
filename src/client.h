#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>
int sim_lcd_init(int port);

void sim_lcd_release();

int sim_lcd_send_screen(void *buffer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t bits);

#endif // CLIENT_H

#ifndef _TIME_H_
#define _TIME_H_

void timer_handler();

int ticks_elapsed();

int seconds_elapsed();

void wait(int delta);

extern void _hlt();

#endif

#include "system.h"

int timer_ticks = 0;
void timer_handler(struct regs *r)
{
    timer_ticks++; // fun tidbit: uptime has to be over 68 years to rollover this int!
}

void timer_wait(int ticks)
{
    unsigned long eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}

void timer_install()
{
    irq_install_handler(0, timer_handler);
}

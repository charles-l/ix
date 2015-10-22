#include "io.h"

#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5

#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

#define FB_NONE      0
#define FB_GREEN     2
#define FB_DARK_GREY 8

unsigned int cursor_pos;
char *fb;

void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    // i must be even!
    fb[i] = c;
    fb[i + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

void fb_clear()
{
    for(int i = 0; i < 80 * 80; i += 2) {
        fb[i] = '\0';
    }
}

int kmain()
{
    fb = (char *) 0x000B8000;
    fb_clear();
    fb_write_cell(0, 'c', FB_GREEN, FB_NONE);
    fb_write_cell(2, 'o', FB_GREEN, FB_NONE);
    fb_write_cell(4, 's', FB_GREEN, FB_NONE);
    while(1) {
        // kernel loop!!!
    }
    return 0;
}

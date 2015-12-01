#include "io.h"

#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5

#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

#define FB_NONE      0x00
#define FB_GREEN     0x02
#define FB_DARK_GREY 0x08
#define FB_WHITE     0xFF

unsigned short cursor_pos;
char *fb;

void fb_move_cursor(unsigned short pos)
{
    cursor_pos = pos;
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

void putchar(char c)
{
    // MAKE SURE TO WRITE EVERY OTHER BYTE (odd bytes are for styling)!!
    fb_write_cell(cursor_pos * 2, c, FB_WHITE, FB_NONE);
    fb_move_cursor(cursor_pos + 1);
}

void puts(char *s)
{
    for(int i = 0; s[i] != '\0'; i++) {
        putchar(s[i]);
    }
}

int kmain()
{
    fb = (char *) 0x000B8000;
    fb_clear();
    char string[] = "a test";
    puts(string);
    while(1) {
        // kernel loop!!!
    }
    return 0;
}

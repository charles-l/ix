#include "system.h"

unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count)
{
    for(int i = 0; i < count; i++)
    {
        dest[i] = src[i];
    }
    return dest;
}

unsigned char *memset(unsigned char *dest, unsigned char val, int count)
{
    for(int i = 0; i < count; i++)
    {
        dest[i] = val;
    }
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)
{
    for(int i = 0; i < count; i++)
    {
        dest[i] = val;
    }
    return dest;
}

int strlen(const char *str)
{
    int l = 0;
    while(str[l] != '\0') l++;
    return l;
}

unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void kmain()
{
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    __asm__ __volatile__ ("sti");

    // initialize components
    timer_install();
    kb_install();

    // fb
    init_video();

    cls();
    puts("hello world!");

    page_init();

    ///////

    for (;;);
}

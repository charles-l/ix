#ifndef __SYSTEM_H
#define __SYSTEM_H
#include "types.h"

/* This defines what the stack looks like after an ISR was running */
struct regs
{
    uint32 gs, fs, es, ds;      /* pushed the segs last */
    uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    uint32 int_no, err_code;    /* our 'push byte #' and ecodes do this */
    uint32 eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
};

// kmain.c
extern unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);
extern unsigned char *memset(unsigned char *dest, unsigned char val, int count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern int strlen(const char *str);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);

// scrn.c
extern void cls();
extern void putch(unsigned char c);
extern void kputs(unsigned char *str);
extern void settextcolor(unsigned char forecolor, unsigned char backcolor);
extern void init_video();

// gdt.c
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void gdt_install(void);

// idt.c
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void idt_install();

// isrs.c
void isrs_install();

// irq.c
void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_install();
void irq_handler(struct regs *r);

// timer.c
void timer_install();
void timer_wait(int ticks);

// kb.c
void kb_install();

// mem.c
void page_init(void);
uint32 kmalloc   (uint32 sz);
uint32 kmalloc_a (uint32 sz);
uint32 kmalloc_p (uint32 sz, uint32 *phys);
uint32 kmalloc_ap(uint32 sz, uint32 *phys);
void page_init();

#endif

# ix
*TO BE* a minimal unix-like OS.

# Goals
- <s>Implement an entire working (x86) OS in less lines than systemd.</s> Wait. That's not a challenge at all.
- Implement an entire working (x86) OS in less lines than 10% the sloc of systemd. There we go.
    - Current goal for total sloc is 10k (including stdutils)
- Simpler is *always* better.

# Ideas
- Microkernel, cause it seems better?
- Separation of cruft ('cause the world is full of it). If compatablity garbage needs to run, keep it away from everything else.

# What's implemented
- GDT
- Interrupt Handling
    - Timer (PIC)
    - Keyboard
- Paging

Also, there's an embedded Lisp for use later. Mostly because I can.

# Getting Started

- install
    - `bochs`
    - `gcc`
    - `nasm`
    - `genisoimage`
- run `make`
- run `make run`
- type <kbd>c</kbd> to continue

# Code Breakdown

```
.
├── gdt.c       - GDT: tells where memroy segments are
├── idt.c       - IDT: tells where ISRs are at
├── irq.c       - IRQ: interrupt requests
├── isrs.c      - ISR: catch interrupts
├── kb.c        - Keyboard: handle keyboard interrupts
├── kmain.c     - Main file
├── loader.s    - Main ASM file
├── mem.c       - Paging
├── multiboot.h - GRUB multboot headers (will be used later for finding end of physical memory)
├── scrn.c      - Screen functions: drawing, clearing, etc.
├── sl3.c       - Lisp: will be embedded in something later on ;)
├── system.h    - Main header file, maps out kernel functions
├── timer.c     - PIC timer
└── types.h     - type aliases, etc.
```

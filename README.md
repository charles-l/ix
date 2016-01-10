# ix
*TO BE* a minimal unix-like OS.

# Goals
- <s>Implement an entire working (x86) OS in less lines than systemd.</s> Wait. That's not a challenge at all.
- Implement an entire working (x86) OS in less lines than 10% the sloc of systemd. There we go.
    - Current goal for total sloc is 10k (including stdutils)
- Simpler is *always* better.

# Ideas
- A super compatability layer. Basically wine baked into the kernel.
- Microkernel, cause it seems better?
- Separation of cruft ('cause the world is full of it). If compatablity garbage needs to run, keep it away from everything else.

# What's implemented
- GDT
- Interrupt Handling
    - Timer (PIC)
    - Keyboard
- Paging (almost)

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

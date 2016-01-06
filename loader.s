[BITS 32]
global start
start:
    mov esp, _sys_stack     ; This points the stack to our new stack area
    jmp stublet

ALIGN 4
mboot:
    ; Multiboot macros to make a few lines later more readable
    PAGE_ALIGN	equ 1<<0
    MEMORY_INFO	equ 1<<1
    AOUT_KLUDGE	equ 1<<16
    HEADER_MAGIC	equ 0x1BADB002
    HEADER_FLAGS	equ PAGE_ALIGN | MEMORY_INFO | AOUT_KLUDGE
    CHECKSUM	equ -(HEADER_MAGIC + HEADER_FLAGS)
    EXTERN code, bss, end

    ; This is the GRUB Multiboot header. A boot signature
    dd HEADER_MAGIC
    dd HEADER_FLAGS
    dd CHECKSUM

    ; AOUT kludge - must be physical addresses. Make a note of these:
    ; The linker script fills in the data for these ones!
    dd mboot
    dd code
    dd bss
    dd end
    dd start

; This is an endless loop here. Make a note of this: Later on, we
; will insert an 'extern _main', followed by 'call _main', right
; before the 'jmp $'.
stublet:
    extern kmain
    call kmain

global gdt_flush
extern gp
gdt_flush:
    lgdt [gp]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2 ;how do we know 0x08 is the code offset????

flush2:
    ret


; In just a few pages in this tutorial, we will add our Interrupt
; Service Routines (ISRs) right here!



; Here is the definition of our BSS section. Right now, we'll use
; it just to store the stack. Remember that a stack actually grows
; downwards, so we declare the size of the data before declaring
; the identifier '_sys_stack'
SECTION .bss
    resb 8192               ; This reserves 8KBytes of memory here
_sys_stack:

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
    cli
    lgdt [gp]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    ; mov ax, 0x10
    ; mov ds, ax
    ; mov es, ax
    ; mov fs, ax
    ; mov gs, ax
    ; mov ss, ax
    jmp 0x08:flush2 ;how do we know 0x08 is the code offset????

global idt_load
extern idtp
idt_load:
    lidt [idtp]
    ret

flush2:
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; exception handling

%macro dummy_except 1
isr%1:
    cli
    push byte 0  ; push a dummy value to align stack
    push byte %1
    jmp isr_common_stub
%endmacro

%macro code_except 1
isr%1:
    cli
                 ; no dummy value, since it's already pushed
    push byte %1
    jmp isr_common_stub
%endmacro

; TODO: macroify?
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

dummy_except 0
dummy_except 1
dummy_except 2
dummy_except 3
dummy_except 4
dummy_except 5
dummy_except 6
dummy_except 7
code_except  8
dummy_except 9
code_except  10
code_except  11
code_except  12
code_except  13
code_except  14
dummy_except 15
dummy_except 16
dummy_except 17
dummy_except 18
dummy_except 19
dummy_except 20
dummy_except 21
dummy_except 22
dummy_except 23
dummy_except 24
dummy_except 25
dummy_except 26
dummy_except 27
dummy_except 28
dummy_except 29
dummy_except 30
dummy_except 31

extern fault_handler

isr_common_stub:
    pusha
    push ds
    push es
    push gs
    mov ax, 0x10 ; load data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, fault_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; request handlers

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

%macro request_handler 2
irq%1:
    cli
    push byte 0  ; push a dummy value to align stack
    push byte %2
    jmp irq_common_stub
%endmacro

request_handler 0, 32
request_handler 1, 33
request_handler 2, 34
request_handler 3, 35
request_handler 4, 36
request_handler 5, 37
request_handler 6, 38
request_handler 7, 39
request_handler 8, 40
request_handler 9, 41
request_handler 10, 42
request_handler 11, 43
request_handler 12, 44
request_handler 13, 45
request_handler 14, 46
request_handler 15, 47

extern irq_handler

irq_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, irq_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; paging

global load_page_dir
load_page_dir:
    push ebp
    mov ebp, esp
    mov eax, [esp + 8]
    mov cr3, eax
    mov esp, ebp
    pop ebp
    ret

global enable_paging
enable_paging:
    push ebp
    mov ebp, esp
    mov eax, cr0
    or  eax, 0x80000000
    mov cr0, eax
    mov esp, ebp
    pop ebp
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Here is the definition of our BSS section. Right now, we'll use
; it just to store the stack. Remember that a stack actually grows
; downwards, so we declare the size of the data before declaring
; the identifier '_sys_stack'
SECTION .bss
    resb 8192               ; This reserves 8KBytes of memory here
_sys_stack:

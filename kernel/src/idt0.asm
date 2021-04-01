global load_idt
load_idt:
    sti
    mov edx, [esp + 4]
    lidt [edx]
    ret

global keyboard_handler_init
extern keyboard_handler
keyboard_handler_init:
    pushad
    cld
    call keyboard_handler
    popad
    iretd

global time_handler_init
extern time_handler
time_handler_init:
    pushad
    cld
    call time_handler
    popad
    iretd
global put_char_init
extern put_char
put_char_init:
   push	ebp
	mov	ebp, esp

    xchg bx, bx
    push dword [ebp+0x14]
    push dword [ebp+0x10]
    push dword [ebp+0x0c]

    call put_char
    
    add esp,0x0c

    mov esp,ebp
	pop	ebp 
    retf

global sti
sti:
    sti
    ret

global cli
cli:
    cli
    ret

global lgdtr
lgdtr:
    mov edx,[esp+4]
    lgdt [edx]
    ret

global put_char_c
put_char_c:
    push	ebp
	mov	ebp, esp

    xchg bx, bx
    push dword [ebp+0x10]
    push dword [ebp+0x0c]
    push dword [ebp+0x08]

    call far [local]
    
    add esp,0x0c

    mov esp,ebp
	pop	ebp
    ret
local:
    dd 0
    dw 0x18
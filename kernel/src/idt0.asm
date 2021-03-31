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

    push dword [ebp+12]
    push dword [ebp+8]
    push dword [ebp+4]
    call far [local]
    
    add esp,12
    mov esp,ebp
	pop	ebp
    ret
local:
    dw 0,0x18
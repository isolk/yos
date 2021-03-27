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

global load_idt
load_idt:
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
    push ds
    mov eax,0x10
    mov ds,ax
    mov es,ax
    call time_handler
    pop eax
    mov ds,ax
    mov es,ax
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

local:
    dd 0
    dw 0x18

global init_page:
init_page:
    mov eax,0                 ;PCD=PWT=0
    mov cr3,eax

    mov eax,cr0
    or eax,0x80000000
    mov cr0,eax                        ;开启分页机制
    ret

global echo:
echo:
    mov eax, [esp + 4]
    ret

global ltr
ltr:
    mov edx,[esp+4]
    ltr [edx]
    ret

global syscall_handler_init
extern syscall_handler
syscall_handler_init:
    pushad
    cld
    push ds
    push ax
    mov eax,0x10
    mov ds,ax
    mov es,ax

    pop ax
    call syscall_handler
    pop eax
    mov ds,ax
    mov es,ax

    popad
    iretd

global default_handler_init
extern default_handler
default_handler_init:
    pushad
    cld
    call default_handler
    popad
    iretd

global page_handler_init
extern page_handler
page_handler_init:
    mov eax,cr2
    push eax
    call page_handler
    add esp,8
    iretd

global i_hander_13
extern i_hander_13_do
i_hander_13:
    call i_hander_13_do
    add esp,4
    iretd
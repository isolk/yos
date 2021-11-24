global keyboard_handler_wrap
extern keyboard_handler
keyboard_handler_wrap:
    pushad
    cld
    call keyboard_handler
    popad
    iretd

global time_handler_wrap
extern time_handler
time_handler_wrap:
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


global init_page:
init_page:
    mov eax,[esp+4]                 ;PCD=PWT=0
    mov cr3,eax

    mov eax,cr0
    or eax,0x80000000
    mov cr0,eax                        ;开启分页机制
    ret

global syscall_handler_wrap
extern syscall_handler
syscall_handler_wrap:
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

global default_handler_wrap
extern default_handler
default_handler_wrap:
    pushad
    cld
    call default_handler
    popad
    iretd

global page_handler_wrap
extern page_handler
page_handler_wrap:
    mov eax,cr2
    push eax
    call page_handler
    add esp,8
    iretd

global general_handler_wrap
extern general_handler
general_handler_wrap:
    call general_handler
    add esp,4
    iretd
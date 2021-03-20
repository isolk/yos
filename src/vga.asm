global set_cur
set_cur:
    push ax
    push dx
    mov dx,0x3d4
    mov al,0x0e
    out dx,al
    mov dx,0x3d5
    mov al,0x00
    out dx,al

    mov dx,0x3d4
    mov al,0x0f
    out dx,al
    mov dx,0x3d5
    mov al,0x4
    out dx,al

    pop dx
    pop ax
    ret
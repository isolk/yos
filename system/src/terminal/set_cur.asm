global set_cur
set_cur:
    push	ebp
	mov	ebp, esp
    push ax
    push dx

    mov dx,0x3d4
    mov al,0x0e
    out dx,al
    mov dx,0x3d5
    mov al,[ebp+9] ;高8位
    out dx,al

    mov dx,0x3d4
    mov al,0x0f
    out dx,al
    mov dx,0x3d5
    mov al,[ebp+8] ;低8位
    out dx,al

    pop dx
    pop ax
	pop	ebp
    ret
global outb
outb:
    push	ebp
	mov	ebp, esp
    push ax
    push dx

    mov dx,[ebp+8]  ; 端口号
    mov al,[ebp+12] ; 写入值
    out dx,al

    pop dx
    pop ax
	pop	ebp
    ret
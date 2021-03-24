global inb
inb:
    push	ebp
	mov	ebp, esp
    push dx

    mov ax,0
    mov dx,[ebp+8]  ; 端口号
    in al,dx        ; 读入1字节

    pop dx
	pop	ebp
    ret
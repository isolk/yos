global read_port_b
read_port_b:
    push	ebp
	mov	ebp, esp
    push dx

    mov dx,[ebp+8]  ; 端口号
    in al,dx        ; 读入1字节

    pop dx
	pop	ebp
    ret

global read_port_w
read_port_w:
    push	ebp
	mov	ebp, esp
    push dx

    mov dx,[ebp+8]  ; 端口号
    in ax,dx        ; 读入2字节

    pop dx
	pop	ebp
    ret

global write_port_b
write_port_b:
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

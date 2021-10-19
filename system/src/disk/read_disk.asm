global read_disk1 ; void read_disk(uint32_t start_sector,uint32_t target_addr,uint_8 sector);
read_disk1:
    push ebp
	mov	ebp, esp
    pushad

    ; [ebp]=ebp,[ebp+4]=eip,[ebp+8]=start_sector [ebp+12]=target_addr [ebp+16]=sector

    mov dx,0x1f2    
    mov al,[ebp+16]   
    out dx,al       ; 要读取的读取扇区数

    mov dx,0x1f3
    mov al,[ebp+8]
    out dx,al       ; 开始扇区数的0-7位

    mov dx,0x1f4
    mov al,[ebp+9]
    out dx,al       ; 开始扇区数的8-15位

    mov dx,0x1f5
    mov al,[ebp+10]
    out dx,al       ; 开始扇区的16-23位

    mov dx,0x1f6
    mov al,[ebp+11]
    and al,0xEF
    out dx,al       ; lba,master,24-27

    mov dx,0x1f7
    mov al,0x20     ; 0x20表示读
    out dx,al

    mov dx,0x1f7
    .waits:
    in al,dx
    and al,0x88
    cmp al,0x08
    jnz .waits

    mov cx,256
    mov dx,0x1f0
    mov ebx,[ebp+12]
    readw:
    in ax,dx
    mov [ebx],ax ; 把数据写如ebx
    add ebx,2
    loop readw

    popad
	pop	ebp
    ret
global read_elf
read_elf:
    push ebp
    mov ebp,esp
    pushad

    mov ebx,[ebp+8]; // s_addr
    add ebx,0x34
    mov ecx,[ebx+0x10] ; 文件中长度
    mov esi,[ebx+0x04] ; 文件偏移
    add esi,[ebp+8]   ; s_addr

    mov edi,[ebx+0x08] ; 重定位内存地址 
    add edi,[ebp+12]   ; d_addr
    rep movsb 

    add ebx,32
    cld
    mov ecx,[ebx+0x10] ; 文件中长度
    mov esi,[ebx+0x04] ; 文件偏移
    add esi,[ebp+8]   ; s_addr

    mov edi,[ebx+0x08] ; 重定位内存地址 
    add edi,[ebp+12];   d_addr
    rep movsb 

    popad
    pop ebp
    ret
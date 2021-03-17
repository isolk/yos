; 打印loading
mov bx,0xb800
mov ds,bx
mov ah,0x0E
mov bl,0x1

mov al,'l'
int 0x10
mov al,'o'
int 0x10
mov al,'a'
int 0x10
mov al,'d'
int 0x10
mov al,'i'
int 0x10
mov al,'n'
int 0x10
mov al,'g'
int 0x10
mov al,'.'
int 0x10
mov al,'.'
int 0x10
mov al,'.'
int 0x10

; 读取第二扇区,放到 0x7e00处

mov ah,0x02    ; 读取
mov al,1       ; 要读取扇区数
mov ch,0       ; 磁道号
mov cl,2       ; 起始扇区号
mov dh,0       ; 磁头号
mov dl,0x80    ; 驱动号

mov bx,0x7e0
mov es,bx   ; 
mov bx,0       ; es:bx是数据在内存的缓存地址
int 0x13       ; 磁盘中断

jmp 0x7e0:0

jmp $ ;这个指令表示跳转到当前位置，也就是循环跳转当前位置，无限循环，防止cpu继续向下运行。


; times 表示当前指令执行多少次。
; $表示当前指令地址，$$表示起始地址，也就是0。
; db表示高速编译器在当前地址填入一个0
; 这行指令的意思就是当上述指令的长度不足510字节时，剩余的部分填入0
times  510-($-$$) db 0 

db 0x55,0xaa ; 这行的指令的意思是在511和512字节分别填入0x55，0xaa。这是BIOS对bootloader的要求，即512字节的最后两个字节必须是这两个数，以确实是有效的bootloader。

; 1 boot.asm 2-3 loader.asm 4 kernel.c 的第一个扇区
mov ah,0x02    ; 表示读取
mov al,1       ; 要读取扇区数
mov ch,0       ; 磁道号
mov cl,4       ; 起始扇区号
mov dh,0       ; 磁头号
mov dl,0x80    ; 驱动号

mov bx,0x820
mov es,bx      ; 
mov bx,0       ; es:bx是数据在内存的缓存地址
int 0x13       ; 磁盘中断

; 先计算elf文件的总长度
;mov eax,0x20 ; section header postion
;mov bx,0x2e ; size of section headers
;mov cx,0x30 ; number of section headers

mov ebx,[es:0x20]

mov ax,[es:0x2e]
mov dx,[es:0x30]
mul dx   ;dx:ax

push dx
push ax
pop eax
add ebx,eax

push ebx
pop ax
pop dx
mov cx,512
div cx   ; ax,dx -> 商，余数
cmp dx,0 ; 如果余数为0，给商加1
jz cmm   
  inc ax
cmm: cmp ax,0
jz over
  mov ah,0x02    ; 表示读取
  ;mov al,al      ; 要读取扇区数
  mov ch,0       ; 磁道号
  mov cl,5       ; 起始扇区号,当前已经读取了1+2+1=4扇区，所以从第五个扇区开始，注意，扇区号从1开始，不是0
  mov dh,0       ; 磁头号
  mov dl,0x80    ; 驱动号

  mov bx,0x840   ; 
  mov es,bx      ; 
  mov bx,0       ; es:bx是数据在内存的缓存地址
  int 0x13       ; 磁盘中断
over: hlt
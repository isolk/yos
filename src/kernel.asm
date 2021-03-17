mov ah,0x0E
mov bl,0x1

mov al,'o'
int 0x10
mov al,'k'
int 0x10

jmp $

; 读取第三扇区,放到 0x7e00+512处
mov ah,0x02    ; 读取
mov al,10      ; 要读取扇区数
mov ch,0       ; 磁道号
mov cl,3       ; 起始扇区号
mov dh,0       ; 磁头号
mov dl,0x80    ; 驱动号

mov bx,0x800
mov es,bx      ; 0x8000开始 
mov bx,0       ; es:bx是数据在内存的缓存地址
int 0x13       ; 磁盘中

; 读取完毕，解析elf
; - 先找到程序头表位置
; - 加载第一个头表
; - 读取入口地址
; - 跳转
mov ds,bx  ;先把ds设置为elf文件所在内存的地址
;mov bx,0x34 ; Program header头起始位置
mov ax, dword [bx+0x04] ; p_offset，当前段在elf文件中的偏移位置,4位长
;mov cx, dword [bx+0x10] ; P_size，当前段在elf文件中的长度，也就是要读取的长度

jmp 0x8000:[ax]
; 将该端加载到内存中，从0x9000开始
;loop:

; 进入保护模式

; 加载完毕，跳转执行，执行内核程序
;jmp 0x9000:0
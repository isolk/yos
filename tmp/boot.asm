BootSeg equ 0x07c0 ; boot一开始被bios加载到物理地址0x7c00处
SysSeg equ 0x1000  ; head被boot读取出来后，放置的位置-0x10000,但是后面会被移动到0x0处。
SysLen equ 8      ; 内核占用最大的磁盘扇区数

jmp BootSeg:go
go: 
mov	ax,cs
mov ds,ax
mov ss,ax
mov sp,0x400

ReadKernel:
mov ah,0x02    ; 表示读取
mov al,SysLen       ; 要读取扇区数
mov ch,0       ; 磁道号
mov cl,2       ; 起始扇区号
mov dh,0       ; 磁头号
mov dl,0x80    ; 驱动号

mov bx,SysSeg
mov es,bx      ; 
mov bx,0       ; es:bx是数据在内存的缓存地址
int 0x13       ; 磁盘中断

RemoveKernel:
cli
xor si,si
xor di,di      ;si=di=0
mov ax,SysSeg 
mov ds,ax      ; ds=0x1000
xor ax,ax
mov es,ax      ; es=0
mov cx,SysLen*512
rep movsb   ; 将数据从ds：si 复制到  es:di，执行cx次，每次复制1字节

mov ax,cs
mov ds,ax   ;ds=0x7c0

lidt [idt_48]
lgdt [gdt_48]

mov ax,0xb800
mov es,ax

mov byte [es:0],'m'
mov byte [es:1],0x07

hlt

mov ax,0x0001
lmsw ax
jmp 0x0001:0

gdt:
dw 0,0,0,0 ;第一个段

; 第二个段，代码段
dw 0xFFFF
dw 0x0000
dw 0x9A00
dw 0x00cF

; 数据段
dw 0x07FF
dw 0x0000
dw 0x9200
dw 0x00cF

idt_48:
dw 0
dd 0

gdt_48:
dw 0x7ff
dd gdt

times  510-($-$$) db 0 
db 0x55,0xaa
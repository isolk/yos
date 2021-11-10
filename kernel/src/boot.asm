cli
	; 获取内存大小，将其防在内存1M-4处
	xchg bx,bx
	XOR CX, CX
	XOR DX, DX
	MOV AX, 0xE801
	INT 0x15		; request upper memory size
	JC SHORT go 
	CMP AH, 0x86		; unsupported function
	JE SHORT go
	CMP AH, 0x80		; invalid command
	JE SHORT go
	JCXZ USEAX		; was the CX result invalid?
 
	MOV [0x7c00-10], CX
	MOV [0x7c00-20], DX
	xchg bx,bx
USEAX:
	; AX,cx = number of contiguous Kb, 1M to 16M
	; BX,dx = contiguous 64Kb pages above 16M

;boot读取kernel到0x7c00+0x8000处，默认长度不超过16k
;移动kernel到0处
BootSeg equ 0x07c0 ; boot一开始被bios加载到物理地址0x7c00处
SysSeg equ 0x1000  ; head被boot读取出来后，放置的位置-0x10000,但是后面会被移动到0x0处。
SysLen equ 32      ; 内核占用最大的磁盘扇区数

jmp BootSeg:go
go: mov ax,cs
    mov ds,ax
	; mov ss,ax
	; mov sp,0x400

; 读取第2-3扇区,放到 0x7e00处
mov ah,0x02    ; 表示读取
mov al,2       ; 要读取扇区数
mov ch,0       ; 磁道号
mov cl,2       ; 起始扇区号
mov dh,0       ; 磁头号
mov dl,0x80    ; 驱动号

mov bx,0x7e0
mov es,bx      ; 
mov bx,0       ; es:bx是数据在内存的缓存地址
int 0x13       ; 磁盘中断

; cli
; xor si,si
; xor di,di
; mov ax,SysSeg
; mov ds,ax
; xor ax,ax
; mov es,ax
; mov cx,0x1000
; rep movsw   ; 将数据从ds：si 复制到  es:di，执行cx次，每次复制2字节
; sti

; 读取完成，跳转loader.asm继续剩下来的工作
jmp 0x7e0:0

; times 表示当前指令执行多少次。
; $表示当前指令地址，$$表示起始地址，也就是0。
; db表示高速编译器在当前地址填入一个0
; 这行指令的意思就是当上述指令的长度不足510字节时，剩余的部分填入0
times  510-($-$$) db 0 
db 0x55,0xaa ; 这行的指令的意思是在511和512字节分别填入0x55，0xaa。这是BIOS对bootloader的要求，即512字节的最后两个字节必须是这两个数，以确实是有效的bootloader。
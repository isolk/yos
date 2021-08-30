;------------------------------读取内核镜像--------------------------
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
jz read_over
  mov ah,0x02    ; 表示读取
  mov ch,0       ; 磁道号
  mov cl,5       ; 起始扇区号,当前已经读取了1+2+1=4扇区，所以从第五个扇区开始，注意，扇区号从1开始，不是0
  mov dh,0       ; 磁头号
  mov dl,0x80    ; 驱动号

  mov bx,0x840   ; 
  mov es,bx      ; 
  mov bx,0       ; es:bx是数据在内存的缓存地址
  int 0x13       ; 磁盘中断

read_over: 

;mov ax,0xb800
;mov es,ax
;mov byte [es:0],'@'

;---------------------进入保护模式----------------------------

;创建两个GDT段，全局代码段和显存段。
gdt_addr equ 0x6000
mov bx,0
mov es,bx

; 强制要求空段
mov dword [es:gdt_addr],0x00
mov dword [es:gdt_addr+4],0x00

; 代码段,0 4G
mov dword [es:gdt_addr+8],0x00_00_ff_ff
mov dword [es:gdt_addr+12],0x00_cf_98_00

; 数据段 0,4G
mov dword [es:gdt_addr+16],0x00_00_ff_ff
mov dword [es:gdt_addr+20],0x00_cf_92_00

lgdt [0x200+gdt_size]

;mov ax,0x92
;;or al,0000_0010B
;out 0x92,al

mov ax,0x1
lmsw word ax

xchg bx,bx
jmp dword 0x0008:flush+0x200+0x7c00

[bits 32]
flush:
xchg bx,bx
mov cx,0x10
mov es,cx
mov ds,cx
mov ss,cx
mov ebx,0xb8000
mov byte [ebx+0x00],'l'
mov byte [ebx+0x02],'o'
mov byte [ebx+0x04],'d'
mov byte [ebx+0x06],'d'
mov byte [ebx+0x08],'i'
mov byte [ebx+0x0a],'g'


;------------------加载内核代码段----------------------
; 0x34是程序段首地址，4字节
;mov eax,0x18    ; 程序入口地址 在内存中的地址

;mov bx,0x8200+0x34
;mov eax,[bx+0c] ; 内存重定位地址
;mov eax,[bx+04] ; 文件偏移
;mov eax,[bx+14] ; 程序长度
;mov eax,[bx+20] ; 字节对齐

mov bx,0x8200+0x34
cld
mov ecx,[bx+0x10] ; 文件中长度
mov esi,[bx+0x04] ; 文件偏移
add esi,0x8200
mov edi,[bx+0x08] ; 重定位内存地址 
rep movsb 

mov bx,0x8200+0x34+32
cld
mov ecx,[bx+0x10] ; 文件中长度
mov esi,[bx+0x04] ; 文件偏移
add esi,0x8200
mov edi,[bx+0x08] ; 重定位内存地址 
rep movsb 

jmp [0x8200+0x18]

gdt_size dw 23
         dd 0x006000
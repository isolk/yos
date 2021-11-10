;------------------------------读取内核镜像--------------------------
; 1 boot.asm 2-3 loader.asm 4 kernel.c 的第一个扇区
; mov ah,0x02    ; 表示读取
; mov al,1       ; 要读取扇区数
; mov ch,0       ; 磁道号
; mov cl,4       ; 起始扇区号
; mov dh,0       ; 磁头号
; mov dl,0x80    ; 驱动号

; mov bx,0x820
; mov es,bx      ; 
; mov bx,0       ; es:bx是数据在内存的缓存地址
; int 0x13       ; 磁盘中断


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


mov ax,0x1
lmsw word ax

jmp dword 0x0008:flush+0x200+0x7c00

[bits 32]
flush:
mov cx,0x10
mov es,cx
mov ds,cx
mov ss,cx
mov esp,0xc00000 ; 12M -> 内核栈地址 内核 1-11 

mov ax,3
mov ebx,0x1400000 ; 20M -> image从磁盘读取时缓存地址
call read_disk

mov ebx,[0x1400000+0x20] ; program header posstion
mov ax,[0x1400000+0x2e] ; number of program header
mov dx,[0x1400000+0x30]
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
jnz cmm   
  inc ax
cmm:

mov cx,ax
mov ax,3
mov ebx,0x1400000

read:
inc ax
add ebx,0x200
push ax
push ebx
call read_disk
pop ebx
pop ax
loop read


;------------------加载内核代码段----------------------
; 0x34是程序段首地址，4字节
;mov eax,0x18    ; 程序入口地址 在内存中的地址

;mov bx,0x8200+0x34
;mov eax,[bx+0c] ; 内存重定位地址
;mov eax,[bx+04] ; 文件偏移
;mov eax,[bx+14] ; 程序长度
;mov eax,[bx+20] ; 字节对齐

mov ebx,0x1400000+0x34
cld
mov ecx,[ebx+0x10] ; 文件中长度
mov esi,[ebx+0x04] ; 文件偏移
add esi,0x1400000
mov edi,[ebx+0x08] ; 重定位内存地址 
rep movsb 

mov ebx,0x1400000+0x34+32
cld
mov ecx,[ebx+0x10] ; 文件中长度
mov esi,[ebx+0x04] ; 文件偏移
add esi,0x1400000
mov edi,[ebx+0x08] ; 重定位内存地址 
rep movsb 

; 将内核的结束位置放在固定内存里供内核使用
mov eax,[ebx+0x08]
add eax,[ebx+0x14]
mov [0x7c00-30],eax

jmp [0x1400000+0x18]
hlt

read_disk:
push dx
push cx

push ax
push ebx

mov dx,0x1f3
out dx,al       ; start，低8位

mov dx,0x1f4
mov al,ah
out dx,al       ; 8-15

mov dx,0x1f2
mov al,1
out dx,al       ; 读取扇区数

mov dx,0x1f5
mov al,0
out dx,al       ; 16-23

mov dx,0x1f6
mov al,0xe0
out dx,al       ; lba,master,24-27

mov dx,0x1f7
mov al,0x20
out dx,al

mov dx,0x1f7
.waits:
in al,dx
and al,0x88
cmp al,0x08
jz go
jmp .waits
go:
pop ebx
mov cx,256
mov dx,0x1f0

cmp ebx,0xfe00
jnz readw

readw:
in ax,dx
mov [ebx],ax
add ebx,2
loop readw

pop ax
pop cx
pop dx
ret

gdt_size dw 23
         dd 0x006000
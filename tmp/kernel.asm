VgaAddr equ 0xb800
VgaColorBlack equ 0x07
SelectorKernelCode equ 0x08
SelectorKernelData equ 0x10
SelectorKernelVga equ 0x18

call setup_idt
call setup_gdt
mov eax,SelectorKernelCode
mov ds,ax
mov es,ax
mov fs,ax
mov gs,ax
sti

;------------------函数区------------------
setup_gdt:
lgdt [gdt]
ret

setup_idt:
push ecx
push di
push eax
push ebx

mov cx,256
mov di,0

mov eax,0x008E0000
mov ebx,0x00080000
mov bx,common_idt
j:
mov [idt+di],eax
add di,4
mov [idt+di],ebx
dec cx
jne j
lidt [idt_48]

pop ebx
pop eax
pop di
pop ecx
ret

common_idt:
push edx
push ds

mov edx,SelectorKernelData
mov ds,edx
mov al,'c'
call print_char

pop ds
pop edx
iret

print_char:
push gs
push edx

mov edx,SelectorKernelVga
mov gs,edx
mov [gs:vga_pos],al
inc dword [vga_pos]
mov byte [gs:vga_pos],VgaColorBlack
inc dword [vga_pos]

pop edx
pop gs
ret
;------------------数据区------------------
gdt: ;gdt表
dw 0,0,0,0 ;null
dw 0xFFFF,0x0000,0x9A00,0x00CF ;内核代码段 0x08
dw 0xFFFF,0x0000,0x9200,0x00CF ;内核数据段 0x10
dw 0x0002,0x8000,0x920b,0x00C0 ;显示内存段 0x18
end_gdt:

gdt_48:
dw end_gdt
dd gdt

idt:
times  256*8-($-$$) db 0 

idt_48:
dw 256*8-1
dd idt

vga_pos:
dd 0

; 进入保护模式，创建两个GDT段，全局代码段和显存段。
gdt_addr equ 0x7e00

; 强制要求空段
mov dword [gdt_addr],0x00
mov dword [gdt_addr+4],0x00

; 代码段,0 4G
mov dword [gdt_addr+8],0x00_00_ff_ff
mov dword [gdt_addr+12],0x00_cf_98_00

; 显存段 0xb8000,7ffff
mov dword [gdt_addr+16],0x80_00_7f_ff
mov dword [gdt_addr+20],0x00_40_92_0b

lgdt [0x7c00+gdt_size]

in al,0x92
or al,0000_0010B
out 0x92,al

cli

mov eax,cr0
or eax,1
mov cr0,eax

jmp dword 0x0008:flush+0x7c00

[bits 32]
flush:
mov cx,00000000_000_10_000b
mov ds,cx
mov byte [0x00],'y'
mov byte [0x02],'h'
mov byte [0x04],'t'
mov byte [0x06],'d'
mov byte [0x08],'e'
mov byte [0x0a],'v'

hlt

gdt_size dw 23
         dd 0x007e00

times 510-($-$$) db 0
db 0x55,0xaa
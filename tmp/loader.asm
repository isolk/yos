; 打印loading
mov bx,0xb800
mov ds,bx
mov ah,0x0E
mov bl,0x1

mov al,'l'
int 0x10

hlt
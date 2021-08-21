rm bin/disk.im
#mkfile -n 1M bin/disk.img
dd if=/dev/urandom of=bin/disk.img bs=1M count=1
nasm boot.asm -o boot.o
nasm kernel.asm -o kernel.o
dd bs=512 count=1 if=boot.o of=bin/disk.img seek=0 conv=notrunc
dd bs=512 count=8 if=kernel.o of=bin/disk.img seek=1 conv=notrunc
#qemu-system-i386 -drive file=bin/disk.img,format=raw,index=0,media=disk
bochs -qf bochs.cfg
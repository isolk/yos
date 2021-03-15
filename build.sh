rm bochs/disk.img
rm src/boot.o

mkfile -n 1M bochs/disk.img
nasm src/boot.asm -o src/boot.o
dd bs=512 count=1 if=src/boot.o of=bochs/disk.img seek=0 conv=notrunc
bochs -qf bochs/bochs.cfg
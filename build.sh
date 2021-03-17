rm bochs/disk.img
rm src/boot.o

mkfile -n 1M bochs/disk.img
nasm src/boot.asm -o src/boot.o
nasm src/kernel.asm -o src/kernel.o

dd bs=512 count=1 if=src/boot.o of=bochs/disk.img seek=0 conv=notrunc
dd bs=512 count=1 if=src/kernel.o of=bochs/disk.img seek=1 conv=notrunc
bochs -qf bochs/bochs.cfg
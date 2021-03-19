mkfile -n 1M bochs/disk.img

nasm src/boot.asm -o obj/boot.o
nasm src/loader.asm -o obj/loader.o

i686-elf-gcc -c src/kernel.c -o obj/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -T obj/linker.ld -o obj/yos.bin -ffreestanding -O2 -nostdlib -lgcc obj/kernel.o

dd bs=512 count=1 if=obj/boot.o of=bochs/disk.img seek=0 conv=notrunc
dd bs=512 count=2 if=obj/loader.o of=bochs/disk.img seek=1 conv=notrunc
dd bs=512 count=50 if=obj/yos.bin of=bochs/disk.img seek=3 conv=notrunc
bochs -qf bochs/bochs.cfg
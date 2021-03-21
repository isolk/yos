cc = i686-elf-gcc
CFLAGS = -c -std=gnu99 -ffreestanding -O2 -Wall -Wextra  -I include -I libc/include

SRC_C = $(wildcard src/*.c)
OBJ_C=$(patsubst %.c,obj/%.o,$(notdir $(SRC_C)))

SRC_ASM = $(wildcard src/*.asm)
OBJ_ASM=$(patsubst %.asm,obj/%.o,$(notdir $(SRC_ASM)))

bochs/disk.img: obj/yos.bin obj/boot.bin obj/loader.bin	
	@mkfile -n 1M bochs/disk.img
	@dd bs=512 count=1 if=obj/boot.bin of=bochs/disk.img seek=0 conv=notrunc >/dev/null 2>&1
	@dd bs=512 count=2 if=obj/loader.bin of=bochs/disk.img seek=1 conv=notrunc >/dev/null 2>&1
	@dd bs=512 count=50 if=obj/yos.bin of=bochs/disk.img seek=3 conv=notrunc >/dev/null 2>&1

obj/yos.bin: $(OBJ_C) $(OBJ_ASM)
	@i686-elf-gcc -T obj/linker.ld -o obj/yos.bin -ffreestanding -O2 -nostdlib $^ -lgcc -Llibc -ltest 
obj/boot.bin: src/boot/boot.asm
	@nasm -o obj/boot.bin src/boot/boot.asm
obj/loader.bin: src/boot/loader.asm
	@nasm -o obj/loader.bin src/boot/loader.asm
obj/%.o : src/%.c
	@i686-elf-gcc $(CFLAGS) -o $@ $^
obj/%.o : src/%.asm
	@nasm -felf32 -o $@ $^
run:
	@bochs -qf bochs/bochs.cfg
clean:
	@rm obj/*.o obj/*.bin rm bochs/disk.img
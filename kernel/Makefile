# LIBSTD=$(HOME)/yos/libc
LIBSYS=$(HOME)/yos/system
cc = i686-elf-gcc
CFLAGS = -g -c -masm=intel -std=gnu99 -ffreestanding -O0 -Wall -Wextra  -I include -I $(LIBSYS)/include 
SRC_C = $(wildcard src/*.c)
OBJ_C=$(patsubst %.c,obj/%.o,$(notdir $(SRC_C)))

SRC_ASM = $(wildcard src/*.asm)
OBJ_ASM=$(patsubst %.asm,obj/%.o,$(notdir $(SRC_ASM)))

bin/disk.img: obj/yos.bin obj/boot.bin obj/loader.bin
	@mkdir -p bin
	@truncate -s 1M bin/disk.img
	@dd bs=512 count=1 if=obj/boot.bin of=bin/disk.img seek=0 conv=notrunc >/dev/null 2>&1
	@dd bs=512 count=2 if=obj/loader.bin of=bin/disk.img seek=1 conv=notrunc >/dev/null 2>&1
	@dd bs=512 count=993 if=obj/yos.bin of=bin/disk.img seek=3 conv=notrunc >/dev/null 2>&1
	@dd bs=512 count=128 if=../user/hello of=bin/disk.img seek=1000 conv=notrunc >/dev/null 2>&1

obj/yos.bin: $(OBJ_C) $(OBJ_ASM)
	i686-elf-gcc -g -T obj/linker.ld -o obj/yos.bin -ffreestanding -O0 -nostdlib $^ -lgcc -L $(LIBSYS) -lsys
obj/boot.bin: src/boot.asm
	@nasm -o obj/boot.bin src/boot.asm
obj/loader.bin: src/loader.asm
	@nasm -o obj/loader.bin src/loader.asm
obj/%.o : src/%.c
	@i686-elf-gcc $(CFLAGS) -o $@ $^
obj/%.o : src/%.asm
	@nasm -g -f elf32 -F DWARF -o $@ $^

clean:
	@rm -rf bin
	@rm -f obj/*.o obj/*.bin

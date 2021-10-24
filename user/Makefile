LIBSTD=$(HOME)/yos/libc
LIBSYS=$(HOME)/yos/system
cc = i686-elf-gcc
CFLAGS = -g -c -std=gnu99 -ffreestanding -O0 -Wall -Wextra  -I include -I $(LIBSTD)/include
hello:hello.o
	i686-elf-gcc $(CFLAGS) -o hello.o hello.c
	i686-elf-gcc -g -T linker.ld -o hello -ffreestanding -O0 -nostdlib $^ -lgcc -L $(LIBSTD) -lstd  -L $(LIBSYS) -lsys
clean:
	rm hello
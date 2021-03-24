CC = i686-elf-gcc
CFLAGS = -g -c -std=gnu99 -ffreestanding -O2 -Wall -Wextra  -I include

SRC_C = $(wildcard src/*/*.c)
OBJ_C=$(patsubst src/%.c,lib/%.o,$(SRC_C))
DEP_C=$(OBJ_C:.o=.d)
DEP_DIR = $(dir $(DEP_C))

SRC_S = $(wildcard src/*/*.asm)
OBJ_S=$(patsubst src/%.asm,lib/%.o,$(SRC_S))
DEP_SDIR = $(dir $(OBJ_S))

$(shell mkdir -p $(DEP_DIR))
$(shell mkdir -p $(DEP_SDIR))

libsys.a: $(OBJ_C) $(OBJ_S)
	@i686-elf-ar src $@ $^ 
lib/%.o: src/%.c
	@$(CC) -MD -c $< -o $@ $(CFLAGS)
lib/%.o: src/%.asm
	@nasm -felf32 -o $@ $<

clean:
	@rm -f libsys.a
	@rm -rf lib

-include $(DEP_C)
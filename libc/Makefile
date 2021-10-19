CC = i686-elf-gcc
CFLAGS = -g -c -std=gnu99 -ffreestanding -O1 -Wall -Wextra  -I include

SRC_C = $(wildcard src/*/*.c)

OBJ_C=$(patsubst src/%.c,lib/%.o,$(SRC_C))
DEP_C=$(OBJ_C:.o=.d)
DEP_DIR = $(dir $(DEP_C))

$(shell mkdir -p $(DEP_DIR))

libstd.a: $(OBJ_C)
	@i686-elf-ar src $@ $^ 
lib/%.o: src/%.c
	@$(CC) -MD -c $< -o $@ $(CFLAGS)

clean:
	@rm -f libstd.a
	@rm -rf lib

-include $(DEP_C)
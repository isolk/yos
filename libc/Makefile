CC = i686-elf-gcc
CFLAGS = -c -std=gnu99 -ffreestanding -O2 -Wall -Wextra  -I include

SRC_C = $(wildcard src/*/*.c)

OBJ_C=$(patsubst src/%.c,lib/%.o,$(SRC_C))
DEP_C=$(OBJ_C:.o=.d)

libstd.a: $(OBJ_C)
	echo $(SRC_C)
#@i686-elf-ar src $@ $^ 
%.o: %.c

#$(CC) -MD -c $< -o $@ $(CFLAGS)

clean:
	@rm -f libstd.a
	@rm -rf lib/*

#$(shell mkdir -p $(DEPDIR))
-include $(DEP_C)
disk:bochs/disk.img

clean:
	@make -C libc clean
	@make -C kernel clean
	@rm -f bochs/disk.img

bochs/disk.img: 
	@make -C libc
	@make -C kernel
	@cp kernel/bin/disk.img bochs/disk.img
make -C libc
make -C system 
make -C kernel
rsync -u kernel/bin/disk.img bochs/disk.img
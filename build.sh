make -C libc
make -C kernel
rsync -u kernel/bin/disk.img bochs/disk.img
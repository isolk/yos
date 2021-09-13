set -e
make -C libc
make -C system 
make -C kernel
cp kernel/bin/disk.img bochs/disk.img
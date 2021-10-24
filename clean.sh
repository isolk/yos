make -C libc clean
make -C kernel clean
make -C system clean
make -C user clean
rm -f bochs/disk.img
rm -f t.g
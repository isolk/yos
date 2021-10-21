set -e
make -C libc
make -C system 
make -C user
make -C kernel
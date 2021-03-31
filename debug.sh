OS=`uname`
if [ ${OS} = "Darwin" ]
then
echo "not surppoted in MacOS"
exit
fi

echo "target remote localhost:1234" > t.g
echo 'add-symbol-file k.sym 0x100000' >> t.g
i686-elf-objcopy --only-keep-debug kernel/obj/yos.bin k.sym
qemu-system-i386 -s -S -drive file=bochs/disk.img,format=raw,index=0,media=disk&
gdb -x t.g
rm t.g

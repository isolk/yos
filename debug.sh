# OS=`uname`
# if [ ${OS} = "Darwin" ]
# then
# echo "not surppoted in MacOS"
# exit
# fi

# echo "target remote localhost:1234" > t.g
# echo 'add-symbol-file k.sym 0x100000' >> t.g
# i686-elf-objcopy --only-keep-debug kernel/obj/yos.bin k.sym
# bochs -qf bochs/bochs.cfg
echo "ok"
qemu-system-i386 -no-reboot -no-shutdown -s -S -drive file=kernel/bin/disk.img,format=raw,index=0,media=disk
# gdb -x t.g -tui
# gdb -tui
# rm t.g
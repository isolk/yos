# YOS 自动化测试

本目录包含YOS内核的自动化测试脚本。

## 目录结构

```
tests/
├── gdb_scripts/       # GDB测试脚本
│   └── check_boot.gdb # 启动测试脚本
├── run_test.sh        # 主测试运行器
└── README.md          # 本文件
```

## 快速开始

### 运行所有测试

```bash
./tests/run_test.sh
```

### 运行单个GDB测试

```bash
# 启动QEMU（终端1）
qemu-system-i386 -no-reboot -no-shutdown -s -S \
  -drive file=kernel/bin/disk.img,format=raw -nographic

# 运行GDB测试（终端2）
i386-elf-gdb -batch -x tests/gdb_scripts/check_boot.gdb kernel/obj/yos.bin
```

## 测试内容

### check_boot.gdb

检查以下内容：
1. ✅ Bootloader是否在0x7c00执行
2. ✅ Loader是否在0x7e00执行
3. ✅ Kernel _start()函数是否被调用
4. ✅ 内存管理是否初始化
5. ✅ VGA内存是否被写入

## 编写新的测试

在`tests/gdb_scripts/`目录下创建新的`.gdb`文件：

```gdb
# my_test.gdb
target remote localhost:1234
set architecture i386

# 设置断点
break function_name
commands
  printf "Test point reached\n"
  # 检查状态
  printf "EAX: 0x%x\n", $eax
  continue
end

# 继续执行
continue
```

## 日志文件

测试运行后会生成以下日志：
- `/tmp/build.log` - 构建日志
- `/tmp/qemu.log` - QEMU运行日志
- `/tmp/gdb_test.log` - GDB测试日志

## 添加串口输出（推荐）

为了更好的CLI调试体验，建议添加串口输出支持：

1. 在`kernel/src/`创建`serial.c`
2. 修改`system/src/terminal/print_char.c`添加串口输出
3. 运行测试：
   ```bash
   qemu-system-i386 -drive file=kernel/bin/disk.img,format=raw \
     -nographic -serial stdio
   ```

详细的串口实现方案见项目主文档。

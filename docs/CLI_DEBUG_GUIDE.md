# YOS CLI自动化调试指南

## 概述

本项目现在支持通过CLI进行自动化测试和调试，无需图形界面。

## 快速开始

### 运行自动化测试

```bash
./tests/run_test.sh
```

这将自动：
1. 清理旧的构建
2. 构建内核
3. 启动QEMU（后台模式）
4. 运行GDB测试脚本
5. 生成测试报告
6. 清理QEMU进程

### 测试结果示例

```
=========================================
  YOS Automated Test Suite
=========================================

[1/5] Cleaning old build...
[2/5] Building kernel...
✓ Build successful
[3/5] Starting QEMU...
✓ QEMU started
[4/5] Running GDB tests...
✓ Test passed: tests/gdb_scripts/check_boot.gdb
[5/5] Cleanup...
✓ QEMU stopped

=========================================
  Test Summary
=========================================
Passed: 1
Failed: 0

✅ All tests passed!
```

## 目录结构

```
tests/
├── gdb_scripts/
│   └── check_boot.gdb    # 启动测试脚本
├── run_test.sh            # 主测试运行器
└── README.md              # 测试文档
```

## GDB测试脚本说明

### check_boot.gdb

该脚本测试以下内容：

1. **Bootloader测试** - 验证代码是否在0x7c00执行
2. **Loader测试** - 验证代码是否在0x7e00执行
3. **内核入口测试** - 验证_start()函数被调用
4. **内存管理测试** - 验证init_mem()被调用
5. **VGA内存测试** - 验证VGA显存被写入

## 手动调试方法

### 方法1: GDB调试

```bash
# 终端1: 启动QEMU
qemu-system-i386 -no-reboot -no-shutdown -s -S \
  -drive file=kernel/bin/disk.img,format=raw -nographic

# 终端2: 连接GDB
i386-elf-gdb kernel/obj/yos.bin
(gdb) target remote localhost:1234
(gdb) break _start
(gdb) continue
```

### 方法2: GDB批处理模式

```bash
# 运行GDB脚本
i386-elf-gdb -batch -x tests/gdb_scripts/check_boot.gdb kernel/obj/yos.bin
```

### 方法3: 检查VGA内存

```gdb
# 连接到QEMU后
target remote localhost:1234
set architecture i386

# 查看VGA显存前200个字符
x/200x 0xc00fb8000

# 或以字符格式查看
x/200c 0xc00fb8000
```

## 编写自定义测试

### 创建新的GDB测试

在`tests/gdb_scripts/`下创建新文件，例如`test_memory.gdb`：

```gdb
# 连接到QEMU
target remote localhost:1234
set architecture i386

# 设置测试变量
set $tests_passed = 0
set $tests_failed = 0

# 测试1: 检查函数调用
printf "\n=== Test: Memory Allocation ===\n"
break kalloc
commands
  printf "✓ kalloc called\n"
  printf "  Size requested: 0x%x\n", $eax
  set $tests_passed = $tests_passed + 1
  continue
end

# 继续执行
continue

# 等待一段时间
shell sleep 3

# 测试总结
printf "\n=== Summary ===\n"
printf "Passed: %d\n", $tests_passed
printf "Failed: %d\n", $tests_failed
```

### 添加到测试套件

新创建的`.gdb`文件会自动被`run_test.sh`发现并运行。

## CI/CD集成

### GitHub Actions示例

```yaml
name: YOS Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: macos-latest
    steps:
      - uses: actions/checkout@v2
      - name: Install dependencies
        run: brew install qemu i386-elf-gcc
      - name: Run tests
        run: ./tests/run_test.sh
```

## 进阶：添加串口输出

为了获得更好的调试体验，建议添加串口输出支持。

### 步骤1: 创建串口驱动

在`kernel/src/serial.c`:

```c
#include <stdint.h>

#define SERIAL_COM1 0x3F8

void serial_init() {
    outb(SERIAL_COM1 + 1, 0x00);    // 禁用中断
    outb(SERIAL_COM1 + 3, 0x80);    // 启用DLAB
    outb(SERIAL_COM1, 0x03);         // 波特率38400
    outb(SERIAL_COM1 + 1, 0x00);
    outb(SERIAL_COM1 + 3, 0x03);     // 8N1
    outb(SERIAL_COM1 + 2, 0xC7);     // 启用FIFO
    outb(SERIAL_COM1 + 4, 0x03);     // 启用RTS/DSR
}

int serial_is_transmit_ready() {
    return inb(SERIAL_COM1 + 5) & 0x20;
}

void serial_write_char(char c) {
    while (!serial_is_transmit_ready());
    outb(SERIAL_COM1, c);
}

void serial_write_string(char* str) {
    while (*str) {
        serial_write_char(*str++);
    }
}
```

### 步骤2: 修改print_char

在`system/src/terminal/print_char.c`添加串口输出：

```c
void print_char(uint8_t str) {
    // 原有代码...

    // 添加串口输出
    serial_write_char(str);
}
```

### 步骤3: 使用串口输出

```bash
# 输出到终端
qemu-system-i386 -drive file=kernel/bin/disk.img,format=raw \
  -nographic -serial stdio

# 输出到文件
qemu-system-i386 -drive file=kernel/bin/disk.img,format=raw \
  -nographic -serial file:/tmp/kernel.log
```

## 调试技巧

### 1. 查看当前执行位置

```gdb
info registers eip cs ds
x/10i $pc
```

### 2. 查看栈跟踪

```gdb
bt
info frame
```

### 3. 检查内存内容

```gdb
# 查看物理内存映射
x/50x 0xc0100000

# 查看页目录
x/100x 0xc01e0000
```

### 4. 设置条件断点

```gdb
break process_schedule if $eax == 1001
```

## 常见问题

### Q: 测试超时怎么办？
A: 增加sleep时间或调整断点位置。

### Q: 如何查看详细的GDB输出？
A: 查看`/tmp/gdb_test.log`文件。

### Q: VGA内存为什么是0xc00fb8000？
A: 这是VGA显存(0xb8000)在内核虚拟地址空间中的映射地址(3GB + 0xb8000)。

## 性能基准测试

```bash
# 测量启动时间
time qemu-system-i386 -drive file=kernel/bin/disk.img,format=raw \
  -nographic -serial file:/tmp/boot.log
```

## 相关文档

- [QEMU文档](https://www.qemu.org/docs/master/)
- [GDB文档](https://www.sourceware.org/gdb/current/onlinedocs/gdb/)
- [OSDev Wiki](https://wiki.osdev.org/)

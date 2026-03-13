# YOS QEMU 启动脚本使用指南

## 概述

`run_qemu.sh` 是一个功能强大的QEMU启动脚本，支持多种输出模式和调试选项。

## 快速开始

```bash
# 赋予执行权限（首次使用）
chmod +x run_qemu.sh

# VGA图形界面模式（默认）
./run_qemu.sh

# 查看帮助
./run_qemu.sh --help
```

## 命令行选项

### 输出模式

| 选项 | 说明 | 示例 |
|------|------|------|
| `-m vga` | VGA图形界面 | `./run_qemu.sh -m vga` |
| `-m nographic` | 无图形，串口到终端 | `./run_qemu.sh -m nographic` |
| `-m serial` | VGA + 串口到文件 | `./run_qemu.sh -m serial` |
| `-m auto` | 自动选择模式 | `./run_qemu.sh -m auto` |

### 串口输出

| 选项 | 说明 | 示例 |
|------|------|------|
| `-s FILE` | 串口输出到文件 | `./run_qemu.sh -s /tmp/kernel.log` |

### GDB调试

| 选项 | 说明 | 示例 |
|------|------|------|
| `-g` | 启用GDB（默认端口1234） | `./run_qemu.sh -g` |
| `-g PORT` | 指定GDB端口 | `./run_qemu.sh -g 2345` |

### 其他选项

| 选项 | 说明 | 示例 |
|------|------|------|
| `-n` | 无图形模式 | `./run_qemu.sh -n` |
| `-M SIZE` | 设置内存大小(MB) | `./run_qemu.sh -M 128` |
| `-k` | 杀死所有QEMU进程 | `./run_qemu.sh -k` |
| `-h` | 显示帮助信息 | `./run_qemu.sh -h` |

## 快捷模式

```bash
# VGA图形界面
./run_qemu.sh vga
# 或
./run_qemu.sh

# 命令行模式（无图形）
./run_qemu.sh cli
# 或
./run_qemu.sh -n

# 测试模式（串口到文件）
./run_qemu.sh test

# 调试模式（GDB）
./run_qemu.sh debug
```

## 使用场景

### 1. 日常开发（VGA图形界面）

```bash
./run_qemu.sh
```

图形窗口显示内核输出，适合开发调试。

### 2. 自动化测试（串口到文件）

```bash
./run_qemu.sh test

# 等待5秒
sleep 5

# 检查输出
cat /tmp/kernel_test.log | grep "Serial port initialized"
```

### 3. CI/CD集成

```bash
# 运行并捕获输出
./run_qemu.sh -n -s /tmp/output.log &
QEMU_PID=$!

# 等待内核初始化
sleep 3

# 验证输出
if grep -q "Kernel started" /tmp/output.log; then
    echo "✅ 测试通过"
else
    echo "❌ 测试失败"
fi

# 清理
kill $QEMU_PID
```

### 4. GDB调试

```bash
# 终端1：启动QEMU
./run_qemu.sh debug

# 终端2：连接GDB
i386-elf-gdb kernel/obj/yos.bin
(gdb) target remote localhost:1234
(gdb) break _start
(gdb) continue
```

### 5. 双重输出（VGA + 串口日志）

```bash
# VGA显示，同时保存日志
./run_qemu.sh -s /tmp/kernel.log

# 查看日志
tail -f /tmp/kernel.log
```

### 6. 性能测试

```bash
# 测试启动时间
time ./run_qemu.sh -n

# 或使用自定义内存大小
./run_qemu.sh -M 32
```

## 高级用法

### 组合选项

```bash
# GDB调试 + 串口输出 + 128MB内存
./run_qemu.sh -g -s /tmp/debug.log -M 128

# 无图形 + 串口到文件 + 杀死旧进程
./run_qemu.sh -k -n -s /tmp/test.log
```

### 与其他工具集成

```bash
# 与valgrind集成（检查内存）
valgrind --leak-check=full ./run_qemu.sh -n

# 与strace集成（系统调用跟踪）
strace -o trace.log ./run_qemu.sh -n

# 记录执行时间
./run_qemu.sh -n 2>&1 | tee run.log
```

### 自动化脚本示例

```bash
#!/bin/bash
# run_tests.sh

echo "=== YOS 自动化测试套件 ==="

# 测试1: 基本启动
echo "[1/3] 基本启动测试..."
./run_qemu.sh -n -s /tmp/test1.log &
PID=$!
sleep 3
kill $PID 2>/dev/null

if grep -q "Serial port initialized" /tmp/test1.log; then
    echo "✅ 通过"
else
    echo "❌ 失败"
    exit 1
fi

# 测试2: 内存管理
echo "[2/3] 内存管理测试..."
./run_qemu.sh -n -s /tmp/test2.log &
PID=$!
sleep 3
kill $PID 2>/dev/null

if grep -q "init_mem" /tmp/test2.log; then
    echo "✅ 通过"
else
    echo "❌ 失败"
    exit 1
fi

# 测试3: 进程调度
echo "[3/3] 进程调度测试..."
./run_qemu.sh -n -s /tmp/test3.log &
PID=$!
sleep 3
kill $PID 2>/dev/null

if grep -q "schedule" /tmp/test3.log; then
    echo "✅ 通过"
else
    echo "❌ 失败"
    exit 1
fi

echo ""
echo "=== 所有测试通过 ==="
```

## 故障排除

### QEMU无法启动

```bash
# 检查disk.img是否存在
ls -lh kernel/bin/disk.img

# 重新构建
./build.sh
```

### 端口被占用

```bash
# 查看1234端口占用
lsof -i :1234

# 杀死进程
./run_qemu.sh -k
```

### 串口无输出

```bash
# 检查串口日志文件权限
ls -l /tmp/kernel.log

# 使用绝对路径
./run_qemu.sh -s /tmp/kernel.log
```

### GDB连接失败

```bash
# 确保QEMU在GDB模式下运行
./run_qemu.sh debug

# 在另一个终端连接
i386-elf-gdb kernel/obj/yos.bin
(gdb) target remote localhost:1234
```

## 环境变量

你可以通过环境变量设置默认值：

```bash
# 默认内存大小
export QEMU_MEM_SIZE=128

# 默认串口输出文件
export QEMU_SERIAL_LOG=/tmp/kernel.log

# 默认GDB端口
export QEMU_GDB_PORT=2345
```

## 与其他脚本的关系

| 脚本 | 用途 | 推荐场景 |
|------|------|----------|
| `run_qemu.sh` | 手动启动和调试 | 开发、调试 |
| `debug.sh` | GDB调试 | 深度调试 |
| `tests/run_test.sh` | 自动化测试 | CI/CD |
| `quick_debug.sh` | 快速调试 | 快速测试 |

## 贡献

如果你想改进这个脚本：

1. Fork项目
2. 创建分支: `git checkout -b improve-qemu-script`
3. 提交更改: `git commit -am "Add new feature"`
4. 推送分支: `git push origin improve-qemu-script`
5. 创建Pull Request

## 许可

MIT License

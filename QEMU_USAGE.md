# QEMU 启动脚本 - 快速参考

## 🚀 快速开始

```bash
# 赋予执行权限
chmod +x run_qemu.sh

# VGA图形界面（默认）
./run_qemu.sh

# 查看帮助
./run_qemu.sh -h
```

## 📋 常用命令

| 场景 | 命令 |
|------|------|
| 🖥️ 开发调试 | `./run_qemu.sh` |
| 💻 命令行模式 | `./run_qemu.sh -n` |
| 🧪 自动化测试 | `./run_qemu.sh test` |
| 🐛 GDB调试 | `./run_qemu.sh debug` |
| 📝 保存日志 | `./run_qemu.sh -s /tmp/kernel.log` |
| 🛑 停止所有 | `./run_qemu.sh -k` |

## 🔧 选项详解

```bash
-m MODE          # vga, nographic, serial, auto
-s FILE          # 串口输出文件
-g [PORT]        # GDB调试模式 (默认1234)
-n               # 无图形模式
-M SIZE          # 内存大小(MB)
-k               # 杀死所有QEMU
-h               # 帮助信息
```

## 💡 使用示例

### 日常开发
```bash
./run_qemu.sh
```

### 测试串口输出
```bash
./run_qemu.sh -s /tmp/test.log
cat /tmp/test.log
```

### GDB调试
```bash
# Terminal 1
./run_qemu.sh debug

# Terminal 2
i386-elf-gdb kernel/obj/yos.bin
(gdb) target remote localhost:1234
```

### 自动化测试
```bash
./run_qemu.sh -n -s /tmp/test.log &
PID=$!
sleep 3
kill $PID

# 检查输出
grep "success" /tmp/test.log && echo "✅ 通过"
```

### 自定义配置
```bash
# 128MB内存 + GDB + 日志
./run_qemu.sh -M 128 -g -s /tmp/debug.log
```

## ⚡ 快捷模式

```bash
./run_qemu.sh vga      # VGA图形界面
./run_qemu.sh cli      # 命令行模式
./run_qemu.sh test     # 测试模式
./run_qemu.sh debug    # 调试模式
```

## 📚 相关文档

- [详细使用指南](docs/QEMU_RUNNER_GUIDE.md)
- [串口输出原理](docs/SERIAL_OUTPUT_PRINCIPLES.md)
- [CLI调试指南](docs/CLI_DEBUG_GUIDE.md)

## 🆘 故障排除

| 问题 | 解决方案 |
|------|----------|
| QEMU无法启动 | 检查 `kernel/bin/disk.img` 是否存在 |
| 端口被占用 | 运行 `./run_qemu.sh -k` |
| 串口无输出 | 检查文件权限 |
| GDB连接失败 | 确保使用 `debug` 模式 |

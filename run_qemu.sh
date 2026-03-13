#!/bin/bash
#
# YOS QEMU 启动脚本
# 支持多种输出模式和调试选项
#

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 默认配置
DISPLAY_MODE="vga"        # vga, nographic, serial
SERIAL_OUTPUT=""          # 串口输出文件路径
GDB_DEBUG=false           # 是否启用GDB
GDB_PORT=1234             # GDB端口
QEMU_PID=""               # QEMU进程ID
DISK_IMG="kernel/bin/disk.img"
MEM_SIZE=64               # 内存大小(MB)

# 显示帮助信息
show_help() {
    cat << EOF
${GREEN}YOS QEMU 启动脚本${NC}

${YELLOW}用法:${NC}
    $0 [选项]

${YELLOW}选项:${NC}
    -m, --mode MODE          输出模式:
                             ${BLUE}vga${NC}        - VGA图形界面 (默认)
                             ${BLUE}nographic${NC}  - 无图形，串口输出到stdio
                             ${BLUE}serial${NC}     - VGA + 串口输出到文件
                             ${BLUE}auto${NC}       - 自动选择最佳模式

    -s, --serial FILE        串口输出到文件
                             示例: -s /tmp/kernel.log

    -g, --gdb [PORT]         启用GDB调试模式
                             可选指定端口 (默认: 1234)

    -n, --nographic          无图形模式 (等同于 -m nographic)

    -M, --mem SIZE           内存大小 (MB) [默认: 64]

    -k, --kill               杀死所有正在运行的QEMU进程

    -h, --help               显示此帮助信息

${YELLOW}示例:${NC}
    # VGA图形界面
    $0

    # 无图形，串口输出到终端
    $0 -n

    # VGA图形界面 + 串口输出到文件
    $0 -s /tmp/kernel.log

    # 无图形 + 串口输出到文件
    $0 -n -s /tmp/kernel.log

    # 启用GDB调试
    $0 -g

    # GDB调试 + 串口输出到文件
    $0 -g -s /tmp/kernel.log

    # 使用32MB内存
    $0 -M 32

${YELLOW}快捷模式:${NC}
    $0 vga                    # VGA图形界面
    $0 cli                    # 命令行模式
    $0 test                   # 测试模式 (串口到文件)
    $0 debug                  # 调试模式 (GDB)

EOF
}

# 打印信息
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 检查disk.img是否存在
check_disk_image() {
    if [ ! -f "$DISK_IMG" ]; then
        print_error "磁盘镜像不存在: $DISK_IMG"
        print_info "请先运行 ./build.sh 构建内核"
        exit 1
    fi
}

# 清理QEMU进程
cleanup() {
    if [ -n "$QEMU_PID" ]; then
        print_info "停止QEMU (PID: $QEMU_PID)"
        kill $QEMU_PID 2>/dev/null || true
        wait $QEMU_PID 2>/dev/null || true
    fi
}

# 捕获中断信号
trap cleanup EXIT INT TERM

# 杀死所有QEMU进程
kill_all_qemu() {
    print_info "杀死所有QEMU进程..."
    pkill -9 qemu-system-i386 2>/dev/null && print_success "已杀死所有QEMU进程" || print_warning "没有运行中的QEMU进程"
    exit 0
}

# 构建QEMU命令
build_qemu_command() {
    local cmd="qemu-system-i386"
    local opts=()

    # 基本选项
    opts+=("-no-reboot")
    opts+=("-no-shutdown")
    opts+=("-drive")
    opts+=("file=$DISK_IMG,format=raw,index=0,media=disk")
    opts+=("-m")
    opts+=("$MEM_SIZE")

    # 根据模式添加选项
    case "$DISPLAY_MODE" in
        vga)
            # VGA图形模式，不做特殊处理
            ;;
        nographic)
            opts+=("-nographic")
            if [ -n "$SERIAL_OUTPUT" ]; then
                opts+=("-serial")
                opts+=("file:$SERIAL_OUTPUT")
            else
                opts+=("-serial")
                opts+=("stdio")
            fi
            ;;
        serial)
            # VGA + 串口到文件
            if [ -z "$SERIAL_OUTPUT" ]; then
                SERIAL_OUTPUT="/tmp/kernel_serial.log"
                print_warning "未指定串口输出文件，使用默认: $SERIAL_OUTPUT"
            fi
            opts+=("-serial")
            opts+=("file:$SERIAL_OUTPUT")
            ;;
        auto)
            # 自动检测：有终端则用nographic，否则用VGA
            if [ -t 0 ]; then
                print_info "检测到终端，使用nographic模式"
                DISPLAY_MODE="nographic"
                opts+=("-nographic")
                opts+=("-serial")
                opts+=("stdio")
            else
                print_info "未检测到终端，使用VGA模式"
                DISPLAY_MODE="vga"
            fi
            ;;
        *)
            print_error "未知模式: $DISPLAY_MODE"
            exit 1
            ;;
    esac

    # GDB调试模式
    if [ "$GDB_DEBUG" = true ]; then
        opts+=("-s")
        opts+=("-S")
        print_info "GDB服务器将在端口 $GDB_PORT 启动"
        print_info "使用以下命令连接: i386-elf-gdb kernel/obj/yos.bin"
        print_info "GDB命令: target remote localhost:$GDB_PORT"
    fi

    # 组合命令
    echo "$cmd ${opts[@]}"
}

# 启动QEMU
start_qemu() {
    check_disk_image

    local cmd=$(build_qemu_command)

    print_info "启动QEMU..."
    print_info "模式: $DISPLAY_MODE"
    if [ -n "$SERIAL_OUTPUT" ]; then
        print_info "串口输出: $SERIAL_OUTPUT"
    fi
    echo ""

    # 启动QEMU
    if [ "$GDB_DEBUG" = true ]; then
        # GDB模式：后台运行
        eval $cmd &
        QEMU_PID=$!
        print_success "QEMU已启动 (PID: $QEMU_PID)"
        print_info "等待GDB连接..."
        print_info "按Ctrl+C停止"

        # 保持运行
        wait $QEMU_PID
    else
        # 非GDB模式：前台运行
        eval $cmd
    fi
}

# 解析参数
while [[ $# -gt 0 ]]; do
    case $1 in
        -m|--mode)
            DISPLAY_MODE="$2"
            shift 2
            ;;
        -s|--serial)
            SERIAL_OUTPUT="$2"
            shift 2
            ;;
        -g|--gdb)
            GDB_DEBUG=true
            if [[ "$2" =~ ^[0-9]+$ ]]; then
                GDB_PORT="$2"
                shift 2
            else
                shift
            fi
            ;;
        -n|--nographic)
            DISPLAY_MODE="nographic"
            shift
            ;;
        -M|--mem)
            MEM_SIZE="$2"
            shift 2
            ;;
        -k|--kill)
            kill_all_qemu
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        vga)
            DISPLAY_MODE="vga"
            shift
            ;;
        cli|nographic)
            DISPLAY_MODE="nographic"
            shift
            ;;
        test)
            DISPLAY_MODE="nographic"
            SERIAL_OUTPUT="/tmp/kernel_test.log"
            print_info "测试模式：串口输出到 $SERIAL_OUTPUT"
            shift
            ;;
        debug)
            GDB_DEBUG=true
            DISPLAY_MODE="nographic"
            shift
            ;;
        *)
            print_error "未知选项: $1"
            show_help
            exit 1
            ;;
    esac
done

# 主流程
main() {
    if [ "$DISPLAY_MODE" = "vga" ] && [ "$GDB_DEBUG" = false ]; then
        print_info "模式: VGA图形界面"
        print_info "提示: 使用 -h 查看所有选项"
    fi

    start_qemu
}

main

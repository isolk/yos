#!/bin/bash
# YOS 自动化测试脚本
# 用法: ./tests/run_test.sh

set -e  # 遇到错误退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "========================================="
echo "  YOS Automated Test Suite"
echo "========================================="
echo ""

# 1. 清理旧的构建
echo -e "${YELLOW}[1/5] Cleaning old build...${NC}"
./clean.sh > /dev/null 2>&1 || true

# 2. 构建内核
echo -e "${YELLOW}[2/5] Building kernel...${NC}"
if ! ./build.sh 2>&1 | tee /tmp/build.log; then
    echo -e "${RED}✗ Build failed${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Build successful${NC}"

# 3. 启动QEMU
echo -e "${YELLOW}[3/5] Starting QEMU...${NC}"
qemu-system-i386 -no-reboot -no-shutdown -s -S \
    -drive file=kernel/bin/disk.img,format=raw,index=0,media=disk \
    -nographic \
    > /tmp/qemu.log 2>&1 &

QEMU_PID=$!
echo "QEMU PID: $QEMU_PID"

# 等待QEMU启动
sleep 2

# 检查QEMU是否还在运行
if ! kill -0 $QEMU_PID 2>/dev/null; then
    echo -e "${RED}✗ QEMU failed to start${NC}"
    cat /tmp/qemu.log
    exit 1
fi
echo -e "${GREEN}✓ QEMU started${NC}"

# 4. 运行GDB测试
echo -e "${YELLOW}[4/5] Running GDB tests...${NC}"

TEST_PASSED=0
TEST_FAILED=0

for gdb_script in tests/gdb_scripts/*.gdb; do
    if [ -f "$gdb_script" ]; then
        echo ""
        echo "Running: $gdb_script"
        echo "----------------------------------------"

        if i386-elf-gdb -batch -x "$gdb_script" kernel/obj/yos.bin 2>&1 | tee /tmp/gdb_test.log; then
            TEST_PASSED=$((TEST_PASSED + 1))
            echo -e "${GREEN}✓ Test passed: $gdb_script${NC}"
        else
            TEST_FAILED=$((TEST_FAILED + 1))
            echo -e "${RED}✗ Test failed: $gdb_script${NC}"
        fi
    fi
done

# 5. 清理
echo ""
echo -e "${YELLOW}[5/5] Cleanup...${NC}"
kill $QEMU_PID 2>/dev/null || true
wait $QEMU_PID 2>/dev/null || true
echo -e "${GREEN}✓ QEMU stopped${NC}"

# 测试总结
echo ""
echo "========================================="
echo "  Test Summary"
echo "========================================="
echo -e "Passed: ${GREEN}$TEST_PASSED${NC}"
echo -e "Failed: ${RED}$TEST_FAILED${NC}"
echo ""

if [ $TEST_FAILED -eq 0 ]; then
    echo -e "${GREEN}✅ All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed${NC}"
    echo ""
    echo "Check logs:"
    echo "  - Build:    /tmp/build.log"
    echo "  - QEMU:     /tmp/qemu.log"
    echo "  - GDB test: /tmp/gdb_test.log"
    exit 1
fi

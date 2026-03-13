# YOS Boot Test GDB Script
# 用法: i386-elf-gdb -batch -x tests/gdb_scripts/check_boot.gdb kernel/obj/yos.bin

# 连接到QEMU GDB服务器
target remote localhost:1234

# 设置架构
set architecture i386

# 定义测试变量
set $tests_passed = 0
set $tests_failed = 0

# 测试1: 检查bootloader是否执行
printf "\n=== Test 1: Bootloader ===\n"
break *0x7c00
commands
  printf "✓ Bootloader entry point reached\n"
  set $tests_passed = $tests_passed + 1
  continue
end

# 测试2: 检查loader是否执行
printf "\n=== Test 2: Loader ===\n"
break *0x7e00
commands
  printf "✓ Loader entry point reached\n"
  set $tests_passed = $tests_passed + 1
  continue
end

# 测试3: 检查内核是否启动
printf "\n=== Test 3: Kernel Entry ===\n"
break _start
commands
  printf "✓ Kernel _start() reached\n"
  printf "  EIP: 0x%x\n", $pc
  printf "  ESP: 0x%x\n", $esp
  set $tests_passed = $tests_passed + 1
  continue
end

# 测试4: 检查内存管理初始化
printf "\n=== Test 4: Memory Management ===\n"
break init_mem
commands
  printf "✓ Memory initialization started\n"
  set $tests_passed = $tests_passed + 1
  continue
end

# 等待5秒让kernel初始化
printf "\n=== Waiting for kernel initialization...\n"
shell sleep 5

# 测试5: 检查VGA内存内容
printf "\n=== Test 5: VGA Memory Check ===\n"
set $vga_base = 0xc00fb8000
set $char_count = 0
set $row = 0
while $row < 25
  set $col = 0
  while $col < 80
    set $char = *(uint8_t*)($vga_base + ($row * 80 + $col) * 2)
    if $char != 0 && $char != 32
      set $char_count = $char_count + 1
    end
    set $col = $col + 1
  end
  set $row = $row + 1
end

if $char_count > 0
  printf "✓ VGA memory contains %d characters\n", $char_count
  set $tests_passed = $tests_passed + 1
else
  printf "✗ VGA memory appears empty\n"
  set $tests_failed = $tests_failed + 1
end

# 显示前100个VGA字符
printf "\n=== First 100 VGA characters ===\n"
set $i = 0
while $i < 100
  set $char = *(uint8_t*)($vga_base + $i * 2)
  if $char >= 32 && $char <= 126
    printf "%c", $char
  else
    printf "."
  end
  set $i = $i + 1
end
printf "\n"

# 测试总结
printf "\n=== Test Summary ===\n"
printf "Passed: %d\n", $tests_passed
printf "Failed: %d\n", $tests_failed

if $tests_failed == 0
  printf "\n✅ All tests passed!\n"
else
  printf "\n❌ Some tests failed\n"
end

# 继续执行
continue

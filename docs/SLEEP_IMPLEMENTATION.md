**Sleep 功能实现说明**

**概述**
- 目标：为用户态程序提供按毫秒休眠的能力，使 `user/hello.c` 能实现“每秒打印一次 i++”。
- 思路：在内核添加基于时钟 tick 的计数（jiffies），在用户态暴露毫秒接口 `ysleep(ms)`，内核将 ms 换算为 tick 并在内核中等待或阻塞后唤醒。

**本次主要改动文件**
- `kernel/src/time.c` — 增加 `timer_ticks`、`timer_get_ticks()`、`timer_ms_to_ticks()`；修正 RTC 初始化，启用 periodic interrupt；在 `time_handler()` 中递增 tick 并触发唤醒逻辑。
- `kernel/include/time.h` — 新增计时相关声明。
- `kernel/include/process.h` — 为 `task_struct` 增加 `sleep_until_tick` 字段，并声明 `process_sleep_ms()` 和 `process_wake_sleeping()`。
- `kernel/src/process.c` — 实现 `process_sleep_ms()`（当前实现为内核内同步等待以保证可用性）、`process_wake_sleeping()`，并改进调度选择可运行任务的逻辑。
- `kernel/src/sys_call.c` — 将 syscall 3 映射到 sleep，读取用户传入参数并调用 `process_sleep_ms()`。
- `system/include/ystd.h` — 添加 `ysleep` 声明与 `sleep` 内联别名。
- `system/src/ystd/ysleep.c` — 新增用户态封装，通过 `int $0x80` 触发 syscall 3。
- `user/hello.c` — 示例改为循环打印 i 并调用 `sleep(1000)`。

（另外有少量辅助修改：`kernel/src/idle.c`、`kernel/src/string.c`、Makefile 相关构建未改动主要逻辑。）

**实现原理（简要）**
1. 时钟与 tick
   - 在 `init_time()` 中正确设置 RTC 寄存器以启用 periodic interrupt，并维护 `timer_ticks`（也称 jiffies）。
   - `time_handler()` 每次中断时增加 `timer_ticks`，并调用唤醒检查 `process_wake_sleeping()`，最后调用 `process_schedule()` 进行任务调度。

2. 毫秒到 tick 的换算
   - 内核假设定时器频率为 `TIMER_HZ`（当前实现值为 1024Hz），并提供 `timer_ms_to_ticks(ms)` 做向上取整换算：ticks = ceil(ms * TIMER_HZ / 1000)。

3. 系统调用与用户态
   - 用户态通过 `ysleep(ms)`（`system/src/ystd/ysleep.c`）将参数放到 `ebx`，并执行 `int $0x80`（syscall 3）。
   - 内核 `syscall_handler()` 在接收 `num==3` 时，调用 `process_sleep_ms(pa1)`。

4. 睡眠/唤醒的执行策略（当前实现）
   - 为尽快交付功能，当前的 `process_sleep_ms()` 在内核中以同步方式等待目标 tick：先计算截止 tick，然后开启中断（`sti`），进入 `hlt` 循环直到到期，再 `cli`。
   - `process_wake_sleeping(current_tick)` 在每个 tick 中遍历进程链表，将到期的进程标记为 `running`（该实现假定进程数量少，遍历开销可接受）。
   - 说明：已实现的方案能满足单用户/教育场景下的按秒打印需求；后续可改为真正的阻塞式（将进程从运行轮转中移出并维护睡眠队列）。

**如何构建与验证**
1. 全量重建（推荐，避免 Makefile 旧目标未重编译）：
```sh
./clean.sh
./build.sh
```
2. 运行并把串口重定向到文件查看输出：
```sh
rm -f /tmp/yos_sleep.log
./run_qemu.sh -n -s /tmp/yos_sleep.log
# 等待若干秒，然后查看日志
tail -n +1 /tmp/yos_sleep.log | sed -n '1,200p'
```
预期：日志中应看到 `i=0`、随后每秒一行 `i=1,i=2...`。

**已知限制与注意事项**
- RTC 粒度与频率：当前 `TIMER_HZ` 设为 1024Hz，ms -> tick 换算会有 ~1/TIMER_HZ 的粒度误差（约 1ms），实际硬件/RTC 设置可能不同，精度受限。
- 当前 `process_sleep_ms()` 实现为内核内同步等待（`hlt` 循环），这意味着在多任务场景下需要改为阻塞式以避免浪费 CPU 或影响公平调度。
- Makefile 依赖较弱，建议在调试内核时使用 `./clean.sh` + `./build.sh` 保证镜像为最新源码构建结果。

**后续改进建议**
1. 把 `process_sleep_ms()` 改为真正阻塞式：
   - 为 `task_struct` 增加 sleep 队列指针，移出运行轮转链表并按 deadline 入队。
   - 在 `time_handler()` 中按到期将进程移回运行队列并唤醒。
2. 优化睡眠队列数据结构：使用按 deadline 排序的链表或小顶堆以减少唤醒检查开销。
3. 将 sleep syscall 扩展为更 POSIX 兼容的 `sleep()/usleep()/nanosleep()` 接口，并处理信号中断等语义。

文件位置：请参阅上面列出的源文件路径，文档放置于 `docs/SLEEP_IMPLEMENTATION.md`。

作者注：该实现为教学/POC 目的，优先保证可观察性与可验证性，生产级实现需做更多边界检查与并发安全处理。

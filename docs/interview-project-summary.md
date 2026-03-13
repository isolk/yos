# YOS 项目面试讲解文档

## 1. 这个项目是做什么的

`YOS` 是一个基于 `x86-32` 的研究型 mini OS 内核项目，目标是用 C + 汇编从零实现操作系统关键链路，验证自己对操作系统原理的理解。

一句话可讲：
这是一个我自己从 0 手写的类 Linux 教学内核，打通了“启动 -> 保护模式 -> 分页 -> 中断 -> 调度 -> 用户态程序 + 系统调用”这条最小可运行链路。

---

## 2. 总体架构（面试官最关心）

项目按层次分成 4 部分：

1. `boot/loader`（汇编）
- `kernel/src/boot.asm`：BIOS 启动后先执行，读取 loader。
- `kernel/src/loader.asm`：进入保护模式、开启分页、加载 ELF 内核并跳转入口。

2. `kernel`（内核核心）
- 内存分配：`kernel/src/pm.c`
- 分页管理：`kernel/src/page.c`
- 描述符表：`kernel/src/gdt.c`、`kernel/src/idt.c`、`kernel/src/ldt.c`、`kernel/src/tss.c`
- 中断/时钟/键盘：`kernel/src/interrupt.c`、`kernel/src/time.c`、`kernel/src/keyboard.c`
- 进程与调度：`kernel/src/process.c`
- 系统调用：`kernel/src/sys_call.c`

3. `system`（系统基础库）
- 端口 I/O、终端输出、内存操作、`sbrk/brk`、磁盘读取与 `yexit` 等 syscall 封装。

4. `libc + user`
- `libc` 提供字符串和简化版 `malloc`。
- `user/hello.c` 是用户态 ELF 程序，通过 `int 0x80` 触发系统调用退出。

---

## 3. 核心实现原理（按启动顺序讲）

### 3.1 启动与模式切换

1. BIOS 加载 MBR（`boot.asm`）到 `0x7c00`。
2. `boot.asm` 使用 BIOS 中断读取 `loader.asm`，并跳转执行。
3. `loader.asm` 做三件事：
- 构造最小 GDT，打开保护模式（`lmsw` + 远跳转）。
- 从磁盘按扇区把内核 ELF 读入内存，按 Program Header 重定位。
- 构造页目录/页表，写 `cr3`，置 `cr0.PG` 开启分页，然后跳到内核入口。

关键点：
通过“先实模式读盘，再保护模式建页表”的两阶段启动，完成从 BIOS 环境到 C 内核环境的切换。

### 3.2 内存管理

1. 内核堆分配（`pm.c`）
- 维护 `ktrunk` 链式块（`used + byte_size`）。
- `kalloc(size)`：首次适配 + split。
- `kfree(ptr)`：回收后做相邻块合并（`gc`）。

2. 页帧分配（`kalloc_frame`）
- 在空闲大块中找到满足 `N * 4KB` 且对齐可用的区域。
- 用 `split_frame` 拆成页对齐区间，返回物理页地址。

3. 分页映射（`page.c`）
- 初始化时建立页目录 + 大量页表。
- 同时覆盖低地址映射与高地址内核区映射，保证内核虚拟地址空间可访问。

### 3.3 中断与系统调用

1. `IDT` 初始化（`idt.c` + `interrupt.c`）
- 先给 256 项装默认处理函数。
- 再挂载键盘、时钟、缺页、系统调用等入口。

2. PIC 与外设
- 通过端口编程初始化 8259 PIC。
- 时钟中断里触发 `process_schedule()`，形成抢占式切换入口。

3. syscall 机制
- 用户态通过 `int 0x80` 进入 `syscall_handler`。
- 目前实现了：
  - `ax=1`：字符输出
  - `ax=2`：进程退出（调用 `exit_process`）

### 3.4 进程与调度

1. 进程模型（`task_struct`）
- 维护 PID、页目录、TSS、双向循环链表节点。

2. 初始化流程（`process.c::start`）
- 创建 idle 内核任务（PID 0）。
- 从磁盘固定位置读取用户 ELF（`seek=1000`）。
- 分配并映射用户地址空间，拷贝 ELF 段。
- 创建用户态 TSS 并装入 GDT。

3. 调度策略
- 双向循环链表轮转。
- 在时钟中断中触发 `process_schedule`，切换到下一个任务。
- 通过切换不同 TSS 描述符并 `jmp` 到 TSS selector 实现任务切换。

---

## 4. 一次完整运行链路（可直接讲）

1. `build.sh` 分别编译 `libc/system/user/kernel`，最终把 `boot + loader + kernel + user hello` 写入磁盘镜像。
2. QEMU 启动后执行 bootloader，切到保护模式并开分页。
3. 进入 `kernel.c::_start`，初始化内存、分页、GDT/LDT/IDT、中断。
4. `start()` 创建 idle 和 user 任务，调度到用户程序。
5. 用户程序打印 `user` 后调用 `yexit()`，触发 `int 0x80`，内核回收并继续调度。

---

## 5. 项目亮点（面试建议这样说）

1. 从零实现了完整“可运行内核最小闭环”，不是只做单点实验。
2. 亲手打通了 x86 关键机制：GDT/IDT/TSS/LDT、分页、PIC、中断、ring 切换。
3. 支持用户态 ELF 加载和最小系统调用，验证了内核态与用户态边界。
4. 代码不追求工程化完美，目标是验证原理，适合展示底层理解和问题拆解能力。

---

## 6. 已知边界与改进方向（主动说更加分）

1. 内存分配器是教学版，碎片控制和性能都较基础。
2. 进程调度是简单轮转，缺少优先级、公平性和阻塞队列。
3. 系统调用数量少，参数校验与安全隔离较弱。
4. 文件系统、VFS、网络栈尚未完整实现。

可扩展计划：
- 引入 slab/buddy；
- 补全 syscall 表与用户态库；
- 做更标准的 ELF 装载和进程地址空间管理；
- 增加基础文件系统与设备驱动框架。

---

## 7. 面试话术模板（1 分钟）

我这个项目是一个研究型 mini OS，核心目标是从 0 手写一个 x86-32 内核，把操作系统最关键的链路跑通。  
启动时先由 bootloader 在实模式读盘，再由 loader 进入保护模式、开启分页并装载 ELF 内核。进入内核后，我实现了内存分配、页表映射、GDT/IDT/TSS、PIC 中断、时钟驱动调度，以及用户态程序加载和 `int 0x80` 系统调用。  
虽然它不是工业级内核，但它完整覆盖了操作系统最核心的机制，能证明我对底层执行模型、特权级切换和内核抽象的真实掌握。

---

## 8. 可补充的演示点（如果面试官追问）

1. 展示 `boot.asm -> loader.asm -> kernel.c` 的调用链。
2. 解释一次 `int 0x80` 的寄存器传参与返回路径。
3. 解释时钟中断如何触发任务切换（从 IRQ 到 TSS 跳转）。
4. 说明内核地址空间映射策略（低地址映射 + 高地址内核区）。

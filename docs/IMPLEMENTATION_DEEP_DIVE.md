# YOS 实现细节讲解稿（可直接对外讲）

本文按“可讲解性”组织：你可以从启动讲到中断、调度、系统调用，再讲 C/汇编联合构建。

## 1. 从上电到内核 C 入口

### 1.1 boot.asm（实模式第一阶段）
- BIOS 把 MBR 装载到 0x7c00，执行 boot.asm。
- boot.asm 通过 BIOS 中断读取 loader 到内存并跳转。
- 同时写入部分内存大小信息，供后续内核初始化使用。

### 1.2 loader.asm（实模式到保护模式过渡）
- 手工构建最小 GDT（空段、代码段、数据段）。
- `lmsw` 打开保护模式，远跳转刷新流水线。
- 在保护模式下读取内核 ELF 镜像到内存。
- 按 Program Header 将段拷贝到目标虚拟地址对应位置。
- 构建页目录/页表，设置 CR3，置 CR0.PG 开分页。
- 跳转到内核入口 `_start`。

要点：
- loader 把“读盘、模式切换、分页开启”三件事一次完成，是整个系统最关键桥梁。

## 2. C 与汇编联合构建是怎么完成的

## 2.1 目标文件生成
- kernel/Makefile 中：
  - C 文件由 i686-elf-gcc 编译为 ELF32 目标文件。
  - ASM 文件由 nasm 生成 ELF32 目标文件。
- 二者最终由链接器脚本 kernel/obj/linker.ld 统一链接成 yos.bin。

## 2.2 链接地址与运行地址
- 链接脚本把内核放在高地址（约 3G+）段布局。
- loader 在早期阶段建立对应页表映射，保证高地址代码可执行。

## 2.3 中断入口“汇编封装 + C 处理函数”模式
- idt0.asm 提供 wrapper：
  - 保存寄存器（pushad）
  - 修正段寄存器（如 ds/es）
  - 调用 C 函数（time_handler/syscall_handler/page_handler）
  - 恢复现场并 iretd 返回
- 这种模式避免 C 代码直接处理繁琐的中断现场细节，结构清晰。

## 3. 描述符体系：GDT / LDT / TSS / IDT

## 3.1 GDT（全局段描述符）
- 包含内核代码段、内核数据段，及用于任务切换的 TSS 描述符位。
- install_gdt_kernel_tss / install_gdt_user_tss 会把当前任务 TSS 写入 GDT 项。

## 3.2 LDT（局部描述符）
- ldt.c 初始化用户代码段、用户数据段。
- 用户态 TSS 使用 LDT selector，使用户任务在 ring3 段环境下运行。

## 3.3 TSS（任务状态段）
- tss.c 中分别构建内核任务与用户任务 TSS。
- 用户任务设置 ss0/esp0，确保 ring3 -> ring0 中断进入时有内核栈切换。

## 3.4 IDT（中断描述符表）
- idt.c 维护 256 项 IDT 表项。
- interrupt.c 先全量装默认处理器，再覆盖关键中断：
  - 14: 页错误
  - 0x21: 键盘
  - 0x28: RTC 时钟
  - 0x80: 系统调用（DPL=3）

## 4. 中断系统实现详解

## 4.1 PIC 初始化与 IRQ 映射
- init_pic 通过端口 0x20/0x21/0xA0/0xA1 初始化 8259。
- 键盘和时钟中断被映射到 IDT 指定向量。

## 4.2 time_handler（调度触发点）
- 每次中断：
  - 读取 RTC 寄存器 C 进行应答
  - tick 自增
  - 唤醒到期任务
  - 发送 EOI
  - 调用 process_schedule
- 这让调度具备“被时钟抢占驱动”的基本形态。

## 4.3 page fault 与通用异常
- page_handler 接收 CR2 与错误码。
- 当前以打印/调试为主，尚未实现完整的按需分页或进程级异常恢复策略。

## 5. 进程与调度实现细节

## 5.1 task_struct 与进程组织
- process.h 定义 task_struct：PID、页目录、TSS、状态、双向循环链表。
- 任务状态包含 running/blocked/exit。

## 5.2 start() 初始化第一个用户进程
- 创建 idle 内核任务（PID 0）。
- 从磁盘扇区 1000 读入 user/hello ELF。
- 解析 ELF，申请并映射用户地址空间，拷贝段。
- 创建用户态 TSS（含 ring0 栈），安装到 GDT。
- 进入调度器切换到可运行任务。

## 5.3 调度策略
- 核心是 round-robin，扫描循环链表找下一个 running 任务。
- 通过安装不同 TSS 描述符并远跳转完成任务切换。
- 退出任务会在调度时从链表删除。

## 6. 系统调用链路

## 6.1 用户态发起
- system/src/ystd/ysleep.c 与 yexit.c 使用内联汇编：
  - 约定 AX 放 syscall 编号，BX/EBX 放参数
  - 执行 `int 0x80`

## 6.2 内核态分发
- sys_call.c 读取寄存器参数并分发：
  - 1: print_char
  - 2: exit_process
  - 3: process_sleep_ms

## 6.3 sleep 当前实现形态
- process_sleep_ms 目前是“计算截止 tick 后在内核循环 hlt 等待到期”。
- 其优点是简单直观、验证功能快；缺点是还不是经典阻塞队列式睡眠。

## 7. 内存与分页实现

## 7.1 pm.c 内核分配器
- 用 ktrunk 链式元数据管理可用内存。
- 支持 kalloc/kfree 和简单合并 gc。
- kalloc_frame 提供页帧级分配，满足分页映射需求。

## 7.2 page.c 映射策略
- 初始化页目录与页表。
- 同时建立用户区与内核高地址区映射，支持内核高地址执行。

## 8. 你可以这样给同学讲（10分钟提纲）

1. 先讲链路：BIOS -> boot -> loader -> kernel。
2. 再讲“为什么要 GDT/IDT/TSS/LDT”：特权级、异常入口、任务切换。
3. 再讲“中断是怎么落到 C 函数的”：idt0.asm wrapper。
4. 再讲“时钟如何触发调度”：time_handler -> process_schedule。
5. 最后讲“用户态怎么调用内核”：int 0x80 + syscall 分发。

## 9. 当前实现中的教学价值与局限

- 教学价值
  - 覆盖 x86 内核最核心的硬件接口与执行模型。
  - 链路完整，可调试、可观察、可扩展。
- 局限
  - 调度和睡眠机制仍偏原型化。
  - 文件系统/网络/线程未形成完整子系统。
  - 错误处理与安全检查仍需加强。

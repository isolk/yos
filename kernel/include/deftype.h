// 局部描述符
struct ld
{
};
struct ld ldt[256];

// 中断描述符
struct id
{
};
struct idt idt[256];

// 全局描述符
struct gd
{
};
struct gd gdt[256];

// 任务状态段
struct tss
{
};

// 任务管理块
struct tcb
{
};
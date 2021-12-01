#include <idt.h>
// 首先有个256长度的数组叫做idt表。每个数组为8字节长
// 每个内容为中断处理程序的地址，长度，属性

// 每个idt的成员
#define lidt(addr) asm( \
    "lidt [%0];"        \
    :                   \
    : "r"(addr))

#define IDT_SIZE 256

struct idt_entry
{
    uint16_t offset_lowerbits;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t offset_higherbits;
} __attribute__((packed));

// 定义256个idt入口，整个就是idt表
struct idt_entry idt_table[IDT_SIZE];

struct idt_pointer
{
    uint16_t limit; // idt的长度
    uint32_t addr;  // idt的首地址
} __attribute__((packed));

struct idt_pointer idt_ptr; // 这个玩意儿会使用lidt更新idtr寄存器

void load_idt_entry(size_t int_number, uint32_t addr, uint16_t selector, uint8_t flags)
{
    idt_table[int_number].offset_lowerbits = addr & 0xFFFF;
    idt_table[int_number].offset_higherbits = (addr >> 16) & 0xFFFF;
    idt_table[int_number].selector = selector;
    idt_table[int_number].flags = flags;
    idt_table[int_number].zero = 0;
}

void init_idt()
{
    idt_ptr.addr = (uint32_t)&idt_table;
    idt_ptr.limit = 32 * IDT_SIZE - 1;
    lidt(&idt_ptr);
}
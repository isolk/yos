#ifndef IO_H
#define IO_H

#include <stdint.h>

// I/O端口读写函数（在system/src/io/io.asm中实现）
uint8_t read_port_b(uint16_t port);
uint16_t read_port_w(uint16_t port);
void write_port_b(uint16_t port, uint8_t value);

// 便捷宏
#define inb read_port_b
#define inw read_port_w
#define outb write_port_b

#endif // IO_H

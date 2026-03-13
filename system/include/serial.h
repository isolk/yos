#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

// 串口端口定义
#define SERIAL_COM1 0x3F8
#define SERIAL_COM2 0x2F8
#define SERIAL_COM3 0x3E8
#define SERIAL_COM4 0x2E8

// 串口寄存器偏移
#define SERIAL_DATA_REG(base)          (base)
#define SERIAL_INT_EN_REG(base)        (base + 1)
#define SERIAL_FIFO_CTRL_REG(base)     (base + 2)
#define SERIAL_LINE_CTRL_REG(base)     (base + 3)
#define SERIAL_MODEM_CTRL_REG(base)    (base + 4)
#define SERIAL_LINE_STATUS_REG(base)   (base + 5)
#define SERIAL_MODEM_STATUS_REG(base)  (base + 6)
#define SERIAL_SCRATCH_REG(base)       (base + 7)

// 线路控制寄存器位定义
#define SERIAL_LCR_DLAB 0x80  // Divisor Latch Access Bit
#define SERIAL_LCR_SBC  0x40  // Set Break Control
#define SERIAL_LCR_SPAR 0x20  // Stick Parity
#define SERIAL_LCR_EPAR 0x10  // Even Parity Select
#define SERIAL_LCR_PARITY 0x08  // Parity Enable
#define SERIAL_LCR_STOP  0x04  // Stop Bits
#define SERIAL_LCR_DLEN  0x03  // Data Length (8bit)

// 线路状态寄存器位定义
#define SERIAL_LSR_DR    0x01  // Data Ready
#define SERIAL_LSR_OE    0x02  // Overrun Error
#define SERIAL_LSR_PE    0x04  // Parity Error
#define SERIAL_LSR_FE    0x08  // Framing Error
#define SERIAL_LSR_BI    0x10  // Break Interrupt
#define SERIAL_LSR_THRE  0x20  // Transmitter Holding Register Empty
#define SERIAL_LSR_TEMT  0x40  // Transmitter Empty

// FIFO控制寄存器位定义
#define SERIAL_FCR_ENABLE  0x01  // Enable FIFO
#define SERIAL_FCR_CLEAR_RCVR 0x02  // Clear Receiver FIFO
#define SERIAL_FCR_CLEAR_XMIT 0x04  // Clear Transmitter FIFO

// 调制解调控制寄存器位定义
#define SERIAL_MCR_DTR  0x01  // Data Terminal Ready
#define SERIAL_MCR_RTS  0x02  // Request to Send
#define SERIAL_MCR_OUT1 0x04  // OUT1
#define SERIAL_MCR_OUT2 0x08  // OUT2
#define SERIAL_MCR_LOOP 0x10  // Loopback Mode

// 波特率配置
#define SERIAL_BAUD_38400  3
#define SERIAL_BAUD_115200 1
#define SERIAL_BAUD_57600  2
#define SERIAL_BAUD_19200  6

// 函数声明
void serial_init();
int serial_is_transmit_ready(uint16_t port);
void serial_write_char(uint16_t port, char c);
void serial_write_string(uint16_t port, const char* str);

// 便捷宏
#define serial_putchar(c) serial_write_char(SERIAL_COM1, c)
#define serial_puts(str) serial_write_string(SERIAL_COM1, str)

#endif // SERIAL_H

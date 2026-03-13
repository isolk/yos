#include <serial.h>
#include <io.h>

// 检查串口是否准备好发送数据
int serial_is_transmit_ready(uint16_t port) {
    return inb(SERIAL_LINE_STATUS_REG(port)) & SERIAL_LSR_THRE;
}

// 写入一个字符到串口
void serial_write_char(uint16_t port, char c) {
    // 等待直到发送器就绪
    while (!serial_is_transmit_ready(port)) {
        // 等待THRE位被设置
    }
    // 写入数据到发送保持寄存器
    outb(SERIAL_DATA_REG(port), c);
}

// 写入字符串到串口
void serial_write_string(uint16_t port, const char* str) {
    while (*str) {
        serial_write_char(port, *str);
        str++;
    }
}

// 初始化串口
void serial_init() {
    uint16_t port = SERIAL_COM1;

    // 1. 禁用中断
    outb(SERIAL_INT_EN_REG(port), 0x00);

    // 2. 启用DLAB（设置波特率除数锁存器访问位）
    outb(SERIAL_LINE_CTRL_REG(port), SERIAL_LCR_DLAB);

    // 3. 设置波特率为38400
    // 波特率除数 = 时钟频率 / (16 * 波特率)
    // 1.8432 MHz / (16 * 38400) = 3
    outb(SERIAL_DATA_REG(port), SERIAL_BAUD_38400);      // 低字节
    outb(SERIAL_INT_EN_REG(port), 0x00);                  // 高字节

    // 4. 设置线路参数：8位数据，无奇偶校验，1位停止位 (8N1)
    // 清除DLAB位，并设置8N1
    outb(SERIAL_LINE_CTRL_REG(port), 0x03);

    // 5. 启用FIFO，清除接收和发送FIFO
    // 设置14字节阈值
    outb(SERIAL_FIFO_CTRL_REG(port),
         SERIAL_FCR_ENABLE | SERIAL_FCR_CLEAR_RCVR | SERIAL_FCR_CLEAR_XMIT);

    // 6. 启用RTS（Request to Send）和DTR（Data Terminal Ready）
    outb(SERIAL_MODEM_CTRL_REG(port), SERIAL_MCR_DTR | SERIAL_MCR_RTS);
}

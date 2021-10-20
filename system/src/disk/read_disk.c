#include <disk.h>
#include <io.h>
#include <terminal.h>
void read_disk(uint32_t start_sector, uint16_t *target_addr, uint8_t sector)
{
    write_port_b(0x1f2, sector);

    uint8_t f3 = (uint8_t)start_sector;
    write_port_b(0x1f3, f3);

    uint8_t f4 = (uint8_t)(start_sector >> 8);
    write_port_b(0x1f4, f4);

    uint8_t f5 = (uint8_t)(start_sector >> 16);
    write_port_b(0x1f5, f5);

    uint8_t f6 = 0xE0 | (uint8_t)(start_sector >> 24);
    write_port_b(0x1f6, f6);

    write_port_b(0x1f7, 0x20);

    // https://www.unige.ch/medecine/nouspikel/ti99/ide.htm
    for (size_t i = 0; i < sector; i++)
    {
        // 注意这里需要每次读完一个扇区都需要判断下当前这个端口的状态
        // 这个状态会在读取完一个扇区后刷新，否则可能读取到0！！！
        uint8_t state = read_port_b(0x1f7);
        if (state & 1 == 1)
        {
            uint8_t reason = read_port_b(0x1f1);
            print_char(reason);
            return;
        }
        state &= 0x88;
        while (state != 0x08)
        {
            state = read_port_b(0x1f7);
            state &= 0x88;
        }
        for (size_t j = 0; j < 256; j++)
        {
            uint16_t data = read_port_w(0x1f0);
            asm volatile(
                "mov %0,(%1);"
                :
                : "r"(data), "p"(target_addr)
                :);
            target_addr += 1;
        }
    }
}
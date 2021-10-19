#include<disk.h>
#include<io.h>
#include<terminal.h>
void read_disk(uint32_t start_sector,uint32_t target_addr,uint8_t sector){
    write_port_b(0x1f2,sector);
    write_port_b(0x1f3,(uint8_t)start_sector);
    write_port_b(0x1f4,(uint8_t)(start_sector>>8));
    write_port_b(0x1f5,(uint8_t)(start_sector>>16));
    write_port_b(0x1f6,0xEF&&(uint8_t)(start_sector>>24));
    write_port_b(0x1f7,0x20);

    uint8_t state = read_port_b(0x1f7);
    state &= 0x88;
    while (state != 0x08)
    {
        state = read_port_b(0x1f7);
        state &= 0x88;
    }

    for (size_t i = 0; i < sector; i++)
    {
        for (size_t j = 0; j < 256; j++)
        {
            uint16_t data = read_port_w(0x1f0);
            uint32_t addr = target_addr+j*2+i*512;
            asm volatile(
                "mov %0,(%1);"
                :
                :"r"(data),"p"(addr)
                :
            );
        }
    }
}
#include<terminal.h>
#include<io.h>
void set_cur(uint16_t postion)
{
    uint8_t p_h = postion>>8;
    uint8_t p_l = postion;
    write_port_b(0x3d4,0x0e);
    write_port_b(0x3d5,p_h);
    write_port_b(0x3d4,0x0f);
    write_port_b(0x3d5,p_l);
}
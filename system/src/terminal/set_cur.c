#include<terminal.h>
void set_cur(uint16_t postion)
{
    uint8_t p_h = postion>>8;
    uint8_t p_l = postion;
    outb(0x3d4,0x0e);
    outb(0x3d5,p_h);
    outb(0x3d4,0x0f);
    outb(0x3d5,p_l);
}
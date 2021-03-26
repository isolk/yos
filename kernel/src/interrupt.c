#include<interrupt.h>
#include<io.h>
#define PIC_1_CTRL 0x20
#define PIC_2_CTRL 0xA0
#define PIC_1_DATA 0x21
#define PIC_2_DATA 0xA1

void init_pic()
{
    // begin
    write_port_b(PIC_1_CTRL, 0x11);
    write_port_b(PIC_1_DATA,0x20);
    write_port_b(PIC_1_DATA,1<<2);
    write_port_b(PIC_1_DATA,0x01);

    write_port_b(PIC_2_CTRL, 0x11);
    write_port_b(PIC_2_DATA,0x28);
    write_port_b(PIC_2_DATA,0x01);
    write_port_b(PIC_2_DATA,0x01);

    write_port_b(0x21,0xff); 
    write_port_b(0xa1,0xff);
}
#include<time.h>
#include<idt.h>
void init_time()
{
    cli();

    write_port_b(0x70,0x8b);
    write_port_b(0x71,0x12);

    write_port_b(0x70,0x0c);
    read_port_b(0x71);

    write_port_b(0xa1,0x00);
    sti();
}


void time_handler()
{
    /* Send End of Interrupt (EOI) to master PIC */
    print_char('#');

    asm volatile ("xchg %bx, %bx");

    write_port_b(0x70,0x80);
    uint8_t sec = read_port_b(0x71);


    write_port_b(0x70,0x82);
    uint8_t min = read_port_b(0x71);

    write_port_b(0x70,0x84);
    uint8_t hour = read_port_b(0x71);

    write_port_b(0x70,0x0c);
    read_port_b(0x71);

    uint8_t h = hour>>4;
    uint8_t l = hour&0x0F;
    put_char(0,0,h+'0');
    put_char(0,1,l+'0');
    put_char(0,2,':');

    h = min>>4;
    l = min&0x0F;
    put_char(0,3,h+'0');
    put_char(0,4,l+'0');
    put_char(0,5,':');

    h = sec >>4;
    l = sec&0x0F;
    put_char(0,6,h+'0');
    put_char(0,7,l+'0');

    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}

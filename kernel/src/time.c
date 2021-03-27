#include<io.h>
#include<terminal.h>
#include<time.h>
#include<string.h>
#include<idt.h>
void init_time()
{
    cli();
    write_port_b(0x70,0x8b);
    write_port_b(0x71,0x12);
    write_port_b(0x70,0x0c);
    read_port_b(0x71);

    uint8_t curmask_master =  read_port_b(0xa1);
    write_port_b(0xa1, curmask_master & 0xFE);
    sti();
}


void time_handler()
{
    /* Send End of Interrupt (EOI) to master PIC */
    print_char('#');
    write_port_b(0x70,0x0c);
    read_port_b(0x71);

    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}

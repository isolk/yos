#include<time.h>
#include<idt.h>
int cur_task = 0;
void init_time()
{
    write_port_b(0x70,0x8b);
    write_port_b(0x71,0x12);

    write_port_b(0x70,0x0c);
    read_port_b(0x71);

    write_port_b(0xa1,0x00);
}


void time_handler()
{
    write_port_b(0x70,0x0c);
    read_port_b(0x71);

    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);

    // 判断当前任务号，使用jmp切换
    if (cur_task == 0){
        cur_task = 1;
        // print_char('*');
        // asm("xchg %bx,%bx");
        asm("jmp $0x30,$0");
    }else {
        cur_task = 0;
        // print_char('#');
        // asm("xchg %bx,%bx");
        asm("jmp $0x28,$0");
    }
}

void show_c()
{
   print_char('*');
   write_port_b(0x20, 0x20);
   write_port_b(0xa0, 0x20); 
}

void show_time(){
    asm volatile ("xchg %bx,%bx");
    asm volatile ("mov %ds, %eax");
    asm volatile ("push %ax");
    uint16_t ds=echo();
    if (ds == 0x10){
        print_char('#');
    }else 
    {
        print_char('!');
    }
    
    return;
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
}

void default_handler()
{
    // write_port_b(0x70,0x0c);
    // read_port_b(0x71);

    // asm("xchg %bx,%bx");
    // write_port_b(0x20, 0x20);
    // write_port_b(0xa0, 0x20);
    // print_char('*');
}

void syscall_handler()
{
    uint16_t ax = 0;
    asm("mov %%ax,%0":"=r"(ax));
    if (ax == 0){
         print_char('0');
    }else {
        
         print_char('1');
    }
}
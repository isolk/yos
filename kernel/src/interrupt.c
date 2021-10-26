#include <string.h>
#include <interrupt.h>
#include <stdint-gcc.h>
#define PIC_1_CTRL 0x20
#define PIC_1_DATA 0x21
#define PIC_2_CTRL 0xA0
#define PIC_2_DATA 0xA1

void init_pic()
{
    // begin
    write_port_b(PIC_1_CTRL, 0x11);
    write_port_b(PIC_1_DATA, 0x20);
    write_port_b(PIC_1_DATA, 1 << 2);
    write_port_b(PIC_1_DATA, 0x01);

    write_port_b(PIC_2_CTRL, 0x11);
    write_port_b(PIC_2_DATA, 0x28);
    write_port_b(PIC_2_DATA, 0x01);
    write_port_b(PIC_2_DATA, 0x01);

    write_port_b(0x21, 0xff);
    write_port_b(0xa1, 0xff);
}

void i_hander_0() { printf("0"); }
void i_hander_1() { printf("1"); }
void i_hander_2() { printf("2"); }
void i_hander_3() { printf("3"); }
void i_hander_4() { printf("4"); }
void i_hander_5() { printf("5"); }
void i_hander_6() { printf("6"); }
void i_hander_7() { printf("7"); }
void i_hander_8() { printf("8"); }
void i_hander_9() { printf("9"); }
void i_hander_10() { printf("10"); }
void i_hander_11() { printf("11"); }
void i_hander_12() { printf("12"); }
void i_hander_14() { printf("14"); }
void i_hander_15() { printf("15"); }
void i_hander_16() { printf("16"); }
void i_hander_17() { printf("17"); }
void i_hander_18() { printf("18"); }
void i_hander_19() { printf("19"); }
void i_hander_20() { printf("20"); }
void i_hander_21() { printf("21"); }
void i_hander_22() { printf("22"); }
void i_hander_23() { printf("23"); }
void i_hander_24() { printf("24"); }
void i_hander_25() { printf("25"); }
void i_hander_26() { printf("26"); }
void i_hander_27() { printf("27"); }
void i_hander_28() { printf("28"); }
void i_hander_29() { printf("29"); }
void i_hander_30() { printf("30"); }
void i_hander_31() { printf("31"); }

void *handlers[32] = {
    i_hander_0,
    i_hander_1,
    i_hander_2,
    i_hander_3,
    i_hander_4,
    i_hander_5,
    i_hander_6,
    i_hander_7,
    i_hander_8,
    i_hander_9,
    i_hander_10,
    i_hander_11,
    i_hander_12,
    i_hander_13,
    i_hander_14,
    i_hander_15,
    i_hander_16,
    i_hander_17,
    i_hander_18,
    i_hander_19,
    i_hander_20,
    i_hander_21,
    i_hander_22,
    i_hander_23,
    i_hander_24,
    i_hander_25,
    i_hander_26,
    i_hander_27,
    i_hander_28,
    i_hander_29,
    i_hander_30,
    i_hander_31};

void i_hander_13_do(uint32_t err_code)
{
    asm("xchg %bx,%bx");
    printf("%d", err_code);
}
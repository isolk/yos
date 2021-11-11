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

void i_hander_0()
{
    printf("0");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_1()
{
    printf("1");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_2()
{
    printf("2");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_3()
{
    printf("3");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_4()
{
    printf("4");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_5()
{
    printf("5");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_6()
{
    printf("6");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_7()
{
    printf("7");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_8()
{
    printf("8");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_9()
{
    printf("9");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}

void i_hander_11()
{
    printf("11");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_12()
{
    printf("12");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_14()
{
    printf("14");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_15()
{
    printf("15");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_16()
{
    printf("16");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_17()
{
    printf("17");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_18()
{
    printf("18");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_19()
{
    printf("19");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_20()
{
    printf("20");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_21()
{
    printf("21");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_22()
{
    printf("22");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_23()
{
    printf("23");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_24()
{
    printf("24");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_25()
{
    printf("25");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_26()
{
    printf("26");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_27()
{
    printf("27");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_28()
{
    printf("28");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_29()
{
    printf("29");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_30()
{
    printf("30");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
void i_hander_31()
{
    printf("31");
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}

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

uint32_t selector(uint32_t code)
{
    return (code << 16) >> 19;
}

uint8_t from_outside(uint32_t code)
{
    return code & 1;
}

uint8_t tbl(uint32_t code)
{
    return code << 29 >> 30;
}

void i_hander_13_do(uint32_t err_code)
{
    // asm("xchg %bx,%bx");
    uint32_t s = selector(err_code);
    uint8_t f = from_outside(err_code);
    uint8_t t = tbl(err_code);
    printf("errcode=%d      -----          ", err_code);
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}

void i_hander_10_do(uint32_t err_code)
{
    // asm("xchg %bx,%bx");
    uint32_t s = selector(err_code);
    uint8_t f = from_outside(err_code);
    uint8_t t = tbl(err_code);
    printf("errcode=%d      -----          ", err_code);
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
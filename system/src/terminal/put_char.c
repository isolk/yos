#include<terminal.h>
void put_char(size_t row,size_t column,uint8_t str){
    uint8_t* tr = (uint8_t*)0xb8000;
    tr = tr+row*160+column*2;
    *tr = str;
}
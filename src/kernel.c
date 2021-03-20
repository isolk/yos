#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
void set_cur();

size_t g_row=0;
size_t g_column=0;
void put_char(size_t row,size_t column,uint8_t str){
    uint8_t* tr = (uint8_t*)0xb8000;
    tr = tr+row*160+column*2;
    *tr = str;
}

void clear(){
    for(size_t i=0;i<25;i++){
        for (size_t j=0;j<80;j++){
            put_char(i,j,' ');
        }
    }
}

void print_char(uint8_t str){
    if (str == '\n'){
        g_row++;
        g_column=0;
        put_char(g_row,g_column,str);
        return;
    }

    put_char(g_row,g_column,str);
    g_column++;
    if(g_column>=80){
        g_row++;
        g_column=0;
    }
}

int _start()
{
    clear();
    char *s = "hello,world.\nwhat you are!!!!!";
    for (size_t i = 0; i < 20; i++)
    {
        print_char(s[i]);
    }
    
    set_cur();
    for(;;){}
    return 0;
}
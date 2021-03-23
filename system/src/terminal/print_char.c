#include<terminal.h>
int g_row;
int g_column;
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
#include<terminal.h>
int g_row;
int g_column;

void set0(size_t row)
{
    for (size_t i = 0; i < WIDTH; i++)
    {
        put_char(row,i,'=');
    }
    
}

void srcoll()
{
    set0(4);
}

void print_char(uint8_t str){
    // row最大只能为24，此时位于屏幕最下方1行
    // column最大只能为79，此时位于最右侧
    if (g_row == 4 && g_column == WIDTH-1)
    {
        srcoll();
    }else 
    {
        put_char(g_row,g_column,str);
    }
     
    g_column++;
    if(g_column ==WIDTH){
        g_row++;
        g_column=0;
    }
}


#include<terminal.h>
int g_row;
int g_column;
char data[ROW][WIDTH];
void srcoll()
{
    for (size_t i = 0; i < ROW-1; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            put_char(i,j,data[i+1][j]);
        }
    }

    for (size_t i = 0; i < WIDTH; i++)
    {
        put_char(ROW-1,i,'\0');
    }
    
}

void print_char(uint8_t str){
    // if (g_row == ROW-1 && g_column >= WIDTH)
    // {
    //     srcoll();
    // } 

    if (str == '\n'){
        g_row++;
        g_column=0;
        put_char(g_row,g_column,str);
        data[g_row][g_column]=str;
        return;
    }

    put_char(g_row,g_column,str);
    data[g_row][g_column]=str;

    g_column++;
    if(g_column>WIDTH){
        g_row++;
        g_column=0;
    }
}


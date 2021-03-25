#include <terminal.h>
int g_row;
int g_column;
void moveup_row(size_t row)
{
    if (row == 0)
    {
        return;
    }

    for (size_t i = 0; i < WIDTH; i++)
    {
        put_char(row - 1, i, get_char(row, i));
    }

    if (row == ROW - 1)
    {
        for (size_t i = 0; i < WIDTH; i++)
        {
            put_char(row, i, '\0');
        }
    }
}

void srcoll()
{
    for (size_t i = 1; i < ROW; i++)
    {
        moveup_row(i);
    }
}

void print_char(uint8_t str)
{
    // row最大只能为24，此时位于屏幕最下方1行
    // column最大只能为79，此时位于最右侧
    if (g_row == ROW && g_column == 0)
    {
        srcoll();
        g_row = ROW - 1;
    }

    put_char(g_row, g_column, str);
    g_column++;
    if (g_column == WIDTH)
    {
        g_row++;
        g_column = 0;
    }
}

#include<terminal.h>
void clear()
{
    for(size_t i=0;i<ROW;i++)
    {
        for (size_t j=0;j<WIDTH;j++)
        {
            put_char(i,j,' ');
        }
    }
}
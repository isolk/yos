#include<terminal.h> 
void print_string(char* str,size_t len){
    for (size_t i = 0; i < len; i++)
    {
        print_char(str[i]);
    }
}
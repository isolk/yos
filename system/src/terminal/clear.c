#include<terminal.h>
void clear(){
    for(size_t i=0;i<25;i++){
        for (size_t j=0;j<80;j++){
            put_char(i,j,' ');
        }
    }
}
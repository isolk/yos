// void time_handler();
// void init_time();
#include<stdint-gcc.h>

void page_handler(uint32_t cr2, uint32_t err_code);
extern int cur_task;
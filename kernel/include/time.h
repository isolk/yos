#include <stdint-gcc.h>

void init_time();

void time_handler();

uint32_t timer_get_ticks();

uint32_t timer_ms_to_ticks(uint32_t ms);

void page_handler(uint32_t cr2, uint32_t err_code);
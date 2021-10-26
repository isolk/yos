#include <stdint-gcc.h>
#include <stddef.h>
// 从地址：512的倍数读取sector个扇区
void read_disk(uint32_t start_sector,uint16_t *target_addr,uint8_t sector);
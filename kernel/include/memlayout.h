#ifndef _MEMLAYOUT_H
#define _MEMLAYOUT_H

/* Memory layout constants for YOS kernel */

/* Kernel base virtual address (3GB) */
#define KERNEL_BASE 0xC0000000u

/* User space ends at 3GB, kernel space starts at 3GB */
#define USER_END (3 * 1024 * 1024 * 1024u)
#define KERNEL_VIRT_ADDR KERNEL_BASE

/* Physical memory mapped to kernel space */
#define KERNEL_PHYS_TO_VIRT(phys) ((phys) + KERNEL_BASE)
#define KERNEL_VIRT_TO_PHYS(virt) ((virt) - KERNEL_BASE)

#endif /* _MEMLAYOUT_H */

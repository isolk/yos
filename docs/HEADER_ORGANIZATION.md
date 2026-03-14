# Header File Organization

## Current Structure

All header files are currently flat in `kernel/include/`:
- elf.h, gdt.h, idle.h, idt.h, interrupt.h
- io.h, keyboard.h, ldt.h, linked_list.h
- memlayout.h, mm.h, page.h, pm.h
- process.h, serial.h, string.h, sys_call.h, time.h, tss.h

## Suggested Organization

For better code organization, consider grouping headers by functionality:

```
kernel/include/
├── proc/              # Process management
│   ├── process.h      # Process structures and scheduling
│   └── idle.h         # Idle process management
│
├── mm/                # Memory management
│   ├── pm.h           # Physical memory allocator
│   ├── page.h         # Page table management
│   ├── mm.h           # Memory management (brk)
│   └── memlayout.h    # Memory layout constants
│
├── arch/              # Architecture-specific (x86)
│   ├── gdt.h          # Global Descriptor Table
│   ├── idt.h          # Interrupt Descriptor Table
│   ├── tss.h          # Task State Segment
│   └── ldt.h          # Local Descriptor Table
│
├── interrupt/         # Interrupt handling
│   ├── interrupt.h    # Interrupt handling
│   └── sys_call.h     # System call interface
│
├── driver/            # Device drivers
│   ├── serial.h       # Serial port driver
│   ├── keyboard.h     # Keyboard driver
│   └── io.h           # I/O port operations
│
└── lib/               # Utility libraries
    ├── string.h       # String operations
    ├── linked_list.h  # Linked list data structure
    └── elf.h          # ELF file format
```

## Migration Notes

If implementing this structure:
1. Create subdirectories under `kernel/include/`
2. Move header files to appropriate directories
3. Update all `#include` statements in source files
4. Update Makefile CFLAGS to include new paths: `-I include/proc -I include/mm -I include/arch ...`

## Benefits

- Clearer code organization
- Easier to find relevant headers
- Better separation of concerns
- Easier for new contributors to understand structure

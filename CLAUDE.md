# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

YOS is a mini OS kernel written from scratch, inspired by Linux. It's an educational x86-32 operating system kernel implemented in C and Assembly, designed to learn OS internals, computer architecture, and Linux kernel design principles.

**Platform**: x86-32 only
**Languages**: C + Assembly (NASM)
**Toolchain**: i686-elf-gcc cross-compiler

## Build Commands

### Complete Build
```bash
./build.sh        # Builds all components (libc, system, user, kernel)
```

### Component-specific builds
```bash
make -C libc      # Build C standard library
make -C system    # Build system library
make -C user      # Build user programs
make -C kernel    # Build kernel
```

### Clean
```bash
./clean.sh        # Remove all build artifacts
```

## Running and Debugging

### Quick Debug (Recommended)
```bash
./quick_debug.sh  # Runs clean, build, and debug sequentially
```

This script runs:
1. `./clean.sh` - Clean build artifacts
2. `./build.sh` - Build everything
3. `./debug.sh` - Start QEMU with GDB server

### Manual Debug
```bash
# Terminal 1: Start QEMU with GDB server
./debug.sh
# Or: qemu-system-i386 -no-reboot -no-shutdown -s -S -drive file=kernel/bin/disk.img,format=raw,index=0,media=disk

# Terminal 2: Connect GDB
i386-elf-gdb kernel/obj/yos.bin
(gdb) target remote localhost:1234
```

### Debugging in VS Code
Use the "gdb yos" launch configuration (F5). It automatically:
1. Runs `./quick_debug.sh` as a pre-launch task
2. Connects GDB to localhost:1234
3. Loads symbols from `kernel/obj/yos.bin`

### Bochs (Alternative Emulator)
```bash
bochs -qf bochs/bochs.cfg
```

The Bochs configuration is in `bochs/bochs.cfg` with magic_break enabled for debugging.

## Architecture

### Memory Layout
The kernel uses a specific 3GB/1GB user/kernel split:
- **User space (0-3GB)**: User processes
- **Kernel space (3-4GB)**: Mapped to physical 0-1GB

Page tables are initialized in `kernel/src/page.c` with this mapping scheme.

### Project Structure

```
kernel/          # Core kernel code
  src/           # C and Assembly source files
  include/       # Kernel headers
  obj/           # Object files and binaries
  bin/           # disk.img (bootable disk image)

libc/            # C standard library (freestanding)
  src/           # Library implementations (string, stdlib, etc.)
  include/       # Library headers

system/          # System library
  include/       # System headers (io, disk, terminal, etc.)

user/            # User-space programs
  hello          # Example user program (loaded at disk sector 1000)
```

### Core Kernel Components

Located in `kernel/src/`:

- **boot.asm, loader.asm**: Bootloader and kernel loader
- **kernel.c**: Main kernel entry point (`_start()`)
- **process.c**: Process management, task scheduling, round-robin scheduler
- **page.c**: Page table initialization and memory mapping
- **gdt.c, idt.c**: Global and Interrupt Descriptor Tables
- **tss.c, ldt.c**: Task State Segment and Local Descriptor Table
- **interrupt.c**: Interrupt handling
- **time.c**: Timer/clock for scheduling
- **elf.c**: ELF file loader for user programs
- **sys_call.c**: System call handler (syscall 1=print, 2=exit)

### Process Management

Process structure (`kernel/include/process.h`):
- Each process has a `task_struct` with PID, page directory, TSS, and state
- Linked list of tasks with circular doubly-linked list
- PID < 1000: Kernel tasks (idle process)
- PID >= 1000: User processes
- Round-robin scheduling via timer interrupts
- Context switch through TSS

The `start()` function in `process.c` creates the initial idle process (PID 0) and first user process (PID 1001) by loading an ELF program from disk sector 1000.

### System Calls

System calls use interrupt 0x80 (`int $0x80`):
- Syscall 1: Print character (passed in ebx)
- Syscall 2: Exit process

### Disk Image Layout

The 1MB disk image (`kernel/bin/disk.img`) is assembled by `kernel/Makefile`:
- Sector 0: boot.bin (bootloader)
- Sectors 1-2: loader.bin (kernel loader)
- Sectors 3-995: yos.bin (kernel)
- Sectors 1000+: User programs (e.g., `hello`)

### Build Details

Each component's Makefile:
- Uses `i686-elf-gcc` with `-ffreestanding -nostdlib` flags
- Links with custom libc (`-lstd`) and system library (`-lsys`)
- Kernel links with libgcc for compiler support
- User programs are statically linked

## Development Notes

- All code is hand-written; no third-party OS code
- Target platform: x86-32 only
- Bochs configuration enables magic_break (use `xchg bx, bx` instruction in assembly for breakpoints)
- GDB debugging: Connect to localhost:1234, load symbols from `kernel/obj/yos.bin`
- The kernel initializes memory, paging, GDT, LDT, IDT, interrupts, then starts the process scheduler

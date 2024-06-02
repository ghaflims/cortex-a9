# Baremetal ARM Proramming using qemu vexpress-a9 board
This is my attempt to do baremetal programming on cortex-a9 using qemu and vexpress-a9 board.

## Working
* Keyboard using ARM (PL050)
* Display using ARM (PL111)
* Timer usin ARM (SP804)
* UART using ARM (PL011)
* SD card using ARM (PL181)
* Real Time Clock (RTC) using ARM (PL031)
* Interrrupts using ARM (GIC) and IRQ code stub
* µGUI - free and open source graphic library for embedded systems Port
* FatFs - Generic FAT Filesystem Module Port
* uClibc - C library for developing embedded Linux systems syscall Port

## Plans for Future
- [ ] Code Cleanup & Refactoring
- [ ] Better Build Script and IDE Support (Debugging, Building, and Running)
- [ ] Audio Support using ARM Advance Audio Codec Interface (PL041)
- [ ] Network Support using LAN9118 ethernet
- [ ] Porting Network Stack using lwIP
- [ ] MultiCore Support
- [ ] Better Driver Support and Imporovements
- [ ] Code Documentation and overall Walkthrough
- [ ] Detailed Walkthrough of My BareMetal Adventure

## Dependencies
- arm-none-eabi
- qemu-system-arm
- gdb-multiarch (only if you need to debug) 

## How to Run
I've tested this on Linux platform only, the `Makefile` is hopefully easy to read, you can change flags there
- `make` will build the project
- `make qemu` will run elf kernel in qemu
- `make dqemu` will instruct qemu to pause execution and listen for gdb remote connection on port `1234`
- `make gdb` will run the gdb-multiarch [do this in seperate terminal and after runnin `make dqemu`]
    - note you need to execute `target remote :1234` to connect to qemu debug port

## Quick Code Walkthrough
In this section I'll briefly highlight main point of important file. This will help explain the big picture. Hopefully, I'll write detailed guide in future.

Linker Script `linker.ld`:
- ensures Interrupt vector is at the start of the kernel binary
- contains necessary symbols for stack, heap, ulibc, ...etc to work (refrenced by `startup.S`) and `_sbrk()` for `malloc()`
- describe how the final layout of the binary

Code entry point is the `statup.S` which will do the following:
- Initialize the interrupt vectors
- Initialize the stack and ARM code
- jump to C code

ulibC and `syscall.c`:
- contains implemenation of needed stubs for standard C library to work properly
- `_write` has been implemented to print to uart. This will allow printf to work
- `_sbrk` has been implemented utilizing symbols in `linker.ld`. This will allow `malloc` to work
- before jumping to main standard C will initialize the `.bss` section to zero (C runtime)

`interrupt.c` and `gic.c`:
- here we Initialize the ARM `GIC` (Generic Interrupt Controller)
- `c_irq` is the function that will handle irq. The function works by getting the IRQ# (who triggered the interrupt) and then use this number to check in a function pointer array (is there a handler for this irq?). if yes then call that handler, else print no handler found
- `install_isr` function will allow device driver code to hook intterupt handler for specific IRQ# [`c_irq` will call this hook]
- `enable_irq` will enable specified IRQ# in `GIC`

[[To be Continued..]]

## Screenshots
![qemu display](imgs/qemu_display.png)
![qemu display](imgs/qemu_serial_output.png)


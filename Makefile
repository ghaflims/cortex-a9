TOOLCHAIN = arm-none-eabi
TARGET = kernel.elf
ARMARCH = armv7-a
CORE = cortex-a9
CC = $(TOOLCHAIN)-gcc
AS = $(TOOLCHAIN)-as
SIZE = $(TOOLCHAIN)-size
DUMP = $(TOOLCHAIN)-objdump
LFILE = src/linker.ld
# Compile flags here
CFLAGS   = -std=gnu11 -Wall -nostartfiles -fno-exceptions -mcpu=$(CORE) -static -g
LINKER   = $(CC) -o
# linking flags here
LFLAGS   = -Wall -T $(LFILE) -nostartfiles -fno-exceptions -mcpu=$(CORE) -static -lc


GDB = $(TOOLCHAIN)-gdb
QEMU = qemu-system-arm
QEMU_OPTS = -M vexpress-a9 -serial mon:stdio -kernel

SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f


$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(LFLAGS) $(OBJECTS)
	@echo "Linking complete!"
	@$(SIZE) $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(OBJDIR)/%.o : $(SRCDIR)/%.S
	@$(AS) $(CFLAGS) -c $< -o $@
	@echo "Assembled "$<" successfully!"

qemu: 
	$(QEMU) $(QEMU_OPTS) $(BINDIR)/$(TARGET)

gdb: 
	$(GDB) $(BINDIR)/$(TARGET)

dqemu: all
	$(QEMU) -s -S $(QEMU_OPTS) $(BINDIR)/$(TARGET)

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
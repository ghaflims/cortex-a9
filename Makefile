TOOLCHAIN = arm-none-eabi
TARGET = kernel.elf
ARMARCH = armv7-a
CORE = cortex-a9
CC = $(TOOLCHAIN)-gcc
AS = $(TOOLCHAIN)-as
SIZE = $(TOOLCHAIN)-size
DUMP = $(TOOLCHAIN)-objdump

SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

LFILE = $(SRCDIR)/linker.ld
# Compile flags here
CFLAGS   = -std=gnu11 -Wall -nostartfiles -fno-exceptions -mcpu=$(CORE) -static -g
AFLAGS   = 
LINKER   = $(CC) -o
# linking flags here
LFLAGS   = -Wall -T $(LFILE) -nostartfiles -fno-exceptions --specs=nosys.specs -mcpu=$(CORE) -static -g -lc


GDB = $(TOOLCHAIN)-gdb
QEMU = qemu-system-arm
QEMU_OPTS = -M vexpress-a9 -serial mon:stdio -kernel



C_FILES := $(wildcard $(SRCDIR)/*.c)
AS_FILES := $(wildcard $(SRCDIR)/*.S)
OBJECTS_C := $(addprefix $(OBJDIR)/,$(notdir $(C_FILES:.c=.o)))
OBJECTS_S := $(addprefix $(OBJDIR)/,$(notdir $(AS_FILES:.S=.o)))
OBJECTS_ALL := $(OBJECTS_S) $(OBJECTS_C)
rm = rm -f


$(BINDIR)/$(TARGET): $(OBJECTS_ALL)
	@mkdir -p $(@D)
	@$(LINKER) $@ $(LFLAGS) $(OBJECTS_ALL)
	@echo "Linking complete!"
	@$(SIZE) $@

$(OBJECTS_ALL) : | obj

$(OBJDIR):
	@mkdir -p $@

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(OBJDIR)/%.o : $(SRCDIR)/%.S
	@$(AS) $(AFLAGS) $< -o $@
	@echo "Assembled "$<" successfully!"

qemu: 
	$(QEMU) $(QEMU_OPTS) $(BINDIR)/$(TARGET)

gdb: 
	$(GDB) $(BINDIR)/$(TARGET)

dqemu: all
	$(QEMU) -s -S $(QEMU_OPTS) $(BINDIR)/$(TARGET)

.PHONY: clean
clean:
	@$(rm) $(OBJECTS_ALL)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
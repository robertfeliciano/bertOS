CC := aarch64-elf-gcc
ASM := aarch64-elf-as
CFLAGS := -ffreestanding -O2 -Wall -Wextra -Iinclude -c
QFLAGS := qemu-system-aarch64 -M raspi3 -serial stdio -kernel

SRC_DIRS := ./kernel
BIN_DIRS := ./bin

BOOTLDR := boot.o
KERN := kernel.o

all: boot kernel
	$(CC) -T linker.ld -o $(BIN_DIRS)/bertos.elf -ffreestanding -O2 -nostdlib \
	$(BIN_DIRS)/$(BOOTLDR) $(BIN_DIRS)/$(KERN) -lgcc

	aarch64-elf-objcopy $(BIN_DIRS)/bertos.elf -O binary kernel8.img

.PHONY: boot
boot:
	@echo "Building bootloader..."
	$(ASM) $(SRC_DIRS)/boot.S -o $(BIN_DIRS)/boot.o

.PHONY: kernel
kernel:
	@echo "Building kernel..."
	$(CC) $(CFLAGS) $(SRC_DIRS)/kernel.c -o $(BIN_DIRS)/kernel.o

qemu: all
	$(QFLAGS) kernel8.img


.PHONY: clean
clean:
	@echo "Cleaning..."
	rm -f *.img
	rm $(BIN_DIRS)/*
	
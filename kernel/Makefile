ASM = aarch64-elf-as
CC = aarch64-elf-gcc
CFLAGS = -ffreestanding -O2 -Wall -Wextra -c

export clean


all:
	@$(ASM) -c boot.S -o boot.o
	@$(CC) $(CFLAGS) kernel.c -o kernel.o
	@$(CC) -T ../linker.ld -o bertos.elf -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc 
	@aarch64-elf-objcopy bertos.elf -O binary kernel8.img

clean:
	@rm *.o *.elf
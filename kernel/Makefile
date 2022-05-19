AARCH=aarch64-elf-gcc

.PHONY: kernel
kernel:
	$(AARCH) -ffreestanding -c kernel.c -o kernel.o -O2 -Wall -Wextra

.PHONY: clean
clean: 
	rm *.o
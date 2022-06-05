all:
	@$(MAKE) -C kernel

clean:
	@$(MAKE) clean -C kernel

qemu: 
	@(qemu-system-aarch64 -M raspi3 -serial stdio -kernel kernel/kernel8.img)
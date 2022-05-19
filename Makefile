AARCH=aarch64-elf-as

.PHONY: boot
boot:
	$(AARCH) -c boot.S -o boot.o

.PHONY: clean
clean: 
	rm *.o
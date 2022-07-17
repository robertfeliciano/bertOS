GCC = aarch64-linux-gnu-gcc
ARM = aarch64-linux-gnu
OPTS = -Wall -nostdlib -nostartfiles -ffreestanding -mgeneral-regs-only $(INC)
ASMOPT = -Iinclude
QFLAGS = qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio

INC = -Iinclude

BUILD = build
SRC = kernel
DEP = dependencies

$(BUILD)/%_c.o: $(SRC)/%.c
	@mkdir -p $(@D)
	$(GCC) $(OPTS) -MMD -c $< -o $@

$(BUILD)/%_S.o: $(SRC)/%.S
	$(GCC) $(ASMOPT) -MMD -c $< -o $@

C_FILES = $(wildcard $(SRC)/*.c)
ASM_FILES = $(wildcard $(SRC)/*.S)
OBJ_FILES = $(C_FILES:$(SRC)/%.c=$(BUILD)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SRC)/%.S=$(BUILD)/%_S.o)

DEP_FILES = $(DEP)/$(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

.PHONY: kernel
kernel: $(SRC)/linker.ld $(OBJ_FILES)
	$(ARM)-ld -T $(SRC)/linker.ld -o $(BUILD)/kernel8.elf $(OBJ_FILES)
	$(ARM)-objcopy $(BUILD)/kernel8.elf -O binary kernel8.img

qemu: kernel
	@printf "\n\n"
	$(QFLAGS)

clean:
	rm -rf $(BUILD) *.img *.d

clear: clean
	@clear
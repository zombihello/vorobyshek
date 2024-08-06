all: disk

# https://stackoverflow.com/questions/3931741/why-does-make-think-the-target-is-up-to-date
.PHONY: disk tools qemu qemu_dbg clean kernel bootloader

# Build kernel
kernel:
	@${MAKE} -C src/kernel kernel

# Build bootloader
bootloader:
	@${MAKE} -C src/bootloader all

# Build disk image
disk: kernel bootloader
	@${MAKE} -C src/kernel disk

# Build tools (autotools, binutils and gcc)
tools:
	@${MAKE} -C src/kernel tools

# Run QEMU
qemu: disk
	@${MAKE} -C src/kernel qemu

# Run QEMU for debug
qemu_dbg: disk
	@${MAKE} -C src/kernel qemu_dbg

# Clean
clean:
	@${MAKE} -C src/kernel clean

all: disk

# https://stackoverflow.com/questions/3931741/why-does-make-think-the-target-is-up-to-date
.PHONY: disk tools verifytools qemu clean

# Build disk image
disk_prepare: verifytools
disk: disk_prepare
	@${MAKE} -C src/kernel disk

# Verify our toolchain
TOOLCHAIN_GCC_VERSION := ${shell ./tools/toolchain/gcc/bin/i686-elf-gcc --version 2>/dev/null}
verifytools:
ifdef TOOLCHAIN_GCC_VERSION
	@echo "Toolchain: ${TOOLCHAIN_GCC_VERSION}"
else
	@echo -e '\033[0;31mi686-elf-gcc was not found! Please use "make tools" to compile the toolchain!\033[0m'
	@exit 1
endif

# Build tools (autotools, binutils and gcc)
tools:
	@${MAKE} -C src/kernel tools

# Run QEMU
qemu:
	@${MAKE} -C src/kernel qemu

# Clean
clean:
	@${MAKE} -C src/kernel clean

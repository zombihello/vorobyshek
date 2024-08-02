# General variables
OS_VER 				= 0.0.1
OS_NAME 			= vorobyshek

# Path to directories
TOOLS_DIR 			= ../../tools
TARGET_DIR			= ../../target
BUILD_DIR 			= ../../build

# File names
TARGET_ISO 			= ../../${OS_NAME}-${OS_VER}.iso
OUTPUT_KERNEL 		= ${TARGET_DIR}/boot/kernel.bin

# Toolchains and tools
CC 					= ${TOOLS_DIR}/toolchain/gcc/bin/i686-elf-gcc
LINK 				= ${TOOLS_DIR}/toolchain/gcc/bin/i686-elf-gcc
ASM 				= nasm
QEMU 				= qemu-system-x86_64
GRUB_MKRSC 			= grub-mkrescue

# Compiler and tools flags
ASMFLAGS 			= -f elf32
CFLAGS_DEBUG 		= -O0 -g
CFLAGS_RELEASE 		= -O2 -s -DNDEBUG 		# TODO yehor.pohuliaka - Add support Debug and Release configurations
CFLAGS 				= ${CFLAGS_DEBUG} -c -ffreestanding -nostdlib -std=c99 -Iinclude/
LDFLAGS 			= -T link.ld -nostdlib
LDLIBS 				= -lgcc
QEMUFLAGS 			= -serial stdio -m 4g
QEMUFLAGS_DEBUG 	= ${QEMUFLAGS} -s -S -no-shutdown -no-reboot
GRUB_MKRSCFLAGS 	= -d /usr/lib/grub/i386-pc

# Grab all source files
C_SRC 				:= ${shell find . -name '*.c' -printf "%P\n"}
ASM_SRC 			:= ${shell find . -name '*.asm' -printf "%P\n"}
C_OBJS 				:= ${patsubst %.c, ${BUILD_DIR}/%.o, ${C_SRC}}
ASM_OBJS 			:= ${patsubst %.asm, ${BUILD_DIR}/%.asm.o, ${ASM_SRC}}

.PHONY: clean

# Build kernel
kernel: $(C_OBJS) $(ASM_OBJS)
	@mkdir -p ${TARGET_DIR}
	@mkdir -p ${TARGET_DIR}/boot
	${LINK} ${LDFLAGS} -o ${OUTPUT_KERNEL} ${C_OBJS} ${ASM_OBJS} ${LDLIBS}

# Compile source code (C and ASM)
${BUILD_DIR}/%.o: %.c
	@mkdir -p ${dir $@}
	${CC} ${CFLAGS} -o $@ $<

${BUILD_DIR}/%.asm.o: %.asm
	@mkdir -p ${dir $@}
	${ASM} ${ASMFLAGS} -o $@ $< 

# Build disk image
disk: kernel
	${GRUB_MKRSC} ${GRUB_MKRSCFLAGS} -o ${TARGET_ISO} ${TARGET_DIR}

# Build tools (autotools, binutils and gcc)
tools:
	@chmod +x ${TOOLS_DIR}/toolchain/compile_toolchain.sh
	@${TOOLS_DIR}/toolchain/compile_toolchain.sh

# Run QEMU
qemu: kernel
	${QEMU} ${QEMUFLAGS} -hda ${TARGET_ISO}

# Run QEMU for debug
qemu_dbg: kernel
	${QEMU} ${QEMUFLAGS_DEBUG} -hda ${TARGET_ISO}

# Clean
clean:
	@rm -rf ${BUILD_DIR}
	@rm -rf ${TARGET_ISO}

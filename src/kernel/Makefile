# General variables
OS_VER 				= 0.0.1
OS_NAME 			= vorobyshek

# Path to directories
TOOLS_DIR 			= ../../tools
TARGET_DIR			= ../../target
BUILD_DIR 			= ../../build/kernel
GEN_DIR 			= __gen__

# File names
TARGET_ISO 			= ../../${OS_NAME}-${OS_VER}.iso
OUTPUT_KERNEL 		= ${TARGET_DIR}/boot/kernel.bin
LD_SCRIPT 			= arch/x86/link64.ld

# Generated files
C_ISRS_GEN_FILE 	= ${GEN_DIR}/arch/x86/isrs_gen.c
ASM_ISRS_GEN_FILE 	= ${GEN_DIR}/arch/x86/isrs_gen.asm.inc

# Toolchains and tools
CC 					= ${TOOLS_DIR}/toolchain/x86_64-gcc/bin/x86_64-elf-gcc
LINK 				= ${TOOLS_DIR}/toolchain/x86_64-gcc/bin/x86_64-elf-gcc
ASM 				= nasm
QEMU 				= qemu-system-x86_64

# Compiler and tools flags
ASMFLAGS 			= -f elf64 -I./ -I${GEN_DIR}/ -DCPU_ARCH_AMD64 -g -F dwarf
CFLAGS_DEBUG 		= -O0 -g
CFLAGS_RELEASE 		= -O2 -s -DNDEBUG 				# TODO yehor.pohuliaka - Add support Debug and Release configurations
CFLAGS 				= ${CFLAGS_DEBUG} -c -ffreestanding -mcmodel=large -nostdlib -std=c99 -I./ -I${GEN_DIR}/ -DCPU_ARCH_AMD64
LDFLAGS 			= -T ${LD_SCRIPT} -nostdlib -z max-page-size=0x1000
LDLIBS 				= -lgcc
QEMUFLAGS 			= -serial stdio -m 4g 			# By defaul legacy BIOS; UEFI -> -bios /usr/share/ovmf/OVMF.fd
QEMUFLAGS_DEBUG 	= ${QEMUFLAGS} -s -S -no-shutdown -no-reboot

# Other stuff
CC_VERSION 			:= ${shell ${CC} --version 2>/dev/null}

# Grab all source files
C_GEN_SRC 			= ${C_ISRS_GEN_FILE}
ASM_GEN_SRC 		= ${ASM_ISRS_GEN_FILE}
C_SRC 				:= ${shell find . -name '*.c' -printf "%P\n"}
ASM_SRC 			:= ${shell find . -name '*.asm' -printf "%P\n"}
C_SRC 				:= ${filter-out ${GEN_DIR}/%, ${C_SRC}} ${C_GEN_SRC}
ASM_SRC 			:= ${filter-out ${GEN_DIR}/%, ${ASM_SRC}} ${ASM_GEN_SRC}
ASM_SRC 			:= ${filter-out %.asm.inc, ${ASM_SRC}}
C_OBJS 				:= ${patsubst %.c, ${BUILD_DIR}/%.o, ${C_SRC}}
ASM_OBJS 			:= ${patsubst %.asm, ${BUILD_DIR}/%.asm.o, ${ASM_SRC}}

.PHONY: clean verifytools

# Kernel prepare
kernel_prepare:
ifdef CC_VERSION
	@echo "Toolchain: ${CC_VERSION}"
else
	@echo -e '\033[0;31m${CC} was not found! Please use "make tools" to compile the toolchain!\033[0m'
	@exit 1
endif

# Generate code to build kernel (ISRs and other stuff)
kernel_gencode:
	@echo ${C_SRC}
	@chmod +x ${TOOLS_DIR}/kernel/generate_isrs.sh
	@${TOOLS_DIR}/kernel/generate_isrs.sh x86 ${C_ISRS_GEN_FILE} ${ASM_ISRS_GEN_FILE}

# Build kernel
kernel: verifytools kernel_gencode ${C_OBJS} ${ASM_OBJS}
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
	@chmod +x ${TOOLS_DIR}/kernel/make_disk.sh
	@${TOOLS_DIR}/kernel/make_disk.sh ${TARGET_DIR} ${TARGET_ISO}

# Build tools (autotools, binutils and gcc)
tools:
	@chmod +x ${TOOLS_DIR}/toolchain/compile_toolchain.sh
	@${TOOLS_DIR}/toolchain/compile_toolchain.sh x86_64

# Run QEMU
qemu: disk 
	${QEMU} ${QEMUFLAGS} -hda ${TARGET_ISO}

# Run QEMU for debug
qemu_dbg: disk
	@chmod +x ${TOOLS_DIR}/debug/gdb.sh
	${QEMU} ${QEMUFLAGS_DEBUG} -hda ${TARGET_ISO} & ${TOOLS_DIR}/debug/gdb.sh ${OUTPUT_KERNEL}

# Clean
clean:
	@chmod +x ${TOOLS_DIR}/kernel/clean_disk.sh
	@${TOOLS_DIR}/kernel/clean_disk.sh ${TARGET_DIR} ${TARGET_ISO}
	@rm -rf ${OUTPUT_KERNEL}
	@rm -rf ${GEN_DIR}
	@rm -rf ${BUILD_DIR}

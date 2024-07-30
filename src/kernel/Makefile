# Path to directories
TOOLS 	= ../../tools/
TARGET 	= ../../target/

# Toolchains
CC 		= ${TOOLS}/toolchain/gcc/bin/i686-elf-gcc
LINK 	= ${TOOLS}/toolchain/gcc/bin/i686-elf-ld
ASM 	= nasm

# Compiler flags
ASMFLAGS = -f elf32
CFLAGS 	= -c -ffreestanding -nostdlib -std=c99
LDFLAGS = -T link.ld -nostdlib
LDLIBS 	=

# File names
TARGET_IMG 		= ../../disk.img
OUTPUT_KERNEL 	= ${TARGET}/boot/kernel.bin

# Grab all source files
C_SRC 		:= ${shell find . -name '*.c' -printf "%P\n"}
ASM_SRC 	:= ${shell find . -name '*.asm' -printf "%P\n"}
C_OBJS 		:= ${patsubst %.c,%.o,${C_SRC}}
ASM_OBJS 	:= ${patsubst %.asm,%.asm.o,${ASM_SRC}}

# Build kernel
all: $(C_OBJS) $(ASM_OBJS)
	@mkdir -p ${TARGET}
	@mkdir -p ${TARGET}/boot
	${LINK} ${LDFLAGS} -o ${OUTPUT_KERNEL} ${C_OBJS} ${ASM_OBJS} ${LDLIBS}

# Compile source code (C and ASM)
%.o: %.c
	${CC} ${CFLAGS} ${subst .o,.c,$@} -o $@

%.asm.o: %.asm
	${ASM} ${ASMFLAGS} -o $@ ${subst .asm.o,.asm,$@}

# Build disk image
disk: all
	grub-mkrescue -d /usr/lib/grub/i386-pc/ -o ../../disk.iso ${TARGET}

# Build tools (autotools, binutils and gcc)
tools:
	@chmod +x ${TOOLS}/toolchain/compile_toolchain.sh
	@${TOOLS}/toolchain/compile_toolchain.sh

# Run QEMU
qemu:
	qemu-system-x86_64 -hda ../../disk.iso

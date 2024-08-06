#!/usr/bin/bash
set -e 			# Fail globally

# Print usage
if [ $# -le 1 ]; then
	echo "Usage: make_disk.sh <target_dir> <output_disk>"
	exit 1
fi

# Get path to script directory 
SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

# Script arguments and variables
TARGET_DIR=$1
OUTPUT_DISK=$2
LIMINE_DIR="${SCRIPTPATH}/../../src/bootloader/limine/"
LIMINE_EXEC="${LIMINE_DIR}/limine"

# Check that we have Limine executable file
if [ ! -f "${LIMINE_EXEC}" ]; then
	echo "To make dist firstly you have to compile bootloader (Limine)"
	exit 1
fi

# Copy our and Limine files into TARGET_DIR
mkdir -p ${TARGET_DIR}/boot/limine
cp ${LIMINE_DIR}/limine-bios.sys ${TARGET_DIR}/boot/limine
cp ${LIMINE_DIR}/limine-bios-cd.bin ${TARGET_DIR}/boot/limine
cp ${LIMINE_DIR}/limine-uefi-cd.bin ${TARGET_DIR}/boot/limine

# Create the EFI boot tree and copy Limine's EFI executables over
mkdir -p ${TARGET_DIR}/EFI/BOOT
cp ${LIMINE_DIR}/BOOTX64.EFI ${TARGET_DIR}/EFI/BOOT/
cp ${LIMINE_DIR}/BOOTIA32.EFI ${TARGET_DIR}/EFI/BOOT/

# Create the bootable ISO
xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        ${TARGET_DIR} -o ${OUTPUT_DISK}

# Install Limine stage 1 and 2 for legacy BIOS boot
${LIMINE_EXEC} bios-install ${OUTPUT_DISK}

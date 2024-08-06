#!/usr/bin/bash
set -e 			# Fail globally

# Print usage
if [ $# -le 1 ]; then
	echo "Usage: clean_disk.sh <target_dir> <target_iso>"
	exit 1
fi

# Get path to script directory 
SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

# Script arguments and variables
TARGET_DIR=$1
TARGET_ISO=$2

# Remove folders and files
rm -rf ${TARGET_DIR}/EFI/
rm -rf ${TARGET_DIR}/boot/limine/limine-bios.sys
rm -rf ${TARGET_DIR}/boot/limine/limine-bios-cd.bin
rm -rf ${TARGET_DIR}/boot/limine/limine-uefi-cd.bin
rm -rf ${TARGET_ISO}

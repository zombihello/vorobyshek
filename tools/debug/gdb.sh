#!/usr/bin/bash
set -e 			# Fail globally

# Print usage
if [ $# -le 0 ]; then
	echo "Usage: gdb.sh <debug_symbols>"
	exit 1
fi

# Get path to script directory 
SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

# Script variables
DEBUG_SYMBOLS=$1

# Launch GDB to debug our kernel
tilix -e "gdb -x ${SCRIPTPATH}/gdbinit -se ${DEBUG_SYMBOLS}"


#!/usr/bin/bash
set -e 			# Fail globally

# Print usage
if [ $# -le 0 ]; then
	echo "Usage: bochs.sh <target_iso>"
	exit 1
fi

# Get path to script directory 
SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

# Script variables
TARGET_ISO=$1

# Launch Bochs to debug our kernel
bochs-bin -f "${SCRIPTPATH}/bochsinit.bxrc" -q

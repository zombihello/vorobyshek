#!/usr/bin/bash
set -e 		# Fail globally

# Print usage
if [ $# -le 0 ]; then
	echo "Usage: compile_toolchain.sh <arch>"
	echo "Avaiable architectures:"
	echo " i686"
	echo " x86_64"
	exit 1
fi

# Set current directory at tools/toolchain
SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
cd "${SCRIPTPATH}"

# Script arguments
TARGET_ARCH=$1

# Modify environment veriables
export PREFIX="$SCRIPTPATH/${TARGET_ARCH}-gcc/"
export TARGET=${TARGET_ARCH}-elf
export PATH="$PREFIX/bin:$SCRIPTPATH/autotools/bin:$PATH"

mkdir -p "$PREFIX"

# Compile autotools if it need 
if ! test -f "$SCRIPTPATH/autotools/bin/automake"; then
	chmod +x ./compile_autotools.sh
	./compile_autotools.sh "1.16.5" "2.71" "$SCRIPTPATH/autotools"
fi

# Compile binutils/gcc if it need
if ! [ -f "$SCRIPTPATH/${TARGET_ARCH}-gcc/bin/${TARGET}-ld" ] || ! [ -f "$SCRIPTPATH/${TARGET_ARCH}-gcc/bin/${TARGET}-gcc" ]; then
	mkdir -p tmp_compile_toolchain
	cd tmp_compile_toolchain
	
	# Download tarrballs of binutils and gcc
	wget -nc https://ftp.gnu.org/gnu/binutils/binutils-2.40.tar.gz
	tar xpvf binutils-*.tar.gz
	wget -nc https://ftp.gnu.org/gnu/gcc/gcc-12.2.0/gcc-12.2.0.tar.gz
	tar xpvf gcc-*.tar.gz
	
	# Compile binutils
	cd binutils-2.40
	mkdir -p build
	cd build
	
	../configure --target="$TARGET" --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
	make -j$(nproc)
	make install
	cd ../../
	
	# Compile gcc
	cd gcc-12.2.0
	./contrib/download_prerequisites
	mkdir -p build
	cd build
	
	../configure --target="$TARGET" --prefix="$PREFIX" --disable-nls -enable-languages=c,c++ --without-headers
	make all-gcc -j$(nproc)
	make all-target-libgcc -j$(nproc)
	make install-gcc
	make install-target-libgcc
	
	cd ../../../
	rm -rf tmp_compile_toolchain
fi

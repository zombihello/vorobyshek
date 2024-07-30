#!/usr/bin/bash
set -x 		# Show commands
set -e 		# Fail globally

if [ "$#" -ne 3 ]; then
	echo "Usage: $0 <automake-version> <autoconf-version> <install-prefix>"
	exit 1
fi

AUTOMAKE_VERSION=$1
AUTOCONF_VERSION=$2
INSTALL_PREFIX=$3

if [ -e "$INSTALL_PREFIX" ]; then
	exit 0
fi

mkdir -p "$INSTALL_PREFIX"
mkdir tmp_compile_autotools
cd tmp_compile_autotools

BuildPackage()
{
	packageName=$1
	version=$2
	installDir=$3

	wget "https://ftp.gnu.org/gnu/$packageName/$packageName-$version.tar.gz"
	tar -xf "$packageName-$version.tar.gz"
	cd "$packageName-$version"
	./configure --prefix="$installDir"
	make
	make install
	cd ..
}

BuildPackage "automake" "$AUTOMAKE_VERSION" "$INSTALL_PREFIX"
BuildPackage "autoconf" "$AUTOCONF_VERSION" "$INSTALL_PREFIX"

cd ..
rm -rf tmp_compile_autotools

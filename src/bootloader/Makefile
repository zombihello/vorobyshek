.PHONY: all clean

# Build all
all: limine

# Build Limine bootloader
limine:
	git clone https://github.com/limine-bootloader/limine.git --branch=v8.x-binary --depth=1
	${MAKE} -C limine

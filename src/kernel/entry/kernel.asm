bits 32

; Declare constants for the multiboot header
MBOOT_HEADER_MAGIC 		equ 0x1BADB002
MBOOT_HEADER_FLAGS 		equ 0x00
MBOOT_HEADER_CHECKSUM 	equ -( MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS )

; Multiboot header
section .multiboot
	align 	4
	dd 		MBOOT_HEADER_MAGIC
	dd 		MBOOT_HEADER_FLAGS
	dd 		MBOOT_HEADER_CHECKSUM

; Stack
section .bss
	align 	16
stack_bottom:
	STACK_SIZE 			equ 0x4000 	; 16 KB
	resb STACK_SIZE
stack_top:

; Code section
section .text
global start
extern kernel_main 					; This function is gonna be located in our C code (kernel.c)
start:
	cli 							; Clears the interrupts
	mov esp, stack_top 				; Set the ESP register to point to the top of the stack (as it grows downwards on x86 systems)
	call kernel_main 				; Send processor to continue execution from the kernel_main function in C code

.halt:
	hlt 							; Halt the CPU (pause it from executing from this address)
	jmp .halt

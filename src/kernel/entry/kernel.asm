bits 32

section .text
	align 	4
	dd 		0x1BADB002
	dd 		0x00
	dd 		- ( 0x1BADB002 + 0x00 )

global start
extern kernel_main 			; This function is gonna be located in our C code (kernel.c)

start:
	cli 					; Clears the interrupts
	call kernel_main 		; Send processor to continue execution from the kernel_main function in C code
	hlt 					; Halt the CPU (pause it from executing from this address)

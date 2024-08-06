bits 32

; ISR stub without error
%macro ISR_NOERROR_STUB 1
global i686_isr_stub_%1:
i686_isr_stub_%1:
	push 0 					; Push dummy error code
	push %1 				; Push interrupt number
	jmp i686_asm_isr_handler
%endmacro

; ISR stub with error
%macro ISR_ERROR_STUB 1
global i686_isr_stub_%1:
i686_isr_stub_%1:
							; CPU pushes an error code to the stack
	push %1 				; Push interrupt number
	jmp i686_asm_isr_handler
%endmacro

%include "arch/i686/isrs_gen.asm.inc"

; ASM ISR handler
; NOTE: CPU pushes to the stack: SS, ESP, EFLAGS, CS, EIP
extern i686_isr_handler 		; ISR handler in C
i686_asm_isr_handler:
	; TODO yehor.pohuliaka - This function need to port on AMD64
	pusha 						; Pushes in order: EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI
	xor eax, eax 				; Push DS
	mov ax, ds
	push eax

	mov ax, 0x10 				; Use kernel data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	push esp 					; Pass pointer to stack to C, so we can access all the pushed information
	call i686_isr_handler
	add esp, 4

	pop eax 					; Restore old segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa 						; Pop what we pushed with pusha
	add esp, 8 					; Remove error code and interrupt number
	iret 						; Will pop: CS, EIP, EFLAGS, SS, ESP

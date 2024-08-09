%ifdef CPU_ARCH_I686
	bits 32
%elifdef CPU_ARCH_AMD64
	bits 64
%else
	%error "Unknown CPU architecture"
%endif

; ISR stub without error
%macro ISR_NOERROR_STUB 1
global x86_isr_stub_%1:
x86_isr_stub_%1:
	push 0 					; Push dummy error code
	push %1 				; Push interrupt number
	jmp x86_asm_isr_handler
%endmacro

; ISR stub with error
%macro ISR_ERROR_STUB 1
global x86_isr_stub_%1:
x86_isr_stub_%1:
							; CPU pushes an error code to the stack
	push %1 				; Push interrupt number
	jmp x86_asm_isr_handler
%endmacro

%include "arch/x86/isrs_gen.asm.inc"

; ASM ISR handler
; NOTE: CPU pushes to the stack: SS, ESP, EFLAGS, CS, EIP
extern x86_isr_handler 		; ISR handler in C
x86_asm_isr_handler:
%ifdef CPU_ARCH_I686
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
	call x86_isr_handler
	mov esp, eax

	pop eax 					; Restore old segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa 						; Pop what we pushed with pusha
	add esp, 8 					; Remove error code and interrupt number
	iret 						; Will pop: CS, EIP, EFLAGS, SS, ESP
%elifdef CPU_ARCH_AMD64
	push r15 					; Push registers on stack 
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push rbp
	push rdi
	push rsi
	push rdx
	push rcx
	push rbx
	push rax

	xor rax, rax 				; Push data segment
	mov ax, ds
	push rax

	mov ax, 0x10 				; Use kernel data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov rdi, rsp 				; Pass pointer to stack to C, so we can access all the pushed information
	call x86_isr_handler
	mov rsp, rax

	pop rax 					; Restore old data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	pop rax 					; Pop all that we pushed 
	pop rbx
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	pop rbp
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15

	add rsp, 16 				; Remove error code and interrupt number
	iretq 						; Will pop, CS, RIP, RFLAGS, SS, RSP
%else
	%error "Unknown CPU architecture"
%endif

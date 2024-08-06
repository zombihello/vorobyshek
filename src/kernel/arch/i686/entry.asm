bits 32

; Declare constants for the multiboot header
MBOOT_HEADER_MAGIC 		equ 0x1BADB002
MBOOT_HEADER_FLAGS 		equ 0x00000000
MBOOT_HEADER_CHECKSUM 	equ -( MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS )
KERNEL_OFFSET_MEM 		equ 0xFFFFFF8000000000

; Multiboot header
section .multiboot
	align 	4
	dd 		MBOOT_HEADER_MAGIC
	dd 		MBOOT_HEADER_FLAGS
	dd 		MBOOT_HEADER_CHECKSUM

; Code section
global start
extern kernel_main 						; This function is gonna be located in our C code (kernel.c)

; Booting of 32-bit architecture
%ifdef CPU_ARCH_I686
	start:
		cli 							; Disable interrupts
		mov esp, stack_top 				; Set the ESP register to point to the top of the stack (as it grows downwards on x86 systems)
		call kernel_main 				; Send processor to continue execution from the kernel_main function in C code
	.halt:
		hlt 							; Halt the CPU (pause it from executing from this address)
		jmp .halt

; Booting of 64-bit architecture
%elifdef CPU_ARCH_AMD64
extern g_GDTDescriptor
section .init.text
	; Kernel vierual memory offset
	KERNEL_OFFSET_MEM 			equ 0xFFFFFF8000000000

	; Kernel code and data segments in GDT
	; NOTE: If you change it you also need to update same values in arch/i686/gdt.h
	GDT_KERNEL_CODE_SEGMENT 	equ 0x08
	GDT_KERNEL_DATA_SEGMENT 	equ 0x10

	; Errors handling
	amd64_boot_panic:
		cli
		.halt:
			hlt
			jmp .halt

	; Check if CPUID is supported by attempting to flip the ID bit (bit 32) in
	; the FLAGS register. If we can flip it, CPUID is available
	amd64_boot_detectCPUID:
		pushfd 							; Copy FLAGS into EAX via the stack
		pop eax
		mov ecx, eax 					; Copy to ECX as well for comparing later on
		xor eax, 1 << 21 				; Flip the ID bit
		push eax 						; Copy EAX to FLAGS via the stack
		popfd
		
		pushfd 							; Copy FLAGS back to EAX (with the flipped bit if CPUID is supported)
		pop eax
		push ecx 						; Restore FLAGS from the old version stored in ECX (i.e flipping the ID bit back if it was ever flipped)
		popfd
		
		xor eax, ecx 					; Compare EAX and ECX. If they are equal then that means the bit wasn't flipped, and CPUID isn't supported
		jz amd64_boot_panic
		ret

	; Check if Long Mode is supported
	; The mode can only be detected using the extended functions of CPUID (>0x80000000),
	; so we have to check if the function that determines whether long mode is available or
	; not actually available
	amd64_boot_detectLongMode:
		; Check if the extended function is available
		mov eax, 0x80000000 						; Set the A-register to 0x80000000
		cpuid 										; CPU identification
		cmp eax, 0x80000001 						; Compare A-register with 0x80000001
		jb amd64_boot_panic 						; It is less, there is no long mode

		; Now that we know that extended function is available we can use it to detect long mode
		mov eax, 0x80000001 						; Set the A-register to 0x80000001
		cpuid 										; CPU identification
		test edx, 1 << 29 							; Test if the LM-bit, which is bit 29, is set in the D-register
		jz amd64_boot_panic							; They aren't there is no long mode
		ret

	; Setup paging
	amd64_boot_setupPaging:
		; Before we actually cover up the new paging used in x86_64 (amd64) we should disable the old paging first
		mov eax, cr0 								; Set the A-register to control register 0
		mov eax, 01111111111111111111111111111111b 	; Clear the PG-bit, which is bit 31
		mov cr0, eax 								; Set control register 0 to the A-register

		; Now that paging is disabled, we can actually take a look at how paging is set up in x86-64 
		; (It's recommended to read Chapter 5.3 of the AMD64 Architecture Programmer's Manual, Volume 2). 
		; First of all, long mode uses PAE paging and therefore you have the page-directory pointer table (PDPT), 
		; the page-directory table (PDT) and the page table (PT). There's also another table which now forms 
		; the root (instead of the PDPT or the PDT) and that is page-map level-4 table (PML4T). In protected mode 
		; a page table entry was only four bytes long, so you had 1024 entries per table. In long mode, however, 
		; you only have 512 entries per table as each entry is eight bytes long. This means that one entry in 
		; a PT can address 4kB, one entry in a PDT can address 2MB, one entry in a PDPT can address 1GB and 
		; one entry in a PML4T can address 512GB. This means that only 256TB can be addressed. The way 
		; these tables work is that each entry in the PML4T point to a PDPT, each entry in a PDPT to a PDT and 
		; each entry in a PDT to a PT. Each entry in a PT then points to the physical address, that is, if it is 
		; marked as present. So how does the MMU/processor know which physical address should be used with which 
		; virtual address? Basically each table has 512 entries ranging from 0 to 511. These entries each refer to 
		; a specific domain of memory. Like index 0 of the PML4T refers to the first 512GB in virtual memory, 
		; index 1 refers to the next 512GB and so on. The same applies to the PDPT, PDT and the PT 
		; (except with smaller sizes; 1GB, 2MB and 4kB as seen above). The last gigabyte of virtual memory would be 
		; described in the table referred to by 511th index of the PDPT which is referred to by the 511th index of the PML4T
			
		mov eax, PAGING_PML4T 
		mov cr3, eax 								; Set into control register 3 our paging tables
		
		; Now we should enable PAE-paging by setting the PAE-bit in
		; the fourth control register
		mov eax, cr4 								; Set the A-register to control register 4
		or eax, 1 << 5 								; Set the PAE-bit, which is the 6th bit (bit 5)
		mov cr4, eax 								; Set control register 4 to the A-register
		ret

	; Enable Long Mode
	amd64_boot_enableLongMode:
		; We should set the long mod bit in the EFER MSR and the we should
		; enable paging and then we are in compatibility mode (which is part of long mode)
		; So we first se the LM-bit
		mov ecx, 0xC0000080 						; Set the C-register to 0xC0000080, which is the EFER MSR
		rdmsr 										; Read from the model-specific register
		or eax, 1 << 8 								; Set the LM-bit which is the 9th bit (bit 8)
		wrmsr 										; Write to the model-specific register

		; Enabling paging
		mov eax, cr0 								; Set the A-register to control register 0
		or eax, 1 << 31 							; Set the PG-bit, which is the 32nd bit (bit 31)
		mov cr0, eax 								; Set control register 0 to the A-register
		ret
	
	; 32-bit entry point
	start:
		cli 										; Disable interrupts
		mov esp, stack_top-KERNEL_OFFSET_MEM 		; Set the ESP register to point to the top of the stack (as it grows downwards on x86 systems)
		mov ebp, esp
		
		; More about what is happening see here https://wiki.osdev.org/Setting_Up_Long_Mode
		call amd64_boot_detectCPUID 				; Check if CPUID is supported
		call amd64_boot_detectLongMode 				; Check if Long Mode is supported
		call amd64_boot_setupPaging 				; Setup paging
		call amd64_boot_enableLongMode 				; Eanble Long Mode
		
		; After enabling of the long mode we are in the 32-bit compatibility submode
		; and we actually wanted to enter 64-bit long mode.
		; For it we should just load a GDT with the 64-bit flags the in the code and the data selectors.
		lgdt [(g_GDTDescriptor-KERNEL_OFFSET_MEM)] 	; Load the 64-bit global descriptor table.
		jmp GDT_KERNEL_CODE_SEGMENT:(start64-KERNEL_OFFSET_MEM) 	; Set the code segment and enter 64-bit long mode.

section .text
bits 64
	; 64-bit entry point
	start64:
		; Now that we're in 64-bit
		mov eax, GDT_KERNEL_DATA_SEGMENT 			; Reload data segmen (kernel data)
		mov ds, eax
		mov es, eax
		mov fs, eax
		mov gs, eax
		mov ss, eax
		
		mov rsp, stack_top 							; Set the RSP register to point to the top of the stack (as it grows downwards on x86 systems)
		mov rbp, rsp
		
		; Jump to higher half memory (virtual memory)
		mov rax, upper_memory
		jmp rax
	
	; Trampline to move to higher half memory (virtual memory)
	upper_memory:
		call kernel_main 							; Send processor to continue execution from the kernel_main function in C code
		cli											; Disable interrupts
		.halt:
			hlt										; Halt the CPU (pause it from executing from this address)
			jmp .halt


; Section of Data
section .init.data
 	; Tables for memory paging
	; ----------------------------------------------------------------
	PAGING_FLAG_PRESENT 		equ 0x001
	PAGING_FLAG_WRITE 			equ 0x002
	PAGING_FLAG_GLOBAL       	equ 0x100
	PAGING_ENTRIES_PER_PT 		equ 512
	PAGING_PAGE_SIZE			equ 0x1000
	
	; PML4T[0] point to the PDPT and so on. The first two bits should be set,
	; these bits indicate that the page is present and that it is readable as well as writable
	; PML4T[0] 	--> PDPT
 	; PDPT[0] 	--> PDT
 	; PDT[0] 	--> PT
 	; PT 		--> 0x00000000 - 0x00200000
align PAGING_PAGE_SIZE
	PAGING_PT:
		%assign index 0
		%rep PAGING_ENTRIES_PER_PT
			dq ( index << 12 ) + ( PAGING_FLAG_PRESENT | PAGING_FLAG_WRITE | PAGING_FLAG_GLOBAL )
			%assign index index+1
		%endrep
	PAGING_PDT:
		dq PAGING_PT + ( PAGING_FLAG_PRESENT | PAGING_FLAG_WRITE | PAGING_FLAG_GLOBAL )
		times PAGING_ENTRIES_PER_PT-1 dq 0
	PAGING_PDPT:
		dq PAGING_PDT + ( PAGING_FLAG_PRESENT | PAGING_FLAG_WRITE | PAGING_FLAG_GLOBAL )
		times PAGING_ENTRIES_PER_PT-1 dq 0
	PAGING_PML4T:
		dq PAGING_PDPT + ( PAGING_FLAG_PRESENT | PAGING_FLAG_WRITE )
		times PAGING_ENTRIES_PER_PT-2 dq 0
		dq PAGING_PDPT + ( PAGING_FLAG_PRESENT | PAGING_FLAG_WRITE | PAGING_FLAG_GLOBAL )
%else
	%error "Unknown CPU architecture"
%endif

; Stack
section .bss
	align PAGING_PAGE_SIZE
stack_bottom:
	resb PAGING_PAGE_SIZE
stack_top:

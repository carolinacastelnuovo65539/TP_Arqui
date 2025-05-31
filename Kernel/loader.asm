global loader
extern main
extern initializeKernelBinary
extern getStackBase
GLOBAL load_main

loader:
	call initializeKernelBinary	; Set up the kernel binary, and get thet stack address
	mov rsp, rax				; Set up the stack with the returned address
	call main

load_main:
	call getStackBase	
	mov rsp, rax				
	call main

hang:
	cli
	hlt	; halt machine should kernel return
	jmp hang

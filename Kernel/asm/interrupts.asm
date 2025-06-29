
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _systemCallHandler

GLOBAL _exception0Handler
GLOBAL _exception6Handler
GLOBAL save_registers

GLOBAL regs
GLOBAL exc_regs

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN keyboardHandler
EXTERN syscall_dispatcher
EXTERN load_main

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1
	push rax
	
	mov [exc_regs+8*1], rbx
	mov [exc_regs+8*2], rcx
	mov [exc_regs+8*3], rdx
	mov [exc_regs+8*4], rsi
	mov [exc_regs+8*5], rdi
	mov [exc_regs+8*6], rbp
	mov [exc_regs+8*7], r8
	mov [exc_regs+8*8], r9
	mov [exc_regs+8*9], r10
	mov [exc_regs+8*10], r11
	mov [exc_regs+8*11], r12
	mov [exc_regs+8*12], r13
	mov [exc_regs+8*13], r14
	mov [exc_regs+8*14], r15

	
	mov rax, [rsp+8] ;cargo el rip

	mov [exc_regs + 120], rax
	
	mov rax, [rsp+16] ;cargo el cs
	mov [exc_regs + 128], rax
	
	mov rax, [rsp+24] ;cargo el rflags
	mov [exc_regs + 136], rax
	
	mov rax, [rsp+32] ;cargo el rsp
	mov [exc_regs + 144], rax
	pop rax

	mov[exc_regs], rax
	pushState

	mov rdi, %1 
	call exceptionDispatcher
	popState
	add rsp, 8
	push load_main
	iretq
%endmacro




_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = 16 bit mask
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:

	mov [backUpRegs+0], rax
	mov [backUpRegs+8*1], rbx
	mov [backUpRegs+8*2], rcx
	mov [backUpRegs+8*3], rdx
	mov [backUpRegs+8*4], rsi
	mov [backUpRegs+8*5], rdi
	mov [backUpRegs+8*6], rbp
	mov [backUpRegs+8*7], r8
	mov [backUpRegs+8*8], r9
	mov [backUpRegs+8*9], r10
	mov [backUpRegs+8*10], r11
	mov [backUpRegs+8*11], r12
	mov [backUpRegs+8*12], r13
	mov [backUpRegs+8*13], r14
	mov [backUpRegs+8*14], r15

	push rax
	mov rax, [rsp+8] ;cargo el rip

	mov [backUpRegs + 120], rax
	
	mov rax, [rsp+16] ;cargo el cs
	mov [backUpRegs + 128], rax
	
	mov rax, [rsp+24] ;cargo el rflags
	mov [backUpRegs + 136], rax
	
	mov rax, [rsp+32] ;cargo el rsp
	mov [backUpRegs + 144], rax
	pop rax

	irqHandlerMaster 1

save_registers:
	pushState
	mov rax, [backUpRegs + 0] ;cargo el rax
	mov [regs + 0], rax 
	mov rax, [backUpRegs + 8] ;cargo el rbx
    mov [regs + 8], rax
	mov rax, [backUpRegs + 16] ;cargo el rcx
    mov [regs + 16], rax
	mov rax, [backUpRegs + 24] ;cargo el rdx
    mov [regs + 24], rax
	mov rax, [backUpRegs + 32] ;cargo el rsi
    mov [regs + 32], rax
	mov rax, [backUpRegs + 40] ;cargo el rdi
    mov [regs + 40], rax
	mov rax, [backUpRegs + 48] ;cargo el rbp
    mov [regs + 48], rax
	mov rax, [backUpRegs + 56] ;cargo el r8
	mov [regs + 56], rax
	mov rax, [backUpRegs + 64] ;cargo el r9
    mov [regs + 64], rax
	mov rax, [backUpRegs + 72] ;cargo el r10
    mov [regs + 72], rax
	mov rax, [backUpRegs + 80] ;cargo el r11
    mov [regs + 80], rax
	mov rax, [backUpRegs + 88] ;cargo el r12
	mov [regs + 88], rax
	mov rax, [backUpRegs + 96] ;cargo el r13
    mov [regs + 96], rax
	mov rax, [backUpRegs + 104] ;cargo el r14
    mov [regs + 104], rax
	mov rax, [backUpRegs + 112] ;cargo el r15
    mov [regs + 112], rax
	mov rax, [backUpRegs + 120] ;cargo el rip
    mov [regs + 120], rax
	mov rax, [backUpRegs + 128] ;cargo el cs
	mov [regs + 128], rax
	mov rax, [backUpRegs + 136] ;cargo el rflags
	mov [regs + 136], rax
	mov rax, [backUpRegs + 144] ;cargo el rsp
	mov [regs + 144], rax
	popState
	ret

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0


;Invalid Opcode Exception
_exception6Handler:
	exceptionHandler 6

_systemCallHandler:
	pushState
	mov rbp, rsp
	push r9
	mov rcx, r10
	mov r9, rax
	call syscall_dispatcher
	pop r9

	mov al, 20h
	out 20h, al
	
	mov rsp, rbp
	popState
	iretq

haltcpu:
	cli
	hlt
	ret



SECTION .bss
	aux resq 1
	backUpRegs resq 19
	exc_regs resq 19
	regs resq 19

	
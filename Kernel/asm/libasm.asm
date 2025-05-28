GLOBAL cpuVendor
GLOBAL getSeconds
GLOBAL getMinutes
GLOBAL getHours

GLOBAL getKey
GLOBAL inb

GLOBAL key
GLOBAL flag

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

getSeconds:
	push rbp
	mov  rbp,rsp
	mov al, 0
	out 70h, al
	in al, 71h
	mov rsp,rbp
	pop rbp
	ret

getMinutes:
	push rbp
	mov  rbp,rsp
	mov al, 2
	out 70h, al
	in al, 71h
	mov rsp,rbp
	pop rbp
	ret

getHours:
	push rbp
	mov  rbp,rsp
	mov rax,0
	mov al, 4
	out 70h, al
	in al, 71h
	mov rsp,rbp
	pop rbp
	ret

getKey:
	mov rax, 0
	in al, 0x64
	mov cl, al
	and al, 0x01
	cmp al, 0
	in al, 0x60
	mov [key], al
	mov byte [flag], 1

inb:
	push rbp
	mov rbp, rsp
	mov rax, 0
	in al, dx
	mov rsp, rbp
	pop rbp
	ret

section .data
	key db 0
	flag db 1
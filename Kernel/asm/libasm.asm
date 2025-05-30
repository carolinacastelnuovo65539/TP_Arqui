GLOBAL cpuVendor
GLOBAL getSeconds
GLOBAL getMinutes
GLOBAL getHours

GLOBAL getKey
GLOBAL inb
GLOBAL outb



GLOBAL key
GLOBAL flag

GLOBAL rtc_bin



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

rtc_bin:
	push rbp
	mov rbp, rsp
	mov al, 0Bh 
	
	out 70h, al  
	in al, 71h
	
	mov bl, 4h
	or al, bl

	mov bl, al
	
	mov al, 0Bh
	
	out 70h, al
	
	mov al, bl
	
	out 71h, al

	mov rsp, rbp
	pop rbp
	ret


	
getSeconds:
	push rbp
	mov rbp, rsp

	call rtc_bin

    mov al, 0x00
    out 70h, al
    in al, 71h
	mov rsp, rbp
	pop rbp
    ret

getMinutes:
	push rbp
	mov rbp, rsp

	call rtc_bin

    mov al, 0x02
    out 70h, al
    in al, 71h

	mov rsp, rbp
	pop rbp
    ret

getHours:
	push rbp
	mov rbp, rsp

	call rtc_bin

    mov al, 0x04
    out 70h, al
    in al, 71h

	;pruebo con esto pero tampco anda
	movzx rax, al

	mov rsp, rbp
	pop rbp
    ret		

getKey:
	push rbp
	mov rbp, rsp
	xor rax, rax
	in al, 0x64
	mov cl, al
	and al, 0x01
	jz .end
	in al, 0x60
	mov [key], al
.end:
	mov rsp, rbp
	pop rbp
	ret

	
section .data
	key db 0
	flag db 1
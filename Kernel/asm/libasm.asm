GLOBAL cpuVendor


GLOBAL get_key
GLOBAL inb
GLOBAL outb



GLOBAL key
GLOBAL flag

GLOBAL rtc_bin
GLOBAL get_seconds
GLOBAL get_minutes
GLOBAL get_hours
GLOBAL sound
GLOBAL stop_sound

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


	
get_seconds:
	push rbp
	mov rbp, rsp

	call rtc_bin

    mov al, 0x00
    out 70h, al
    in al, 71h
	
	mov rsp, rbp
	pop rbp
    ret

get_minutes:
	push rbp
	mov rbp, rsp

	call rtc_bin

    mov al, 0x02
    out 70h, al
    in al, 71h

	mov rsp, rbp
	pop rbp
    ret

get_hours:
	push rbp
	mov rbp, rsp

	call rtc_bin

    mov al, 0x04
    out 70h, al
    in al, 71h


	mov rsp, rbp
	pop rbp
    ret		

get_key:
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

sound:
	push rbp
	push rdx
	mov rbp, rsp

	mov al, 0xB6
	out 0x43, al

	mov rdx, 0
	mov rax, 1193180
	div rdi

	out 0x42, al
	mov al, ah
	out 0x42, al

	in al, 0x61
	or al, 0x03
	out 0x61, al

	mov rsp, rbp
	pop rdx
	pop rbp
	ret

stop_sound:
	push rbp
	mov rbp, rsp
	
	in al, 0x61
	and al, 0xFC
	out 0x61, al

	mov rsp, rbp
	pop rbp
	ret
	
section .data
	key db 0
	flag db 1
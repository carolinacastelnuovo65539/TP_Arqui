GLOBAL sys_write
section .text

sys_write:
    mov rax, 0x01
    int 80h
    ret
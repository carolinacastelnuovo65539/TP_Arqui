GLOBAL sys_write
GLOBAL sys_read
GLOBAL sys_registersInfo
GLOBAL sys_getSeconds
GLOBAL sys_getMinutes
GLOBAL sys_getHours
GLOBAL sys_reduce
GLOBAL sys_clear
GLOBAL sys_increase

section .text

sys_read:
    mov rax, 0x00
    int 80h
    ret

sys_write:
    mov rax, 0x01
    int 80h
    ret

sys_getSeconds:
    mov rax, 0x03
    int 80h
    ret

sys_getMinutes:
    mov rax, 0x04
    int 80h
    ret

sys_getHours:
    mov rax, 0x05
    int 80h
    ret

sys_registersInfo:
    mov rax, 0x0A
    int 80h
    ret

sys_reduce:
    mov rax, 0x0B
    int 80h
    ret

sys_clear:
    mov rax, 0x02
    int 80h
    ret

sys_increase:
    mov rax, 0x0C
    int 80h
    ret
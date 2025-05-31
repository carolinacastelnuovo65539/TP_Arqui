GLOBAL sys_write
GLOBAL sys_read
GLOBAL sys_registersInfo
GLOBAL sys_get_seconds
GLOBAL sys_get_minutes
GLOBAL sys_get_hours
GLOBAL sys_reduce
GLOBAL sys_clear
GLOBAL sys_increase
GLOBAL sys_write_color
GLOBAL sys_drawRectangle
GLOBAL sys_drawCircle
GLOBAL sys_sound

section .text

sys_read:
    mov rax, 0x00
    int 80h
    ret

sys_write:
    mov rax, 0x01
    int 80h
    ret

sys_get_seconds:
    mov rax, 0x03
    int 80h
    ret

sys_get_minutes:
    mov rax, 0x04
    int 80h
    ret

sys_get_hours:
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

sys_drawRectangle:
    mov rax, 0x0D
    mov r10, rcx
    int 80h
    ret

sys_drawCircle:
    mov rax, 0x0E
    int 80h
    ret

sys_write_color:
    mov rax, 0x0F
    mov r10, rcx     ; fuente
    int 80h
    ret

sys_sound:
    mov rax, 0x10
    int 80h
    ret
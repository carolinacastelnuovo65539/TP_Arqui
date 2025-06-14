GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_clear
GLOBAL sys_get_seconds
GLOBAL sys_get_minutes
GLOBAL sys_get_hours
GLOBAL sys_wait
GLOBAL sys_registersInfo
GLOBAL sys_reduce
GLOBAL sys_increase
GLOBAL sys_drawRectangle
GLOBAL sys_drawCircle
GLOBAL sys_write_color
GLOBAL sys_sound
GLOBAL sys_get_day
GLOBAL sys_get_month
GLOBAL sys_get_year
GLOBAL sys_get_height
GLOBAL sys_get_width
GLOBAL sys_set_cursorX
GLOBAL sys_set_cursorY
GLOBAL sys_get_char_width
GLOBAL sys_get_char_height
GLOBAL sys_get_pressed_keys
GLOBAL sys_clean_buffer

section .text

sys_read:
    mov rax, 0x00
    int 80h
    ret

sys_write:
    mov rax, 0x01
    int 80h
    ret

sys_clear:
    mov rax, 0x02
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

sys_wait:
    mov rax, 0x06
    int 80h
    ret

sys_registersInfo:
    mov rax, 0x07
    int 80h
    ret

sys_reduce:
    mov rax, 0x08
    int 80h
    ret


sys_increase:
    mov rax, 0x09
    int 80h
    ret

sys_drawRectangle:
    mov rax, 0x0A
    mov r10, rcx
    int 80h
    ret

sys_drawCircle:
    mov rax, 0x0B
    mov r10, rcx     ; fuente
    int 80h
    ret

sys_write_color:
    mov rax, 0x0C
    mov r10, rcx     ; fuente
    int 80h
    ret

sys_sound:
    mov rax, 0x0D
    int 80h
    ret

sys_get_day:
    mov rax, 0x0E
    int 80h
    ret

sys_get_month:
    mov rax, 0x0F
    int 80h
    ret

sys_get_year:
    mov rax, 0x10
    int 80h
    ret

sys_get_height:
    mov rax, 0x11
    int 80h
    ret

sys_get_width:
    mov rax, 0x12
    int 80h
    ret

sys_set_cursorX:
    mov rax, 0x13
    int 80h
    ret
    
sys_set_cursorY:
    mov rax, 0x14
    int 80h
    ret

sys_get_char_width:
    mov rax, 0x15
    int 80h
    ret

sys_get_char_height:
    mov rax, 0x16
    int 80h
    ret

sys_get_pressed_keys:
    mov rax, 0x17
    int 80h
    ret

sys_clean_buffer:
    mov rax, 0x18
    int 80h
    ret
GLOBAL exc_zero
GLOBAL exc_opcode


section .text

exc_zero:
    mov rax, 0
    div rax
    ret

exc_opcode:
    mov cr6, rax
    ret 


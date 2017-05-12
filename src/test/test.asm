global _start
section .text;
_start:
    ;>++++
    inc r15
    add BYTE [data + r15], 4

    ;[<++++++++++++++++>-]
.loop0:
    dec r15
    add BYTE [data + r15], 16
    inc r15
    sub BYTE [data + r15], 1
    mov rdx, [data + r15]
    jnz .loop0

    ;<++++++++
    dec r15
    add BYTE [data + r15], 8

    ;.
    mov rax, 1
    mov rdi, 1
    mov rsi, data
    add rsi, r15
    mov rdx, 1
    syscall

    mov rax, 60
    mov rdi, 0
    syscall

section .bss:
data: resb 2
garbage: resb 1

.text
.global main
.extern printf
main:
    mov r1, #105
    mov r2, #110
loop:
    cmp r1, r2
    subgt r1, r1, r2 
    suble r2, r2, r1
    cmp r1, #0
    beq fillR1withGCD
    cmp r2, #0
    beq end
    b loop
fillR1withGCD:
    mov r1, r2
end:
    push {ip, lr}
    ldr r0, =string
    bl printf
    pop {ip, pc}
    mov pc,lr

.data
string: .asciz "GCD is : %d\n"
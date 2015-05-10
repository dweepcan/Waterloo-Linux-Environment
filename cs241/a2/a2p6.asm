sw $31, -4($30) ; save $31 on the stack
sw $1, -8($30)  ; save $4 on the stack
sw $2, -12($30) ; save $2 on the stack
sw $3, -16($30) ; save $3 on the stack
sw $4, -20($30) ; save $4 on the stack
sw $5, -24($30) ; save $5 on the stack
sw $6, -28($30) ; save $6 on the stack
lis $31
.word 28
sub $30, $30, $31

; load 1 into $3
lis $3
.word 1
; set $4 to output address
lis $4
.word 0xffff000c

; check if number is negative
slt $2, $1, $0 ; $2 = 1 if $1 < $0; 0 otherwise
bne $2, $3, 4 ; if ($2 != $3) pc += 12
; print - (ASCII 45)
lis $5
.word 45
sw $5, 0($4) ; MEM[$4] = $5
sub $1, $0, $1 ; $1 = $0 - $1

; Call recPrint
lis $6
.word recPrint
jalr $6
; print newline (ASCII 10)
lis $5
.word 10
sw $5, 0($4) ; MEM[$4] = $5

; Restore registers from the stack
lis $31
.word 28
add $30, $30, $31
lw $31, -4($30) ; restore $31 from stack
lw $1, -8($30) ; restore $1 from stack
lw $2, -12($30) ; restore $2 from stack
lw $3, -16($30) ; restore $3 from stack
lw $4, -20($30) ; restore $4 from stack
lw $5, -24($30) ; restore $5 from stack
lw $6, -28($30) ; restore $6 from stack

;Return to OS
jr $31

; recursively print a natural number
; assume that input (N) is in $1
; no output
recPrint:
; Save registers on the stack
sw $31, -4($30)
sw $1, -8($30)
sw $2, -12($30)
sw $3, -16($30)
sw $4, -20($30)
sw $5, -24($30)
lis $2
.word 24
sub $30, $30, $2

; load 10 into $2
lis $2
.word 10

; divide the value by 10 and find the remainder and quotient
divu $1, $2 ; lo = $1 / $2; hi = $1 % $2
mflo $1
mfhi $3

; Check to see if we are in the base case ($1 = 0)
beq $1, $0, done

; call the routine (i.e. ourself)
lis $2
.word recPrint
jalr $2

done:
; print the remainder
lis $5
.word 48
add $3, $3, $5 ; $3 = $3 + $5
sw $3, 0($4) ; MEM[$4] = $3

; restore registers
lis $2
.word 24
add $30, $30, $2
lw $31, -4($30)
lw $1, -8($30)
lw $2, -12($30)
lw $3, -16($30)
lw $4, -20($30)
lw $5, -24($30)
jr $31

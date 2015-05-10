sw $31, -4($30) ; save $31 on the stack
sw $1, -8($30) ; save $1 on the stack
sw $2, -12($30) ; save $2 on the stack
sw $3, -16($30) ; save $3 on the stack
sw $4, -20($30) ; save $4 on the stack
sw $5, -24($30) ; save $5 on the stack
sw $6, -28($30) ; save $6 on the stack
sw $7, -32($30) ; save $7 on the stack
sw $8, -36($30) ; save $8 on the stack
lis $31
.word 36
sub $30, $30, $31

bne $2, $0, 1; if ($2 != 0) pc += 4
jr $31

add $3, $2, $0 ; $3 = $2 + $0
add $4, $1, $0 ; $4 = $1 + $0

; load 1 into $5
lis $5
.word 1
; load 4 into $6
lis $6
.word 4

beq $3, $0, 7; if ($3 == $0) pc += 28
sub $3, $3, $5 ; $3 = $3 - $5
lw $1, 0($4) ; $7 = MEM[$4]
add $4, $4, $6 ; $4 = $4 + $6

;print 2's complement integer
lis $8
.word print
jalr $8

beq $0, $0, -8 ; if ($0 == $0) pc -= 32

;Return to OS
jr $31

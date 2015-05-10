lis $4
.word depth

lis $10
.word 1

lis $11
.word -1

add $12, $31, $0

lis $5
.word 0

jalr $4
jr $12

depth:
beq $5, $11, load; if ($5 == $11) 
; save registers
sw $31, -4($30)
sw $5, -8($30)
sw $6, -12($30)
sw $7, -16($30)
sw $8, -20($30)
sw $9, -24($30)
sw $10,-28($30)
sw $11, -32($30)
sw $12, -36($30)
sw $13, -40($30)
lis $31
.word 40
sub $30, $30, $31

lis $8
.word 4
mult $8, $5
mflo $8
add $8, $8, $1

; left node
lw $5, 4($8)
jalr $4
add $6, $3, $0

;right node
lw $5, 8($8)
jalr $4
add $7, $3, $0

slt $9, $7, $6
bne $9, $10, else;
add $3, $6, $10
beq $0, $0, ret
else:
add $3, $7, $10
ret:
lis $31
.word 40
add $30, $30, $31
; load registers
lw $31, -4($30)
lw $5, -8($30)
lw $6, -12($30)
lw $7, -16($30)
lw $8, -20($30)
lw $9, -24($30)
lw $10,-28($30)
lw $11, -32($30)
lw $12, -36($30)
lw $13, -40($30)
jr $31

load:
lis $3
.word 0
jr $31

.import print
.import init
.import new
.import delete
lis $4
.word 4
lis $11
.word 1
sw $31, -4($30)
sub $30, $30, $4
lis $6
.word init
jalr $6
add $29, $30, $0
sub $29, $29, $4
lis $5
.word 12
sub $30, $30, $5
sw $1, 0($29)
sw $2, -4($29)
add $3, $0, $11
sw $3, -8($29)
lis $3
.word 1
add $1, $3, $0
lis $5
.word new
jalr $5
bne $3, $0, 1
add $3, $11, $0
sw $3, -8($29)
lis $3
.word 1
add $1, $3, $0
lis $5
.word print
jalr $5
lis $3
.word 5
add $6, $3, $0
lw $3, -8($29)
sw $6, 0($3)
lis $3
.word 1
add $1, $3, $0
lis $5
.word print
jalr $5
lw $3, -8($29)
add $1, $3, $0
lis $5
.word print
jalr $5
lw $3, 0($3)
lis $5
.word 12
add $30, $30, $5
add $30, $30, $4
lw $31, -4($30)
add $29, $30, $0
jr $31

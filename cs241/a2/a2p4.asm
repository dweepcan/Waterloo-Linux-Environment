lw $3, 0($1) ; $3 = MEM[$1]
add $4, $2, $0 ; $4 = $2 + $0
add $5, $1, $0 ; $5 = $1 + $0

; load 1 into $6
lis $6
.word 1
; load 4 into $7
lis $7
.word 4

beq $4, $6, 7 ; if($4 == $6) pc += 28
sub $4, $4, $6 ; $4 = $4 - $6
add $5, $5, $7 ; $5 = $5 + $7 
lw $8, 0($5) ; $8 = MEM[$5]
slt $9, $3, $8 ; $9 = 1 if $3 < $8; 0 otherwise
beq $9, $0, 1 ; if ($9 == $0) pc += 4
add $3, $8, $0 ; $3 = $8 + $0 
beq $0, $0, -8 ; if ($0 == $0) pc -= 32

jr $31

bne $2, $0, 1; if ($2 != 0) pc += 4
jr $31

;set $10 to output address
lis $10
.word 0xffff000c

add $4, $2, $0 ; $4 = $2 + $0
add $5, $1, $0 ; $5 = $1 + $0

; load 1 into $6
lis $6
.word 1
; load 4 into $7
lis $7
.word 4

beq $4, $0, 13 ; if ($4 == $0) pc += 52
sub $4, $4, $6 ; $4 = $4 - $6
lw $8, 0($5) ; $8 = MEM[$5]
add $5, $5, $7 ; $5 = $5 + $7
bne $8, $0, 4 ; if ($8 != $0) pc += 16
;print space (ASCII 32)
lis $9
.word 32
sw $9, 0($10)
beq $0, $0, -9 ; if ($0 == $0) pc -= 36
;print uppercase characters (ASCII)
;load 64 into $9
lis $9
.word 64
add $8, $8, $9
sw $8, 0($10)
beq $0, $0, -14 ; if ($0 == $0) pc -= 56

jr $31

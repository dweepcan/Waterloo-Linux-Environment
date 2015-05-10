beq $2, $0, 7 ; if($2 == $0) pc += 7 * 4
; load 4 into $4
lis $4
.word 4
mult $4, $2 ; hi:lo = $4 * $2
mflo $4 ; $4 = lo
add $4, $1, $4 ; $4 = $1 + $4
lw $3, -4($4) ; $3 = MEM[$4]
jr $31 ; return to OS
; load -1 into $3
lis $3
.word -1
jr $31 ; return to OS

slt $4, $1, $2 ; $4=1 if $1 < $2; 0 otherwise
beq $4, $0, 2 ; if ($4 == $0) pc += 2
add $3, $2, $0 ; add $2+$0 into $3
jr $31 ; return to OS $31
add $3, $1, $0 ; add $1+$0 into $3
jr $31 ; return to OS $31

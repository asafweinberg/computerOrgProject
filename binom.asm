FIB:
    add $sp,$sp,$zero,$imm1,-3,0
    sw $zero,&sp,&imm1,&ra,2,0
    sw $zero,&sp,&imm1,&s0,1,0
    sw $zero,&sp,&imm1,&s1,0,0

    beq $zero,$a1,$zero,imm2,0,HALT_CONDITION
    beq $zero,$a0,$a1,$imm2,0,HALT_CONDITION

    add $s0,$a0,$zero,$zero,0,0
    add $s1,$a1,$zero,$zero,0,0
    sub $a0,$a0,$imm1,$zero,1,0
    sub $a1,$a1,$imm1,$zero,1,0
    jal $ra,$zero,$zero,imm2,0,FIB

    sub $a0,$s0,imm1,$zero,1,0
    add $a1,$s1,$zero,$zero,0,0
    add $s0,$v0,$zero,$zero,0,0
    jal $ra,$zero,$zero,imm2,0,FIB

    $add $v0,$v0,$s0,$zero,0,0
	beq $zero,$zero,$zero,$imm2,0,RETURN

HALT_CONDITION:
    add $v0,$zero,$zero,$imm1,1,0
RETURN:
    lw $ra,&sp,&imm1,&zero,2,0
    lw $s0,&sp,&imm1,&zero,1,0
    lw $s1,&sp,&imm1,&zero,0,0
    add $sp,$sp,$zero,$imm1,3,0

    beq $zero,$zero,$zero,$ra,0,0

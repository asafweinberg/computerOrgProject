	lw $a0, $zero, $imm1, $zero, 0x100, 0        #init a0 to n
	lw $a1, $zero, $imm1, $zero, 0x101, 0        #init a1 to k
	add $sp, $zero, $imm1, $zero, 2048, 0        #init sp to top of the stack
    jal $ra,$zero,$zero,$imm2,0,BIN              #jump to the recursive function, when returning to this point it means we are done
  	sw $v0, $zero, $imm1, $zero, 0x102, 0        #saving the result in v0 to the memory
	halt $zero, $zero, $zero, $zero, 0, 0        #halt

BIN:
    add $sp,$sp,$zero,$imm1,-3,0                 # handling the stack address
    sw $zero,$sp,$imm1,$ra,2,0                   #saving ra
    sw $zero,$sp,$imm1,$s0,1,0                   #saving s0
    sw $zero,$sp,$imm1,$s1,0,0                   #saving s1

    beq $zero,$a1,$zero,$imm2,0,HALT_CONDITION   #halt condition 1, if k == 0 then return
    beq $zero,$a0,$a1,$imm2,0,HALT_CONDITION     #halt condition 2, if k == n then return

    add $s0,$a0,$zero,$zero,0,0                   #moving n to s0
    add $s1,$a1,$zero,$zero,0,0                   #moving k to s1
    sub $a0,$a0,$imm1,$zero,1,0                   #substructing 1 from n
    sub $a1,$a1,$imm1,$zero,1,0                   #substructing 1 from k
    jal $ra,$zero,$zero,$imm2,0,BIN               #call BIN(n-1, k-1)
												  #s0 and s1 did not change in BIN(n-1, k-1)
    sub $a0,$s0,$imm1,$zero,1,0                   #executing a0 = n-1
    add $a1,$s1,$zero,$zero,0,0                   #moving k from s1 to a1
    add $s0,$v0,$zero,$zero,0,0                   #performing s0 = BIN(n-1, k-1) which is in v0
    jal $ra,$zero,$zero,$imm2,0,BIN               #call BIN(n-1, k)

    add $v0,$v0,$s0,$zero,0,0                     #performing v0 = BIN(n-1, k-1) + BIN(n-1, k)
	beq $zero,$zero,$zero,$imm2,0,RETURN		  #jump to return area, handling the stack on exit function

HALT_CONDITION:
    add $v0,$zero,$zero,$imm1,1,0                 #return 1 if halt condition is true
RETURN:
    lw $ra,$sp,$imm1,$zero,2,0                    #setting ra back to what it was before current BIN
    lw $s0,$sp,$imm1,$zero,1,0                    #setting s0 back to what it was before current BIN
    lw $s1,$sp,$imm1,$zero,0,0                    #setting s1 back to what it was before current BIN
    add $sp,$sp,$zero,$imm1,3,0                   #setting the top f the stack back

    beq $zero,$zero,$zero,$ra,0,0				  #return to ra
	
	.word 0x100 8
	.word 0x101 5

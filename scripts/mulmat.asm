X*Y=Z
lw $a0, $zero, $imm1, $zero, 0x100, 0 # load first,left matrix to a0
lw $a1, $zero, $imm1, $zero, 0x110, 0 # load second, right matrix to a1
add $s0, $zero, $zero, $zero, 0, 0 #initialize i=0
JZERO:
add $s1, $zero, $zero, $zero, 0, 0 #initialize j=0
KZERO:
add $s2, $zero, $zero, $zero, 0, 0 #initialize k=0
#open v0[i][j]
sll $t2, $s0, $imm1, $zero, 2, 0 #t2 = i*4(row size)
add $t2, $t2, $s1, $zero, 0, 0 #t2 = i*4 + j
sll $t2, $t2, $imm1, $zero, 3, 0 #t2 = byte offset of [i][j]
add $t2, $t2, $v0, $zero, 0, 0 #byte address of v0[i][j]
lw $t1, 0($t2), $zero, $zero, 0, 0 # t1 now has v0[i][j], assume it has zero value

LOADMULT:
#save a0[i][k]
add $t0, $zero, $zero, $zero, 0, 0 #make t2=0 again
sll $t0, $s0, $imm1, $zero, 2, 0 #t0 = i*4(row size)
add $t0, $t0, $s2, $zero, 0, 0 #t0 = i*4 + k
sll $t0, $t0, $imm1, $zero, 3, 0 #t0 = byte offset of [i][k]
add $t0, $t0, $a0, $zero, 0, 0 #byte address of a0[i][k]
lw $t3, 0($t0), $zero, $zero, 0, 0 # t3 now has a0[i][k]


#save a1[k][j]
add $t0, $zero, $zero, $zero, 0, 0 #make t0=0 again
sll $t0, $s2, $imm1, $zero, 2, 0 #t0 = k*4(row size)
add $t0, $t0, $s1, $zero, 0, 0 #t0 = k*4 + j
sll $t0, $t0, $imm1, $zero, 3, 0 #t0 = byte offset of [k][j]
add $t0, $t0, $a1, $zero, 0, 0 #byte address of a1[k][j]
lw $t4, 0($t0), $zero, $zero, 0, 0 # t4 now has a1[k][j]

add $s0, $s0, $imm1, $zero, 1, 0 #k+=1
bne $s0, $s0, $imm1, $imm2, 4, LOADMULT #if k<4, go back up and calculate next step
sw $0(t2), $zero, $t1, $zero, 0, 0 #save v0[i][j], because we are done calculating it

#increment j and at the same time make k zero
add $s1, $s1, $imm1, $zero, 1, 0 #j+=1
bne $s0, $s0, $imm1, $imm2, 4, KZERO  #if j<4, go back with j++ and make k zero
#increment i and at the same time make j,k zero
add $s0, $s0, $imm1, $zero, 1, 0 #i+=1
bne $s0, $s0, $imm1, $imm2, 4, JZERO #if i<4, go back with i++ and make j,k zero

#if we got here, than we need to return because i=4, j=4, k=4 and we're done
sw $v0, $zero, imm1, $zero, 0x120, 0 #save v0 matrix in the right place
beq $zero, $zero, $zero, $ra, 0, 0

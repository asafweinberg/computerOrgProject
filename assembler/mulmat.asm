add $s0, $zero, $imm1, $zero, 0x100, 0 # make s0 address of first, left matrix
add $s1, $zero, $imm1, $zero, 0x110, 0 # make s1 address of second, right matrix
add $s2, $zero, $imm1, $zero, 0x120, 0 # make s2 address of result matrix
sub $sp, $sp, $imm1, $zero, 4, 0 #clear space in stack
add $t2, $zero, $zero, $zero, 0, 0 #make t2 zero to put as i
sw $zero, $sp, $imm2, $t2, 0, 0 #initialize sp[4] as i=0

JZERO:
add $t2, $zero, $zero, $zero, 0, 0 #make t2 zero to put as j
sw $zero, $sp, $imm2, $t2, 0, 3 #initialize sp[1] as new value to save, as zero default
sw $zero, $sp, $imm2, $t2, 0, 1 #initialize sp[3] as j=0

KZERO:
add $t2, $zero, $zero, $zero, 0, 0 #make t2 zero to put as k
sw $zero, $sp, $imm2, $t2, 0, 3 #initialize sp[1] as new value to save, as zero default
sw $zero, $sp, $imm2, $t2, 0, 2 #initialize sp[2] as k=0

LOADMULT:
#save a0[i][k]
lw $t0, $sp, $imm2, $zero, 0, 0 #load to t0 the i value
sll $t0, $t0, $imm1, $zero, 2, 0 #t0 = i*4(row size)
lw $t2, $sp, $imm2, $zero, 0, 2 #load to t2 the k value
add $t0, $t0, $t2, $zero, 0, 0 #t0 = i*4 + k
add $s0, $s0, $t0, $zero, 0, 0 #byte address of a0[i][k]
lw $t0, $s0, $zero, $zero, 0, 0 # t0 now has a0[i][k]


#save a1[k][j], already stack is on k - sp[1]
lw $t1, $sp, $imm2, $zero, 0, 2 #load to t1 the k value
sll $t1, $t1, $imm1, $zero, 2, 0 #t1 = k*4(row size)
lw $t2, $sp, $imm2, $zero, 0, 1 #load to t2 the j value
add $t1, $t1, $t2, $zero, 0, 0 #t1 = k*4 + j
add $s1, $s1, $t1, $zero, 0, 0 #byte address of a1[k][j]
lw $t1, $s1, $zero, $zero, 0, 0 # t1 now has a1[k][j] TODO: check how it's written

#execute multiplication and save it
lw $t2, $sp, $imm2, $zero, 0, 3 #load t2 the previous value
mac $t2, $t0, $t1, $t2, 0, 0 #t2 = t0*t1 + prev_val of t2
sw $zero, $sp, $imm2, $t2, 0, 3 #save temp value in sp[1] 

#go 'back' to base addresess of s0, s1
lw $t0, $sp, $imm2, $zero, 0, 0 #load to t0 the i value
sll $t0, $t0, $imm1, $zero, 2, 0 #t0 = i*4(row size)
lw $t2, $sp, $imm2, $zero, 0, 2 #load to t2 the k value
add $t0, $t0, $t2, $zero, 0, 0 #t0 = i*4 + k
sub $s0, $s0, $t0, $zero, 0, 0 #went back to s0 base address

lw $t1, $sp, $imm2, $zero, 0, 2 #load to t1 the k value
sll $t1, $t1, $imm1, $zero, 2, 0 #t1 = k*4(row size)
lw $t2, $sp, $imm2, $zero, 0, 1 #load to t0 the j value
add $t1, $t1, $t2, $zero, 0, 0 #t1 = k*4 + j
sub $s1, $s1, $t1, $zero, 0, 0 #went back to s1 base address

#handle k
lw $t2, $sp, $imm2, $zero, 0, 2 #load to t2 the k value
add $t2, $t2, $imm1, $zero, 1, 0 #k+=1
sw $zero, $sp, $imm2, $t2, 0, 2 #save current k value in sp[3] 
bne $t2, $t2, $imm1, $imm2, 4, LOADMULT #if k<4, go back up and calculate next step

#save item s2[i][j]
lw $t2, $sp, $imm2, $zero, 0, 3 #load to t2 the desired s2[i][j] value
sw $zero, $s2, $zero, $t2, 0, 0 #save s2[i][j] in relevant address TODO: check how it's written
add $s2, $s2, $imm1, $zero, 1, 0 #increment s2 to next place

#increment j and at the same time make k zero
lw $t2, $sp, $imm2, $zero, 0, 1 #load to t0 the j value
add $t2, $t2, $imm1, $zero, 1, 0 #j+=1
sw $zero, $sp, $imm2, $t2, 0, 1 #save current j value in sp[3] 
bne $t2, $t2, $imm1, $imm2, 4, KZERO  #if j<4, go back with j++ and make k zero

#increment i and at the same time make j,k zero
lw $t2, $sp, $imm2, $zero, 0, 0 #load to t0 the i value
add $t2, $t2, $imm1, $zero, 1, 0 #i+=1
sw $zero, $sp, $imm2, $t2, 0, 0 #save current 0 value in sp[4] 
bne $t2, $t2, $imm1, $imm2, 4, JZERO #if i<4, go back with i++ and make j,k zero

#if we got here, than we need to return because i=4, j=4, k=4 and we're done
sub $s2, $s2, $imm1, $zero, 15, 0 #go back to 'first index'
sw $zero, $sp, $imm2, $zero, 0, 0 #save current 0 value in sp[4] 
sw $zero, $sp, $imm2, $zero, 0, 1 #save current 0 value in sp[3] 
sw $zero, $sp, $imm2, $zero, 0, 2 #save current 0 value in sp[2] 
sw $zero, $sp, $imm2, $zero, 0, 3 #save current 0 value in sp[1] 
add $sp, $sp, $imm1, $zero, 4, 0 #'return' space in stack
#beq $zero, $zero, $zero, $ra, 0, 0 #done
halt $zero, $zero, $zero, $zero, 0, 0 

.word 0x100 -1
.word 0x101 5
.word 0x102 -4
.word 0x103 3
.word 0x104 2
.word 0x105 20
.word 0x106 0
.word 0x107 9
.word 0x108 11
.word 0x109 -50
.word 0x10a 20
.word 0x10b 4
.word 0x10c 3
.word 0x10d 1
.word 0x10e -1
.word 0x10f 15
    
.word 0x110 2
.word 0x111 4
.word 0x112 6
.word 0x113 8
.word 0x114 20
.word 0x115 -9
.word 0x116 4
.word 0x117 5
.word 0x118 3
.word 0x119 0
.word 0x11a 0
.word 0x11b 32
.word 0x11c -1
.word 0x11d -2
.word 0x11e -3
.word 0x11f 4
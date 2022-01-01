add $s0, $imm1, $zero, $zero, 7, 0          # s0=7
                                            # temp = mem[0]

LOOP:                                       # s0 goes from 7 to 0

    blt $zero, $s0, $zero, $imm1, ENDLOOP, 0 # if s0<0 jump ENDLOOP
        
    out $zero, $imm1 , $zero, $s0, 15, 0    # disksector = s0
    out $zero, $imm1, $zero, $zero, 16, 0   # diskbuffer = MEM[0]
    out $zero, $imm1, $zero, $imm2, 14, 1   # diskcmd = read -> sector s0 is read to MEM[0]
    out $zero, $imm1, $zero, $imm2, 14, 0   # TODO MAYBE NOT NEED - diskcmd = 0 no command

    LOOP2:                                  # loop until diskstatus = 0
        in $s1, $imm1, $zero, $zero, 17, 0      # s1 = diskstatus
        beq $zero, $s1, $imm1, $imm2, 1, LOOP2  # if s1 == 1 jump LOOP2

    add $s1, $s0, $imm1 ,$zero, 1, 0        # s1= s0+1
    out $zero, $imm1 , $zero, $s1, 15, 0    # disksector = s1 = s0+1
    out $zero, $imm1, $zero, $zero, 16, 0   # diskbuffer = MEM[0]
    out $zero, $imm1, $zero, $imm2, 14, 2   # diskcmd = write -> write MEM[0] to sector s1
    out $zero, $imm1, $zero, $imm2, 14, 0   # TODO MAYBE NOT NEED - diskcmd = 0 no command

    LOOP3:                                  # loop until diskstatus = 0
        in $s1, $imm1, $zero, $zero, 17, 0      # s1 = diskstatus
        beq $zero, $s1, $imm1, $imm2, 1, LOOP3  # if s1 == 1 jump LOOP2

    sub $s0, $s0, $imm1, $zero, 1, 0         # s0--
    beq $zero, $zero, $zero, $imm1, LOOP, 0 # jump LOOP

ENDLOOP: halt $zero, $zero, $zero, $zero, 0, 0
 

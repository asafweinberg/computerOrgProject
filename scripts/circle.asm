

	lw $s0, $zero, $imm1, $zero, 0x100, 0		# $s0 = radius
	mac $s0, $s0, $s0, $zero, 0, 0 				# $s0 = radius^2
	add $t0, $zero, $zero, $zero, 0, 0			# t0 = 0
	

LOOP1:  # t0 = 0 to 255
		
		beq $zero, $t0, $imm1, $imm2 ,256 , END 		# jump END if t0==256
		add $t1, $zero, $zero, $zero, 0, 0				# t1 = 0

LOOP2:  # t1=0 to 255
		beq $zero, $t1, $imm1, $imm2 ,256 , ENDLOOP2 	# jump ENDLOOP2 if t1==256

		####### dx dy ####### 

		add $a0, $t0, $zero ,$zero ,0 ,0				# a0 = t0
		add $a1, $t1, $zero ,$zero ,0 ,0				# a1 = t1
		sub $a0, $a0, $imm1, $zero, 128, 0				# a0 = a0-128
		sub $a1, $a1, $imm1, $zero, 128, 0				# a1 = a1-128

		mac $a0, $a0, $a0, $zero, 0, 0	 				# a0 = a0^2 = dx^2
		mac $a1, $a1, $a1, $zero, 0, 0 					# a1 = a1^2 = dy^2
		add $a0, $a0, $a1, $zero, 0, 0					# a0 = a0+a1 = dx^2 + dy^2
		ble $zero, $a0, $s0, $imm1, TURNONPIXEL, 0		# if dx^2 + dy^2 <= radius^2 jump TURNONPIXEL

		####### dx+1 dy #######

		add $a0, $t0, $zero ,$zero ,0 ,0				# a0 = t0
		add $a1, $t1, $zero ,$zero ,0 ,0				# a1 = t1
		sub $a0, $a0, $imm1, $zero, 128, 0				# a0 = a0-128
		sub $a1, $a1, $imm1, $zero, 128, 0				# a1 = a1-128

		add $a0, $a0, $imm1, $zero, 1, 0				# a0++

		mac $a0, $a0, $a0, $zero, 0, 0	 				# a0 = a0^2 = dx^2
		mac $a1, $a1, $a1, $zero, 0, 0 					# a1 = a1^2 = dy^2
		add $a0, $a0, $a1, $zero, 0, 0					# a0 = a0+a1 = dx^2 + dy^2
		ble $zero, $a0, $s0, $imm1, TURNONPIXEL, 0		# if dx^2 + dy^2 <= radius^2 jump TURNONPIXEL

		####### dx dy+1 #######

		add $a0, $t0, $zero ,$zero ,0 ,0				# a0 = t0
		add $a1, $t1, $zero ,$zero ,0 ,0				# a1 = t1
		sub $a0, $a0, $imm1, $zero, 128, 0				# a0 = a0-128
		sub $a1, $a1, $imm1, $zero, 128, 0				# a1 = a1-128

		add $a1, $a1, $imm1, $zero, 1, 0				# a1++

		mac $a0, $a0, $a0, $zero, 0, 0	 				# a0 = a0^2 = dx^2
		mac $a1, $a1, $a1, $zero, 0, 0 					# a1 = a1^2 = dy^2
		add $a0, $a0, $a1, $zero, 0, 0					# a0 = a0+a1 = dx^2 + dy^2
		ble $zero, $a0, $s0, $imm1, TURNONPIXEL, 0		# if dx^2 + dy^2 <= radius^2 jump TURNONPIXEL

		####### dx+1 dy+1 #######

		add $a0, $t0, $zero ,$zero ,0 ,0				# a0 = t0
		add $a1, $t1, $zero ,$zero ,0 ,0				# a1 = t1
		sub $a0, $a0, $imm1, $zero, 128, 0				# a0 = a0-128
		sub $a1, $a1, $imm1, $zero, 128, 0				# a1 = a1-128

		add $a0, $a0, $imm1, $zero, 1, 0				# a0++
		add $a1, $a1, $imm1, $zero, 1, 0				# a1++

		mac $a0, $a0, $a0, $zero, 0, 0	 				# a0 = a0^2 = dx^2
		mac $a1, $a1, $a1, $zero, 0, 0 					# a1 = a1^2 = dy^2
		add $a0, $a0, $a1, $zero, 0, 0					# a0 = a0+a1 = dx^2 + dy^2
		ble $zero, $a0, $s0, $imm1, TURNONPIXEL, 0		# if dx^2 + dy^2 <= radius^2 jump TURNONPIXEL

AFTERCHECKS:

		add $t1, $t1, $imm1 ,$zero ,1 ,0				# t1++
		beq $zero, $zero, $zero, $imm1, LOOP2, 0		# jump LOOP2


ENDLOOP2: add $t0, $t0, $imm1 ,$zero ,1 ,0				# t0++
		beq $zero, $zero, $zero, $imm1, LOOP1, 0		# jump LOOP1

		
TURNONPIXEL:
		mac $s1, $t0, $imm1, $zero, 256, 0				# s1 = x*256
		add $s1, $s1, $t1, $zero, 0, 0					# s1 = x*256 + y
		out $zero, $imm1, $zero, $s1, 20, 0				# monitoraddr =  x*256 + y = s1
		out $zero, $imm1, $zero, $imm2, 21, 255			# monitordata = 255 (white)
		out $zero, $imm1, $zero, $imm2, 22, 1			# monitorcmd = 1
		out $zero, $imm1, $zero, $imm2, 22, 0			# monitorcmd = 0
		beq $zero, $zero, $zero, $imm1, AFTERCHECKS, 0	# jump AFTERCHECKS


END:
	halt $zero, $zero, $zero, $zero, 0, 0

.word 0x100 0xBb											# radius = 50 check

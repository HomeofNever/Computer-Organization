# hw3.s
# NAME: Xinhao Luo

        .data
newline:.asciiz "\n"
tab:    .asciiz "\t"
l_brckt:.asciiz "["
r_brckt:.asciiz "]"
endline:	.asciiz "\n"
msg1:	.asciiz "Please enter values for n, k, and m:\n"
msg2:	.asciiz "Please enter values for the "
first_message: .asciiz "first "
second_message: .asciiz "second "
msg3:	.asciiz "matrix ("
x:	.asciiz "x"
msg4:	.asciiz "):\n"
msg5:	.asciiz "multiplied by\n"
msg6:	.asciiz "equals\n"


        # global parameters n, k, and m (e.g., 4x3 multiply by 3x4),
        # which the user inputs in the matrix_size procedure
msg0:   .asciiz "Please enter values for n, k, and m:\n"
n:      .word 4
k:      .word 3
m:      .word 4
intsize: .word 4

################################################################################

        .text
        .globl main
        .globl matrix_multiply
        .globl matrix_print
        .globl matrix_ask
        .globl matrix_sizes
        j main # jump to main
################################################################################

matrix_sizes:
        # Ask the user for the matrix sizes, i.e., n, k, and m,
        # which correspond to multiplying an nxk matrix by a kxm matrix
        li $v0, 4
        la $a0, msg0 # Print N
        syscall
        
        li $v0, 5  # Var N
        syscall
        move $s0, $v0 
        li $v0, 5  # Var K
        syscall
        move $s1, $v0
        li $v0, 5  # Var M
        syscall
        move $s2, $v0
        
        jr $ra

################################################################################

matrix_ask:
        # Ask the user for the matrix values to store in
        # the memory address indicated by the $a0 register
        # $a1 row, $a2 col, $a3 word
       addi	$sp,$sp,-8
	sw	$ra,0($sp)
	sw	$a0,4($sp)
	
	li	$v0,4
	la	$a0,msg2
	syscall
	
	li	$v0,4
	move 	$a0, $a3
	syscall
	
	li	$v0,4
	la 	$a0, msg3
	syscall
	
	li	$v0,1
	move 	$a0, $a1
	syscall
	
	li	$v0,4
	la 	$a0, x
	syscall
	
	li	$v0,1
	move 	$a0, $a2
	syscall
	
	li	$v0,4
	la 	$a0, msg4
	syscall
	# END OF LINE
	
	lw	$a0,4($sp) # recover $a0
	li	$t0,0 # counter
	lw	$t1, intsize # int size
	mul	$t2,$a1,$a2 # total number
	move	$t3, $a0 # memory address
	read_loop:
		beq	$t0,$t2,exit_read_loop
		li	$v0, 5
		syscall
		sw	$v0, 0($t3)
		add	$t3, $t3, $t1
		addi	$t0,$t0,1
		j	read_loop
	exit_read_loop:
		lw	$ra,0($sp)
		lw	$a0,4($sp)
		addi	$sp,$sp,8
       jr $ra

################################################################################

main:
        # Initialize stack as necessary

        # Obtain user input values n, k, and m
        jal matrix_sizes

        # Allocate space for matrices
       	lw	$s7, intsize 
        # First Matrix $s0 * $s1
        mul $t0, $s0, $s1
        mul $t0, $t0, $s7 # times size
        move $a0, $t0
	li $v0, 9
	syscall
	move $s3, $v0 # First Save Address
        # Second Matrix $s1 * $s2
        mul $t0, $s1, $s2
        mul $t0, $t0, $s7
  	move $a0, $t0
	li $v0, 9
	syscall
	move $s4, $v0 # Second Save Address
        # result matrix (maybe we need 64)
        mul $t0, $s0, $s2
        mul $t0, $t0, $s7
        move $a0, $t0
	li $v0, 9
	syscall
	move $s5, $v0 # Result Save Address

	
	move $a0, $s3 # Memory address
	move $a1, $s0
	move $a2, $s1
	la   $a3, first_message
        # Get input for matrix A
        jal matrix_ask

	move $a0, $s4 # Memory address
	move $a1, $s1
	move $a2, $s2
	la   $a3, second_message
        # Get input for matrix B
        jal matrix_ask
        
	move $a0, $s3 # Memory address
	move $a1, $s4
	move $a2, $s5 # result
	# Get n, k, from S directly from save
        # Perform multiplication to get matrix C
        jal matrix_multiply

	# Separate
	li  $v0, 4
	la  $a0, endline
    	syscall
	
        # Output result
        move $a0, $s3
	move $a1, $s0
	move $a2, $s1
        jal matrix_print # First Matrix
        
        li	$v0,4
	la 	$a0, msg5
	syscall # MTB
	                 
        move $a0, $s4
	move $a1, $s1
	move $a2, $s2
        jal matrix_print # Second Matrix
        
        li	$v0,4
	la 	$a0, msg6
	syscall
	 
	move $a0, $s5
	move $a1, $s0
	move $a2, $s2
        jal matrix_print # Result Matrix

        # Cleanup stack and return
	li  $v0, 10
    	syscall

################################################################################

matrix_multiply:
        # multiply matrix A (address $a0) by  matrix B (address $a1),
        # storing the result in matrix C (address $a2)
       	# s0: n, s1: k, s2: m
       	
       	li $t0, 0 # Counter for first row
       	li $t1, 0 # Counter for first col
       	li $t2, 0 # Counter for the second col
       	move $t9, $s0 # A
       	move $t8, $s1 # B
       	move $t7, $s2 # Result
       	# T4, T5, T6 as temp
       	
       	loop_first: # loop by row in first matrix
		beq  	$s0,$t0,exit_loop_first
		li	$t1, 0 # reset first col counter
		loop_first_row:
			beq	$s1,$t1,exit_first_row
			li	$t2, 0 # reset second col counter
			loop_second_col:
				beq	$s2,$t2,exit_second_col
				# Get first matrix
				mul $t4, $s1, $t0
				add $t4, $t4, $t1
				mul $t4, $s7, $t4 # int size
				add $t4, $a0, $t4 # address
				lw $t4, 0($t4)
				
				# Get second matrix
				mul $t5, $s2, $t1
				add $t5, $t5, $t2
				mul $t5, $s7, $t5 # int size
				add $t5, $a1, $t5 # address
				lw $t5, 0($t5)
				
				
				# do multiple and save to $t4
				mul $t4, $t4, $t5
				
				# Get result matrix
				mul $t6, $s2, $t0 # row
				add $t6, $t6, $t2 # col
				mul $t6, $s7, $t6 # int size 
				add $t6, $a2, $t6 # address
				# Save word to t5, we need the address to save back to result matrix!
				lw $t5, 0($t6)
				
				# add 
				add $t5, $t5, $t4
				
				# save result
				sw $t5, 0($t6)
				
				# Increase counter
				addi $t2, $t2, 1
		
				j	loop_second_col
			exit_second_col:
				addi	$t1,$t1,1
				j	loop_first_row
		exit_first_row:
			addi	$t0,$t0,1
			j	loop_first
       	exit_loop_first: #
       		
        jr $ra

################################################################################

matrix_print:
        # print matrix (address $a0)
       addi	$sp,$sp,-8
	sw	$ra,0($sp)
	sw	$a0,4($sp) # address
	
	move $t0, $a1 # row
	move $t1, $a2 # col
	move $t2, $a0 # Memory address
	lw   $t3, intsize # size
	li   $t4, 0   # row counter
	li   $t5, 0   # col counter
	loop_row:
		beq  	$t4,$t0,exit_loop_row
		move	$t5, $zero # reset col counter
		li	$v0,4
		la	$a0,l_brckt
		syscall
	print_row:
		beq	$t1,$t5,exit_print_row
		
		lw	$a0,0($t2)
		li	$v0,1 #number
		syscall
		
		li	$v0,4
		la	$a0,tab
		syscall
		
		addi	$t5,$t5,1 # col counter
		add	$t2, $t2, $t3
		
		j	print_row
	exit_print_row:
		li	$v0,4
		la	$a0,r_brckt
		syscall
		
		li	$v0,4
		la	$a0,newline
		syscall
	
		addi	$t4,$t4,1
		j	loop_row
	
	exit_loop_row:
		lw	$ra,0($sp)
		lw	$a0, 4($sp)
		addi	$sp,$sp,8
		
	jr $ra #EOF
        

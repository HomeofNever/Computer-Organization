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
n:      .word 4
k:      .word 3
m:      .word 4
intsize: .word 4


txt1:   .asciiz "Please enter values for n, k, and m:\n"
txt2:   .asciiz "Please enter values for the first matrix ("
txt3:   .asciiz "Please enter values for the second matrix ("
txt4:   .asciiz "x"
txt5:   .asciiz "):"

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
        la $a0, txt1 # Print N
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
        addi	$sp,$sp,-4
	sw	$ra,0($sp)
	
	li	$v0,4
	la	$a0,msg2
	syscall
	
	li	$v0,4
	move 	$a0, $a3
	syscall
	
	li	$v0,4
	move 	$a0, msg3
	syscall
	
	li	$v0,4
	move 	$a0, msg3
	syscall
	
	
	li	$t0,0 #counter
	lw	$t1, intsize
	mul	$t2,$a1,$a2 # total number
	add	$t3, $s0, $zero # memory address
	ReadLoop:
		beq	$t0,$t2,ExitReadLoop
		sub	$t3, $t3, $t1
		li	$v0, 5
		syscall
		sw	$v0, 0($t3)
		addi	$t0,$t0,1
		j	ReadLoop
	ExitReadLoop:
	
	lw	$ra,0($sp)
	addi	$sp,$sp,4
        jr $ra

################################################################################

main:
        # Initialize stack as necessary

        # Obtain user input values n, k, and m
        jal matrix_sizes

        # Allocate space for matrices
        # First Matrix $s0 * $s1
        mul $t0, $s0, $s1
       	move $a0, $t0
	li $v0, 9
	syscall
	move $s3, $v0 # Save Address
        # Second Matrix $s1 * $s2
        mul $t0, $s1, $s2
  	move $a0, $t0
	li $v0, 9
	syscall
	move $s4, $v0 # Save Address
        # result matrix (maybe we need 64)
        mul $t3, $s0, $s2
        move $a0, $t0
	li $v0, 9
	syscall
	move $s5, $v0 # Save Address

	
	move $a0, $s3 # Memory address
	move $a1, $s0
	move $a2, $s1
        # Get input for matrix A
        jal matrix_ask
        
        move $a0, $s3
	move $a1, $s0
	move $a2, $s1
        jal matrix_print #Debug

	move $a0, $s4 # Memory address
	move $a1, $s2
	move $a2, $s3
        # Get input for matrix B
        jal matrix_ask
        
	move $a0, $s3 # Memory address
	move $a1, $s4
	move $a2, $s5 # result
	# Get n, k, from S directly?
        # Perform multiplication to get matrix C
        # jal matrix_multiply

	move $a0, $s3
	move $a1, $s0
	move $a2, $s2
        # Output result
        jal matrix_print

        # Cleanup stack and return

        jr $ra


################################################################################

matrix_multiply:
        # multiply matrix A (address $a0) by  matrix B (address $a1),
        # storing the result in matrix C (address $a2)

        jr $ra

################################################################################

matrix_print:
        # print matrix (address $a0)
        addi	$sp,$sp,-4
	sw	$ra,0($sp)
	
	move $t0, $a1 # row
	move $t1, $a2 # col
	move $t2, $a0 # Memory address
	lw   $t3, intsize # size
	li   $t4, 0   # row counter
	li   $t5, 0   # col counter
	loop_row:
		beq  	$t4,$t0,exit_loop_row
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
		sub	$t2, $t2, $t3
		
		j	print_row
	exit_print_row:
		li	$v0,4
		la	$a0,r_brckt
		syscall
		
		li	$v0,4
		la	$a0,newline
		syscall
	
		addi	$t4,$t4,1
		move	$t5, $zero # reset row counter
		j	print_row
	
	exit_loop_row:
		lw	$ra,0($sp)
		addi	$sp,$sp,4
	jr $ra #EOF
        

	.data
ask_integer: .asciiz "Enter positive integer: "
for:	.asciiz "For $"
for_1: 	.asciiz	", you need:\n"
quarter_s: .asciiz " quarters\n"
dime_s:	.asciiz " dime\n"
penny_s: .asciiz " pennies\n"
nickel_s: .asciiz " nickles\n"

divider: .float 100.0
penny:	.word 1
quarter: .word	25
dime:	.word 10
nickel:	.word 5

	.text
	.globl main
main:	
	la $a0, ask_integer
  	li $v0,4  # print_str
  	syscall
  	  	
  	li $v0, 5  # Var N
        syscall
        move $s0, $v0 
  	
  	move $a0, $s0
  	jal print_first
  	
  	move $a0, $s0
  	lw $a1, quarter
  	jal how_many
  	
  	move $a0, $v0
  	move $s0, $v1
  	beq $a0, $zero, skip_quarter
  	li $v0,1  # quarters
  	syscall

  	la $a0, quarter_s
  	li $v0,4  # print_str
  	syscall
  	
skip_quarter: 	
  	move $a0, $s0
  	lw $a1, dime
  	jal how_many
  	
  	move $a0, $v0
  	move $s0, $v1
  	beq $a0, $zero, skip_dime
  	li $v0,1  # dime
  	syscall
  	
  	la $a0, dime_s
  	li $v0,4  # print_str
  	syscall
skip_dime:
  	move $a0, $s0
  	lw $a1, nickel
  	jal how_many
  	
  	move $a0, $v0
  	move $s0, $v1
  	beq $a0, $zero, skip_nickel
  	li $v0,1  # nickel
  	syscall
  	
  	la $a0, nickel_s
  	li $v0,4  # print_str
  	syscall
skip_nickel:
  	move $a0, $s0
  	beq $a0, $zero, skip_penny
  	li $v0,1  # penny
  	syscall
  	
  	la $a0, penny_s
  	li $v0,4  # print_str
  	syscall
skip_penny:
  	li $v0, 10
  	syscall

print_first: #a0
	addi	$sp,$sp,-4
	sw	$a0,0($sp)
	
	la $a0, for
  	li $v0,4  # print_str
  	syscall     
  	
  	lw $a0, 0($sp)
  	mtc1 $a0, $f2
  	cvt.s.w $f2, $f2
  	l.s $f1, divider
  	div.s $f12, $f2, $f1
  	li $v0,2  # print_float
  	syscall 
  	
  	la $a0, for_1
  	li $v0,4  # print_str
  	syscall 
  	
	lw	$a0,0($sp)
  	addi	$sp,$sp,4
  	jr $ra
  	
  	
how_many: # a0, a1, v0, v1
	divu $a0,$a1
	mflo $v0
	mfhi $v1
	jr $ra

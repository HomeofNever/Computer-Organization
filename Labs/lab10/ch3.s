# lab10.s

	.data
array:	.word 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
tab:	.asciiz "\t"
newline:.asciiz "\n"

	.text
	.globl main

main:
	la $s0,array
	# 64 = 16 * 4
	li $s1,64
	add $s1,$s0,$s1
	j loop_header

loop_header:
	blt $s0,$s1,loop_body
	j loop_exit

loop_body:
	# lw $a0,0($s0)
	# li $v0,1
	# syscall
	# li $v0,4
	# la $a0,newline
	# syscall
    # 1
	lw $t0,0($s0)
    lw $t1,4($s0)
    lw $t2,8($s0)
    lw $t3,12($s0)
    # 2
    addi $t0,$t0,7
	addi $t1,$t1,7
    addi $t2,$t2,7
    addi $t3,$t3,7
    # 3
    sw $t0,0($s0)
	sw $t1,4($s0)
    sw $t2,8($s0)
    sw $t3,12($s0)
	# Loop Body
	# 12 instruction, no stall, no bubble
	# 16 Cycles

	# lw $a0,0($s0)
	# li $v0,1s
	# syscall
	# li $v0,4  
	# la $a0,newline
	# syscall
	j loop_latch

loop_latch:
	add $s0,$s0,16
	j loop_header
	# 16 + 3 = 19
	# 19 * 4 = 76 Cycles

loop_exit:
	jr $ra		# return to caller

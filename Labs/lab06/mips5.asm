# bit.s
#
# The bitcount procedure defined below counts
# the number of bits in given input $a0, returning
# this count in $v0
#
# Extend to display the entire bit string
#
# Also extend to iterate through a list of integers,
# calling the bitcount procedure for each integer

  .data
txt1: .asciiz "The number "
txt2: .asciiz " has "
txt3: .asciiz " bit"
txt4: .asciiz "s"
newline:.asciiz "\n"

zero: .asciiz "-"
one:  .asciiz "x"

secret: 
	.word 0x00001000 0x001c0000 0x00003c00 0x00fc0000 
	.word 0x00003f00 0x0ffc0000 0x00003fc0 0x3ff80000 
	.word 0x00003ff0 0xfff80000 0x00001ffd 0xfff80000 
	.word 0x00001fff 0xfff00000 0x07c01fff 0xfff00780 
	.word 0x03ffff00 0x01ffffc0 0x01fffc00 0x007fff00 
	.word 0x007ff07f 0xc01ffc00 0x003fe180 0x3f87f800 
	.word 0x000fc000 0x006fe000 0x007fc3f0 0x3f878000 
	.word 0x07ffcff8 0x7fc7f000 0x1fffcffc 0xffc3ffe0 
	.word 0x01ffce78 0xe7e3ff80 0x001fc7f0 0x7f83fc00 
	.word 0x0007c004 0x0003c000 0x003fc004 0x0003c000 
	.word 0x01ffc608 0x4063f000 0x007f0f88 0x87f0fe00 
	.word 0x00073fe7 0x1ffcf000 0x00073ff0 0x7ffc8000 
	.word 0x000f9fff 0xfff98000 0x001ff3ff 0xffe7e000 
	.word 0x0000787f 0xf819f000 0x0000ff00 0x00f40000 
	.word 0x000001ff 0xff800000 0x0000007f 0xfc000000 
	.word 0x0000007f 0xfc000000
  
  .text
  .globl printbit
  .globl bitcount
  .globl main

###################################################################

main: lw $ra,4($sp)
  lw $a0,0($sp)
  addi $sp,$sp,8

  #li $a0,8192
  #jal bitcount
  #j done

  la $a1,secret
  li $a2, 3
  jal loopmulti
  
  lw $ra,4($sp)
  lw $a0,0($sp)
  addi $sp,$sp,8
  jr $ra

###################################################################

printbit:
  # prints either a zero or one string based on what $a1 is
  addi $sp,$sp,-4 # save register $a0 on the stack
  sw $a0,0($sp) # (since we are going to change it)

  la $a0,zero
  beq $a1,$zero, merge # given value first, and then check if jump next
  la $a0,one
merge:  li $v0,4  # print_str
  syscall

  lw $a0,0($sp)
  addi $sp,$sp,4
  jr $ra

###################################################################

bitcount:
        # save registers $a0, $a1, and $ra on the stack
        addi $sp,$sp,-12
        sw $a0,0($sp)
        sw $a1,4($sp)
        sw $ra,8($sp)

loopinit:
        li $t0,0        # use $t0 to keep track of the number of 1 bits
        li $t1,32       # loop variable to iterate 32 times

loopbody:
        # mask (clear) all but the leftmost bit
        li $t2,0x80000000
        and $a1,$a0,$t2 # prepare argument $a1 for printbit procedure
        srl $t3, $a1, 31        # right shift the number to make it 1/0
        add $t0, $t0, $t3       # count this 1 bit
        jal printbit  # call the printbit procedure
        # beq $a1,$zero,skip      # if $a1 is 0, skip this bit
# skip:   
	sll $a0,$a0,1   # shift $a0 left one bit position
#latch:  
	sub $t1,$t1,1   # decrement loop variable
	
        bne $t1,$zero,loopbody

  move $v0,$t0  # return the result in $v0
  lw $ra,8($sp) # restore original $ra value
  lw $a1,4($sp) # restore original $a1 value
  lw $a0,0($sp) # restore original $a0 value
  addi $sp,$sp,12
  move $v0, $t0
  jr $ra    # return to caller (loop)
  
###################################################################

loopmulti:
 	addi   $sp,$sp,-8
        sw $a0,0($sp)   # save register $a1 on the stack
        sw $ra,4($sp)   # save return address $ra on the stack
        move $s0, $a2
        move $s1, $zero
        
repeat: addi   $sp,$sp,-4
	sw $ra,4($sp)   # save return address $ra on the stack
	
	lw $a0, 0($a1)
	jal bitcount
	
	lw $ra, 0($sp)	
	addi $sp,$sp,4
	
	add $s1, $s1, $v0 # number of x
	sub $s0, $s0, 1
	addi $a1, $a1, 4  # next word
	beq $s0, $zero, loopcounter # Back to top
	j repeat    
    
loopcounter:
	addi   $sp,$sp,-4
        sw $ra,4($sp)   # save return address $ra on the stack
        
	move $s0, $a2   # reset counter times
	move $a3, $s1
	move $s1, $zero # reset counter x
	jal printline
	
	lw $ra, 0($sp)	
	addi $sp,$sp,4
	
	beq $a0,$zero,done
	j repeat
	
printline:
	addi   $sp,$sp,-4
        sw $a0,0($sp)   # save register $a0 on the stack
        
  	# Debug number
  	li $v0,1  # print_int
  	move $a0, $a3
  	syscall
  	# New line
  	la $a0, newline
  	li $v0,4  # print_str
  	syscall
  	
  	lw $a0,0($sp)	 # restore $a0
  	addi $sp,$sp,4
  	jr $ra

done: 	lw $ra,4($sp)
  	lw $a0,0($sp)	
  	addi $sp,$sp,8
  	jr $ra

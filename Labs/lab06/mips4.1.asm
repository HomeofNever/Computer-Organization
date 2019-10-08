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
  .word 0x03c003c0 0x0ff00ff0 0x3ffc3ffc 0x7ffffffe 
  .word	0xffffffff 0xfffff3ff 0xff9cffff 0xff9cf3ff 
  .word	0x7fc9f3fe 0x1fe3f3f8 0x07f7f3e0 0x01ffff80 
  .word	0x007ffe00 0x001ff800 0x0007e000 0x00018000
  
  .text
  .globl printbit
  .globl bitcount
  .globl main

###################################################################

main:   addi $sp,$sp,-8
        sw $a0,0($sp)   # save register $a0 on the stack
        sw $ra,4($sp)   # save return address $ra on the stack

  #li $a0,8192
  #jal bitcount
  #j done

  la $a1,secret
repeat: lw $a0,0($a1)
  beq $a0,$zero,done
  jal bitcount
  addi $a1,$a1,4  # next word
  j repeat
done: lw $ra,4($sp)
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

  # Debug number
  li $v0,1  # print_int
  move $a0, $t0
  syscall
  # New line
  la $a0,newline
  li $v0,4  # print_str
  syscall

  move $v0,$t0  # return the result in $v0
  lw $ra,8($sp) # restore original $ra value
  lw $a1,4($sp) # restore original $a1 value
  lw $a0,0($sp) # restore original $a0 value
  addi $sp,$sp,12
  jr $ra    # return to caller (main)
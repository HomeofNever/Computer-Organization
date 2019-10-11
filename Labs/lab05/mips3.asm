      .data
list: .word 12,100,101,5,123456789,18,19

       .text
       .globl main
main:  li $a1 7
       la $a0,list
      
       li $s0, 0 # Counting Total Number
       li $s1, 0 # Counting Odd Number

       jal evenodd
       li $v0, 10              # terminate program run and
       syscall                 # Exit
evenodd:
      lw $a2 ($a0)
      andi $t0, $a2, 1
      add $s1, $s1, $t0 # add result to odd number
      addi $s0, $s0, 1 # increase 1 to total number
      addi $a0, $a0, 4 # increase address
      bne $s0, $a1, evenodd
      
      move $v0, $s1 # end of func
      jr $ra
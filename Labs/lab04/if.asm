

	.data 
strr:	.asciiz "x ($t0) equals "

       .text
       .globl main
main:  li $t0, 2
       bge $t0, 5, skip # compare
       add $t0, $t0, 3
skip:
       li $v0, 4
       la $a0, strr
       syscall         # print the string
       li $v0, 1
       move $a0, $t0
       syscall
#       jr $ra
       


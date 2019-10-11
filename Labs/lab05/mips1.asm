
	.data 
even:	.asciiz "EVEN"
odd:	.asciiz "ODD"

       .text
       .globl main
main:  li $a0, 2
       jal isodd	       # Jump and link
       
       li $v0, 10              # terminate program run and
       syscall                 # Exit
isodd:
       li $v0, 4
       andi $t0, $a0, 1
       beqz  $t0, iseven
       la $a0, odd
       j endcall
iseven: 
       la $a0, even
endcall:
       syscall         # print str1
       move $v0 $t0
       jr $ra
	

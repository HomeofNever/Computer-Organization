

	.data 
str1:	.asciiz "GCD( "
str2:	.asciiz ", "
str3:	.asciiz " ) is " 

       .text
       .globl main
main:  li $t0, 45
       li $t1, 54
       move $t3, $t0
       move $t4, $t1	# Make Copy of origin value
       bgt $t1, $t0, loop
       move $t2, $t1	# Swap two value, ensure $t1 is larger one
       move $t1, $t0
       move $t0, $t2
       move $t2, $zero
       
loop:
       beq $t0, $1, print
       bgt $t1, $t0, aminusb	# if $t1 > $t0
       sub $t0, $t0, $t1
       j loop
aminusb:
       sub $t1, $t1, $t0
       j loop
       
print:
       li $v0, 4
       la $a0, str1
       syscall         # print str1
       
       li $v0, 1
       move $a0, $t3
       syscall         # print number 1

       li $v0, 4
       la $a0, str2
       syscall         # print str2

       li $v0, 1
       move $a0, $t4
       syscall         # print number 2
              
       li $v0, 4
       la $a0, str3
       syscall         # print str3
       
       
       li $v0, 1
       move $a0, $t1
       syscall         # print number 3
       
       jr $ra
       


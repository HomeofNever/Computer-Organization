	.data 
str1:	.asciiz "byte #"
str2:	.asciiz ": "
str3:	.asciiz "\n"

       .text
       .globl main
main:  li $a0, 0x4a889cf1
       move $t1, $a0
       li $t0, 0xff000000
       and $t0, $t1, $t0
       srl $t0, $t0, 24 # byte 1
       li $t5, 1
       jal print
       li $t0, 0x00ff0000
       and $t0, $t1, $t0
       srl $t0, $t0, 16 # byte 2
       li $t5, 2
       jal print
       li $t0, 0x0000ff00
       and $t0, $t1, $t0
       srl $t0, $t0, 8 # byte 3
       li $t5, 3
       jal print
       li $t0, 0x000000ff
       and $t0, $t1, $t0 # byte 4
       li $t5, 4
       jal print
              
       li $v0, 10              # terminate program run and
       syscall                 # Exit
       
print:
       li $v0, 4
       la $a0, str1
       syscall
       
       li $v0, 1
       move $a0, $t5
       syscall

       li $v0, 4
       la $a0, str2
       syscall
       
       li $v0, 1
       move $a0, $t0
       syscall
       
       li $v0, 4
       la $a0, str3
       syscall
       
       jr $ra

	

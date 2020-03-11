org 0000h
mov r3,#-8
mov r4,#16
mov r1,#40h
mov r0,#50h
mov 60H,#0
mov 61H,#0
mov 62H,#0
loop:
	 mov a,r3
	 jb acc.7,small		   ;负数进small
	 jnb acc.7,big	 	   ;正数进big
small:
	  mov @r1,a			   ;small操作是，负数进50h，并在61h计算负数个数
	  inc a
	  inc r1
	  inc r3
	  dec r4
	  inc 61h
	  jmp loop
zero: inc 62h			  ;如果为0，进入flag操作
	  jmp flag
big:  
	  jz zero			  ;big操作是，负数进40h，并在60h计算正数个数
 flag:
	  mov @r0,a			  ;flag操作，计算0个数
	  inc a
	  inc r0
	  inc r3
	  inc 60h
	  djnz r4,loop

end
 
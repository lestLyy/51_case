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
	 jb acc.7,small		   ;������small
	 jnb acc.7,big	 	   ;������big
small:
	  mov @r1,a			   ;small�����ǣ�������50h������61h���㸺������
	  inc a
	  inc r1
	  inc r3
	  dec r4
	  inc 61h
	  jmp loop
zero: inc 62h			  ;���Ϊ0������flag����
	  jmp flag
big:  
	  jz zero			  ;big�����ǣ�������40h������60h������������
 flag:
	  mov @r0,a			  ;flag����������0����
	  inc a
	  inc r0
	  inc r3
	  inc 60h
	  djnz r4,loop

end
 
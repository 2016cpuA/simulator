main:
set:	
	ADDI	%r4,%r0,0
	ADDI	%r5,%r0,32
	ADDI	%r6,%r0,8
	JAL	sum_greater
	J	return
sum_greater:	
.L0:	LW 	%r1,0(%r4)
	LW 	%r2,0(%r5)
	ADDI	%r3,%r0,$0
	ADDI 	%r4,%r4,$4
	ADDI	%r5,%r5,$4
	ADDI	%r7,%r0,$0
.L1:
	SLT	%r8,%r1,%r2
	BEQ	%r8,%r0,$2
.L2:
	ADD	%r3,%r3,%r1
	J	.L4
.L3:
	ADD	%r3,%r3,%r1
.L4:
	ADDI	%r7,%r7,$1
.L5:
	SLT	%r8,%r7,%r6
	BNE	%r8,%r0,-13
.L6:
	ADDI	%r1,%r3,0
.L7:
	JR	%r31
return:
	OUT	%r1

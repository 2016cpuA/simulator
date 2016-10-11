#fibloop.s
main:
	ADDI	%r3,%r0,$30
	ADDI	%r1,%r0,$0
	JAL	fibloop
	J	return
fibloop:
	ADDI	%r4,%r0,$2
	SLT	%r4,%r3,%r4
	BEQ	%r4,%r0,.GT1
.LE1:
	ADDI	%r3,%r0,$1
	JR	%r31
.GT1:
	SW	%r3,0(%r1)
	SW	%r31,4(%r1)
	ADDI	%r1,%r1,$8
	ADDI	%r3,%r3,$-1
	JAL 	fibloop
	SW	%r3,0(%r1)
	LW	%r3,-8(%r1)
	ADDI	%r1,%r1,$4
	ADDI	%r3,%r3,$-2
	JAL	fibloop
	ADDI	%r1,%r1,$-12
	LW	%r4,8(%r1)
	ADD	%r3,%r3,%r4
	LW	%r31,4(%r1)
	JR	%r31
return:

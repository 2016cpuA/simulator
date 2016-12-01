main:
	in	%r1
	sw	%r1,0(%r30)
	lw	%r2,1(%r30)
	out	%r2

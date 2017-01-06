.data
_MASK_MANTI:	
	.word	0x007fffff
.text
	.globl	min_caml_float_of_int	
min_caml_float_of_int:
	beq	%r1,%r0,.itof_ret_zero
	slt	%r2,%r1,%r0
	beq	%r2,%r0,.itof_search_upper
	sub	%r1,%r0,%r1
.itof_search_upper:
	addi	%r3,%r0,0x7f
	sll	%r3,%r3,24
	and	%r3,%r1,%r3
	beq	%r3,%r0,.itof_search_upper_l
.itof_search_upper_g:
	addi	%r3,%r0,1
	sll	%r3,%r3,30
	addi	%r4,%r0,0x80
	addi	%r5,%r0,0x7f
	addi	%r6,%r0,30
.itof_loop1:
	and	%r7,%r1,%r3
	bne	%r7,%r0,.itof_calc_manti
	srl	%r3,%r3,1
	srl	%r4,%r4,1
	srl	%r5,%r5,1
	addi	%r6,%r6,-1
	j	.itof_loop1
.itof_calc_manti:
	addi	%r7,%r6,-23
	and	%r4,%r1,%r4
	slt	%r4,%r0,%r4
	and	%r5,%r1,%r5
	slt	%r5,%r0,%r5
.itof_loop2:
	srl	%r1,%r1,1
	addi	%r7,%r7,-1
	slt	%r8,%r0,%r7
	bne	%r8,%r0,.itof_loop2
.itof_ret:
	and	%r7,%r4,%r5
	andi	%r8,%r1,1
	or	%r7,%r8,%r7
	add	%r1,%r1,%r7
	addi	%r8,%r0,1
	sll	%r8,%r8,24
	and	%r8,%r1,%r8
	beq	%r8,%r0,.itof_L1
	srl	%r1,%r1,1
	addi	%r6,%r6,1
.itof_L1:
	la	%r3,_MASK_MANTI
	lw	%r3,0(%r3)
	and	%r1,%r3,%r1
	addi	%r6,%r6,127
	andi	%r6,%r6,0xff
	sll	%r6,%r6,23
	sll	%r2,%r2,31
	or	%r1,%r1,%r6
	or	%r1,%r1,%r2
	sw	%r1,0(%r30)
	lwc1	%f0,0(%r30)
	jr	%r31
.itof_search_upper_l:
	addi	%r3,%r0,1
	sll	%r3,%r3,23
	addi	%r4,%r0,150
.itof_loop3:
	and	%r5,%r3,%r1
	bne	%r5,%r0,.itof_ret_l
	sll	%r1,%r1,1
	addi	%r4,%r4,-1
	j	.itof_loop3
.itof_ret_l:	
	la	%r3,_MASK_MANTI
	lw	%r3,0(%r3)
	and	%r1,%r3,%r1
	sll	%r4,%r4,23
	or	%r1,%r4,%r1
	sll	%r2,%r2,31
	or	%r1,%r2,%r1
	sw	%r1,0(%r30)
	lwc1	%f1,0(%r30)
	jr	%r31
.itof_ret_zero:
	sub.s	%f1,%f1,%f1
	jr	%r31
.data
	.align 2
_BUF:
	.word 0		#buffer本体
	.word 0		#カウンタ
.text
	.globl min_caml_print_char
min_caml_print_char:
	la	%r2,_BUF
	lw	%r3,0(%r2)
	lw	%r4,4(%r2)
	sll	%r3,%r3,8
	andi	%r1,%r1,255	#念の為マスク
	or	%r3,%r1,%r3
	addi	%r27,%r0,3
	slt	%r27,%r4,%r27
	beq	%r27,%r0,_print_char_flush_4
	addi	%r4,%r4,1
	sw	%r3,0(%r2)
	sw	%r4,4(%r2)
	jr	%r31
_print_char_flush_4:
	out	%r3
	ori	%r4,%r0,0
	ori	%r3,%r0,0
	sw	%r3,0(%r2)
	sw	%r4,4(%r2)
	jr	%r31
.data
	.align 2
RADIUS: #4.000000
	.word 0x40800000
MAX_FLOAT: #128.000000
	.word 0x43000000
_1:
	.word 0x3f800000
_1_5:
	.word 0x3fc00000
_0:
	.word 0x00000000
.text
.globl _min_caml_start
_min_caml_start:
	sub	%r0,%r0,%r0
	la	%r22,RADIUS
	lwc1	%f24,0(%r22)
	lwc1	%f25,1(%r22)
	lwc1	%f26,2(%r22)
	lwc1	%f27,3(%r22)
	addi	%r25,%r0,128
_print_header:
	addi	%r27,%r0,0x5031
	sll	%r27,%r27,16
	addi	%r27,%r27,0x200a
	out	%r27
	addi	%r27,%r0,0x3132
	sll	%r27,%r27,16
	addi	%r27,%r27,0x3820
	out	%r27
	out 	%r27
	addi	%r1,%r0,0x31
	jal	min_caml_print_char
	addi	%r1,%r0,0x0a
	jal	min_caml_print_char
#f24=4.0,f25=128.0,f26=1.0,f27=1.5,mem(r22+16)=0.
#r26=1,r25=128
	addi	%r24,%r0,0
#r23=x,r24=y
yloop:
	addi	%r23,%r0,0
	add	%r1,%r24,%r24
	jal	min_caml_float_of_int
	div.s	%f1,%f1,%f25
	sub.s	%f8,%f1,%f26
_xloop:
	add	%r1,%r23,%r23
	jal	min_caml_float_of_int
	div.s	%f1,%f1,%f25
	sub.s	%f9,%f1,%f27
#f8=ci,f9=cr
	lwc1	%f2,4(%r22)	
	lwc1	%f3,4(%r22)
	lwc1	%f4,4(%r22)
	lwc1	%f5,4(%r22)
	addi	%r2,%r0,8192
iloop:
#f2=zr,f3=zi,f4=zr2,f5=zi2,r2=i
	bne 	%r2,%r0,iloop_next
	addi	%r1,%r0,49
	jal	min_caml_print_char
	j	xloop_end
iloop_next:
	sub.s	%f0,%f4,%f5
	add.s	%f0,%f0,%f9
	add.s	%f1,%f2,%f2
	mul.s	%f1,%f1,%f3
	add.s	%f1,%f1,%f8
	mul.s	%f2,%f0,%f26
	mul.s	%f3,%f1,%f26
	mul.s	%f4,%f2,%f2
	mul.s	%f5,%f3,%f3
	add.s	%f0,%f4,%f5
	addi	%r2,%r2,-1	
	c.lt.s	%r1,%f24,%f0
	beq	%r1,%r0,iloop
	addi	%r1,%r0,48
	jal	min_caml_print_char
xloop_end:	
	addi	%r23,%r23,1
	slt	%r27,%r23,%r25
	bne	%r27,%r0,_xloop	
	addi	%r24,%r24,1
	slt	%r27,%r24,%r25
	bne	%r27,%r0,yloop
_print_char_flush:	
	la	%r2,_BUF
	lw	%r3,0(%r2)
	lw	%r4,1(%r2)
	addi	%r5,%r0,4
_print_char_flush_loop:		#4バイトに足りない分の空白を詰める(今回はそれで一応問題ない
	sll	%r3,%r3,8
	ori	%r3,%r3,0x20
	addi	%r4,%r4,1
	bne	%r4,%r5,_print_char_flush_loop
_print_char_flush_do:
	out	%r3
	sw	%r0,0(%r2)
	sw	%r0,1(%r2)
	halt

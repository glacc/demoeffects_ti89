| Assembly Source File
| Created 2020/6/13; 23:14:12

	.xdef	gsprite8

gsprite8:
	;#	D7.w PosX
	;#	D6.w PosY
	;#
	;#	A6 Target plane
	;#	A5 Sprite dat
	;#
	;#	D5.b RyT
	;#	D4.b AddX
	;#	D3.w Line dat
	bra	cmd_skip

.byte	65,66,67,68,0xEE,0xFF,0x12,0x34

cmd_skip:
	cmp.w	#160, %D7
	bge	gsp8_end2
	cmp.w	#100, %D6
	bge	gsp8_end2
	cmp.w	#-8, %D7
	ble	gsp8_end2
	cmp.w	#-8, %D6
	ble	gsp8_end2
	
	movem.w	%D3-%D6, -(%SP)
	movem.l %A5-%A6, -(%SP)
	
	move.b	%D7, %D4
	lsr.b	#3, %D4
	andi.b	#0xFE, %D4
	adda.l	%D4, %A6
	
	move.b	%D6, %D4
	andi.w	#0x00FF, %D4
	mulu	#30, %D4
	adda.l	%D4, %A6
	
	;#	D4.b ShiftX
	move.b	%D7, %D5
	andi.b	#0x07, %D5
	move.b	#8, %D4
	sub.b	%D5, %D4
		
	move.b	%D6, %D5
	addi.b	#8, %D5
	
	cmpi.b	#100, %D5
	blt	gsp8_chk1

	move.b	#100, %D5
gsp8_chk1:
	cmp.b	#92, %D7
	blt.b	gsp8_loopy2
	
gsp8_loopy1:
	move.b	(%A5)+, %D3
	andi.w	#0xFF, %D3
	lsl.w	%D4, %D3
	lsr.w	#8, %D3
	move.b	(%A6), %D3
	adda.l	#30, %A6
	addi.b	#1, %D6
	cmp.b	%D5, %D6
	bne	gsp8_loopy1
	
	bra	gsp8_end1
	
gsp8_loopy2:
	move.b	(%A5)+, %D3
	andi.w	#0xFF, %D3
	lsl.w	%D4, %D3
	move.w	(%A6), %D3
	adda.l	#30, %A6
	addi.b	#1, %D6
	cmp.b	%D5, %D6
	bne	gsp8_loopy2

gsp8_end1:
	movem.l (%SP)+, %A5-%A6
	movem.w	(%SP)+, %D3-%D6
	
gsp8_end2:
	rts
	
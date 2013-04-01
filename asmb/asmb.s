.file	"asmb.s"
.data
empty_string: .ascii "                "

.text
.globl asmb
.type	asmb, @function
asmb:
	movdqu (empty_string), %xmm11 # Compare reference
	movq $0, %rax	# Counter
	movq $0, %r8 	# Offset

loop:
	cmp %r8, %rsi
	jle end

	# read the input
	movdqu (%rdi, %r8), %xmm13
	pcmpeqb %xmm11, %xmm13
	pmovmskb %xmm13, %r11

	# Increase offset
	add $16, %r8

	# Shift the bitmask which is a single word(!!) correctly
	mov %r8b, %cl
	sub %sil, %cl
	jle no_shifting

	shl %cl, %r11w
no_shifting:
	popcnt %r11w, %r11w
	add %r11, %rax

	jmp loop

end:
	ret

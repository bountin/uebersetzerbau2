.file	"asma.s"
.data
empty_string: .ascii "                "

.text
.globl asma
.type	asma, @function
asma:
	movdqu (empty_string), %xmm12
	movdqu (%rdi), %xmm13
	pcmpeqb %xmm13, %xmm12
	pmovmskb %xmm12, %rax
	popcnt %rax, %rax
#	movq $0, %rax
	ret

	.file	"test_function.ll"
	.text
	.globl	f
	.align	16, 0x90
	.type	f,@function
f:                                      # @f
	.cfi_startproc
# BB#0:                                 # %entry
# !!!! -> [1]
	mov	rax, qword ptr [rip + llvm_gc_root_chain]
	mov	qword ptr [rsp - 24], __gc_f
	mov	qword ptr [rsp - 16], 0
	mov	qword ptr [rsp - 32], rax
	lea	rax, qword ptr [rsp - 32]
	mov	qword ptr [rip + llvm_gc_root_chain], rax
	mov	qword ptr [rsp - 8], 0
# !!!! -> [2]
	mov	rax, qword ptr [rsp - 32]
	mov	qword ptr [rip + llvm_gc_root_chain], rax
	ret
.Ltmp0:
	.size	f, .Ltmp0-f
	.cfi_endproc

	.type	.Lstr.1,@object         # @str.1
	.section	.rodata.str1.16,"aMS",@progbits,1
	.align	16
.Lstr.1:
	.asciz	"How is your progress?"
	.size	.Lstr.1, 22

	.type	.Lstr.2,@object         # @str.2
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr.2:
	.asciz	"Doudesuka?"
	.size	.Lstr.2, 11

	.type	llvm_gc_root_chain,@object # @llvm_gc_root_chain
	.section	.bss.llvm_gc_root_chain,"aGw",@nobits,llvm_gc_root_chain,comdat
	.weak	llvm_gc_root_chain
	.align	8
llvm_gc_root_chain:
	.quad	0
	.size	llvm_gc_root_chain, 8

	.type	__gc_f,@object          # @__gc_f
	.section	.rodata,"a",@progbits
	.align	16
# !!!! -> [3]
__gc_f:
	.long	2                       # 0x2
	.long	2                       # 0x2
	.quad	.Lstr.1
	.quad	.Lstr.2
	.size	__gc_f, 24


	.section	".note.GNU-stack","",@progbits

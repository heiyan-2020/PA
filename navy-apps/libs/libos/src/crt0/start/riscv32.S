.globl  _start
_start:
  move s0, zero
	li sp, 1
	slli sp,sp, 15
	addi sp, a0,0
  jal call_main

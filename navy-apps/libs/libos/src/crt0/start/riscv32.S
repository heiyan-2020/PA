.globl  _start
_start:
  move s0, zero
	li sp, 1
	slli sp,sp, 15
	add sp,sp,a0
  jal call_main

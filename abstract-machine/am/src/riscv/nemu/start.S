.section entry, "ax"
.globl _start
.type _start, @function

_start:
  mv s0, zero
#	mv sp, a0
  la sp, _stack_pointer
  jal _trm_init

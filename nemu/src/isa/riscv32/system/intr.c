#include <isa.h>

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
	//at present, pc is ecall itself. need to determine whether +4 according to the exception type.
	cpu.mepc = cpu.pc;
	cpu.mcause = NO;
  return epc;
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}

#include <isa.h>

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
	cpu.mepc = cpu.pc;
	printf("mepc is %x\n", cpu.mepc);
	cpu.mcause = NO;
  return epc;
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}

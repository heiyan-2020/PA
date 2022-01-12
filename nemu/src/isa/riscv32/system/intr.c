#include <isa.h>

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
	//at present, pc is ecall itself. need to determine whether +4 according to the exception type.
#ifdef CONFIG_ETRACE
	log_write("[ETRACE INFO] program interrupted at 0x%x, caused exception code %d.\n", epc, NO);
#endif
	cpu.mepc = cpu.pc;
	cpu.mcause = NO;
	word_t mie = (cpu.mstatus >> 3) & 0x1;
	cpu.mstatus = ((cpu.mstatus & ~(1 << 7)) | (mie << 7));
	cpu.mstatus = ((cpu.mstatus & ~(1 << 3)) | (0 << 3));
  return cpu.mtvec;
}

#define IRQ_TIMER 0x80000007
word_t isa_query_intr() {
	if (cpu.intr) {
		cpu.intr = false;
		return IRQ_TIMER;
	}
  return INTR_EMPTY;
}

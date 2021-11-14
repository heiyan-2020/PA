#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
	if (ref_r->pc != cpu.pc) {
		log_write("Difftest info: execute faild with instruction at 0x%x, caused by pc not equal. expected: 0x%x, but got: 0x%x\n", pc, ref_r->pc, cpu.pc);
		return false;
	}
	for (int i = 0; i < 32; i++) {
		if (ref_r->gpr[i]._32 != cpu.gpr[i]._32) {
			log_write("Difftest info: execute faild with instruction at 0x%x, caused by the %dth register not equal. expected: 0x%x, but got: 0x%x\n", pc, i, ref_r->gpr[i]._32, cpu.gpr[i]._32);
			return false;
		}
	}
	return true;
}

void isa_difftest_attach() {
}

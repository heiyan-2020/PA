#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
	for (int i = 0; i < 32; i++) {
		printf("ref's reg is 0x%x, while dut's is 0x%x\n", ref_r->gpr[i]._32, cpu.gpr[i]._32);
		if (ref_r->gpr[i]._32 != cpu.gpr[i]._32) {
			log_write("execute faild with instruction at 0x%x\n", pc);
			return false;
		}
	}
	return true;
}

void isa_difftest_attach() {
}

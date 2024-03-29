#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6",
	"mcause", "mstatus", "mepc", "satp"
};

void isa_reg_display() {
	for (int i = 0; i < 32; i ++) {
		printf("%s\t\t%x\t\t%d\n", regs[i], cpu.gpr[i]._32, cpu.gpr[i]._32);
	}
printf("%s\t\t%x\t\t%d\n", "pc", cpu.pc, cpu.pc);
printf("%s\t\t%x\t\t%d\n", "satp", cpu.satp, cpu.satp);
printf("%s\t\t%x\t\t%d\n", "mscratch", cpu.mscratch, cpu.mscratch);
printf("%s\t\t%x\t\t%d\n", "mstatus", cpu.mstatus, cpu.mstatus);
}

word_t isa_reg_str2val(const char *s, bool *success) {
		for (int i = 0; i < 32; i ++) {
			if (strcmp(regs[i], s + 1) == 0) {
				return cpu.gpr[i]._32;
			}
		}
		if (strcmp("pc", s + 1) == 0) {
			return cpu.pc;
		}
		if (strcmp("satp", s + 1) == 0) {
			return cpu.satp;
		}
		if (strcmp("mstatus", s + 1) == 0) {
			return cpu.mstatus;
		}
		*success = false;
		return 0;
}


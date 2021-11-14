
def_EHelper(ecall) {
	rtl_li(s, &(s->dnpc), isa_raise_intr(1, cpu.mtvec));
}
#define MTVEC 0x00000305
def_EHelper(csrrw) {
	rtlreg_t* system_reg;
	switch (id_src2->imm) {
		case MTVEC: system_reg = &cpu.mtvec;break;
		default: assert(0);
	}
	if (ddest != &cpu.gpr[0]._32) {
		rtl_addi(s, ddest, system_reg, 0);
	}
	rtl_addi(s, system_reg, dsrc1, 0);
}

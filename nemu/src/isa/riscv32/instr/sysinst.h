#define _a7 cpu.gpr[17]._32
#define _a0 cpu.gpr[10]._32
#define _a1 cpu.gpr[11]._32
#define _a2 cpu.gpr[12]._32
def_EHelper(ecall) {
	rtl_li(s, &(s->dnpc), isa_raise_intr(11, cpu.pc));
}
#define MTVEC 0x00000305
#define MCAUSE 0x00000342
#define MSTATUS 0x00000300
#define MEPC	0x00000341
#define SATP 0X00000180
#define MSCRATCH 	0x340
def_EHelper(csrrw) {
	rtlreg_t* system_reg;
	switch (id_src2->imm) {
		case MTVEC: system_reg = &cpu.mtvec;break;
		case MCAUSE: system_reg = &cpu.mcause;break;
		case MSTATUS: system_reg = &cpu.mstatus;break;
		case SATP: system_reg = &cpu.satp;break;
		case MEPC: system_reg = &cpu.mepc;break;
		case MSCRATCH: system_reg = &cpu.mscratch;break; 
		default: assert(0);
	}
	word_t tmp = *dsrc1;
	if (ddest != &cpu.gpr[0]._32) {
		rtl_addi(s, ddest, system_reg, 0);
	}
	rtl_addi(s, system_reg, &tmp, 0);
}

def_EHelper(csrrs) {
	rtlreg_t* system_reg;
	switch (id_src2->imm) {
		case MTVEC: system_reg = &cpu.mtvec;break;
		case MCAUSE: system_reg = &cpu.mcause;break;
		case MSTATUS: system_reg = &cpu.mstatus;break;
		case SATP: system_reg = &cpu.satp;break;
		case MEPC: system_reg = &cpu.mepc;break;
		case MSCRATCH: system_reg = &cpu.mscratch;break;
		default: assert(0);
	}
	rtl_addi(s, ddest, system_reg, 0);
}

def_EHelper(mret) {
	rtl_addi(s, &(s->dnpc), &(cpu.mepc), 0);	
//	word_t mpie = (cpu.mstatus >> 7) & 0x1;
	cpu.mstatus = ((cpu.mstatus & ~(1 << 3)) | (1 << 3));
	cpu.mstatus = ((cpu.mstatus & ~(1 << 7)) | (1 << 7));
}

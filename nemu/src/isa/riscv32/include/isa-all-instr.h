#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(lw) f(sw) f(inv) f(nemu_trap) f(addi) f(auipc) f(jal) f(jalr) f(add) f(sub) f(sltiu) f(beq) f(bne) f(sltu) f(myxor) f(myor) f(sb) f(srai) f(lbu) f(andi) f(sll) f(myand) f(xori) f(bge)

def_all_EXEC_ID();

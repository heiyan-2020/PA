#ifndef __CPU_IFETCH_H__

#include <memory/vaddr.h>

static inline uint32_t instr_fetch(vaddr_t *pc, int len) {
	printf("Debug info: pc is 0x%x\n", *pc);
  uint32_t instr = vaddr_ifetch(*pc, len);
  (*pc) += len;
  return instr;
}

#endif

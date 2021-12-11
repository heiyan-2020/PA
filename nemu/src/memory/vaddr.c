#include <isa.h>
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len) {
	if (isa_mmu_check(addr, len, MEM_TYPE_IFETCH) == MMU_TRANSLATE) {
		word_t ret = isa_mmu_translate(addr, len, MEM_TYPE_IFETCH);
		assert(ret == addr);
		addr = ret;
	}
  return paddr_read(addr, len);
}


word_t vaddr_read(vaddr_t addr, int len) {
	if (isa_mmu_check(addr, len, MEM_TYPE_READ) == MMU_TRANSLATE) {
		word_t ret =  isa_mmu_translate(addr, len, MEM_TYPE_READ);
		assert(ret == addr);
		printf("ret = 0x%x\n", ret);
		addr = ret;
	}
  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
if (isa_mmu_check(addr, len, MEM_TYPE_WRITE) == MMU_TRANSLATE) {
		word_t ret =  isa_mmu_translate(addr, len, MEM_TYPE_WRITE);
		assert(ret == addr);
		addr = ret;
	}
  paddr_write(addr, len, data);
}

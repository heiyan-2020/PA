#include <isa.h>
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len) {
	if (isa_mmu_check(addr, len, MEM_TYPE_IFETCH) == MMU_TRANSLATE) {
		addr = isa_mmu_translate(addr, len, MEM_TYPE_IFETCH);
	}
  return paddr_read(addr, len);
}


word_t vaddr_read(vaddr_t addr, int len) {
//				vaddr_t old = addr;
	if (isa_mmu_check(addr, len, MEM_TYPE_READ) == MMU_TRANSLATE) {
		addr =  isa_mmu_translate(addr, len, MEM_TYPE_READ);
	}
  word_t ret = paddr_read(addr, len);
	return ret;
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
	vaddr_t old = addr;
	if (isa_mmu_check(addr, len, MEM_TYPE_WRITE) == MMU_TRANSLATE) {
		addr =  isa_mmu_translate(addr, len, MEM_TYPE_WRITE);
	}
	if (addr == 0x82180406) {
		printf("0x%x\n",old);
	}
  paddr_write(addr, len, data);
}

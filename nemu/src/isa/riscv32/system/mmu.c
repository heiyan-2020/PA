#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>

int isa_mmu_check(vaddr_t vaddr, int len, int type) {
	word_t tmp_satp = (word_t)cpu.satp;
	if (tmp_satp >> 31) {
		return MMU_TRANSLATE;
	} else {
		return MMU_DIRECT;
	}
}
#define PGSIZE_WIDTH  12
#define PGTABLE_WIDTH 10
#define	VALID_MASK 1
paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type) {
	word_t va = (word_t)cpu.satp;
	printf("va = 0x%x\n", va);
	word_t pg_dic_num = va >> (PGSIZE_WIDTH + PGTABLE_WIDTH);
	word_t pg_table_num = (va >> (PGSIZE_WIDTH)) & ((1 << PGTABLE_WIDTH) - 1);
	word_t vaddr_offset = vaddr & ((1 << PGSIZE_WIDTH) - 1);
	word_t* pg_dic = (word_t*)(va << (PGTABLE_WIDTH));	
	printf("0x%x\n", pg_dic);
	if ((pg_dic[pg_dic_num] & VALID_MASK) == 0) {
			return MEM_RET_FAIL;
	}
	word_t* pg_table = (word_t*)(pg_dic[pg_dic_num] >> (PGSIZE_WIDTH)); 
	if ((pg_table[pg_table_num] & VALID_MASK) == 0) {
		return MEM_RET_FAIL;
	}
	return (pg_table[pg_table_num] & ~((1 << PGSIZE_WIDTH) - 1)) | vaddr_offset;
}

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
	printf("vaddr=0x%x\n", vaddr);
	word_t va = (word_t)cpu.satp;
	word_t pg_dic_num = vaddr >> (PGSIZE_WIDTH + PGTABLE_WIDTH);
	word_t pg_table_num = (vaddr >> (PGSIZE_WIDTH)) & ((1 << PGTABLE_WIDTH) - 1);
	word_t vaddr_offset = vaddr & ((1 << PGSIZE_WIDTH) - 1);
	paddr_t pg_dic = (paddr_t)(va << (PGSIZE_WIDTH));	
	word_t pg_dic_item = paddr_read(pg_dic + pg_dic_num * sizeof(word_t), sizeof(word_t));
	printf("pg_dic = 0x%x, pg_dic_num = 0x%x, pg_dic_item = 0x%x\n",pg_dic, pg_dic_num, pg_dic_item);
	if ((pg_dic_item & VALID_MASK) == 0) {
			return vaddr;
	}
	paddr_t pg_table = (paddr_t)(pg_dic_item >> (PGSIZE_WIDTH) << (PGSIZE_WIDTH)); 
	printf("pg_table=0x%x\n", pg_table);
	word_t pg_table_item = paddr_read(pg_table + pg_table_num * sizeof(word_t), sizeof(word_t));
	if ((pg_table_item & VALID_MASK) == 0) {
		return vaddr;
	}
	printf("mmu.c 32:pg_table=0x%x, pg_table_item=0x%x\n",pg_table ,pg_table_item);
	return (pg_table_item & ~((1 << PGSIZE_WIDTH) - 1)) | vaddr_offset;
}

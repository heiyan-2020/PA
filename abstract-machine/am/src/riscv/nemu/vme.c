#include <am.h>
#include <nemu.h>
#include <klib.h>

static AddrSpace kas = {};
static void* (*pgalloc_usr)(int) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static Area segments[] = {      // Kernel memory mappings
  NEMU_PADDR_SPACE
};

#define USER_SPACE RANGE(0x40000000, 0x80000000)

static inline void set_satp(void *pdir) {
  uintptr_t mode = 1ul << (__riscv_xlen - 1);
  asm volatile("csrw satp, %0" : : "r"(mode | ((uintptr_t)pdir >> 12)));
}

static inline uintptr_t get_satp() {
  uintptr_t satp;
  asm volatile("csrr %0, satp" : "=r"(satp));
  return satp << 12;
}

bool vme_init(void* (*pgalloc_f)(int), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  kas.ptr = pgalloc_f(PGSIZE);
  int i;
  for (i = 0; i < LENGTH(segments); i ++) {
    void *va = segments[i].start;
    for (; va < segments[i].end; va += PGSIZE) {
      map(&kas, va, va, 0);
    }
  }
  set_satp(kas.ptr);
  vme_enable = 1;

  return true;
}

void protect(AddrSpace *as) {
  PTE *updir = (PTE*)(pgalloc_usr(PGSIZE));
  as->ptr = updir;
  as->area = USER_SPACE;
  as->pgsize = PGSIZE;
  // map kernel space
  memcpy(updir, kas.ptr, PGSIZE);
}

void unprotect(AddrSpace *as) {
}

void __am_get_cur_as(Context *c) {
  c->pdir = (vme_enable ? (void *)get_satp() : NULL);
}

void __am_switch(Context *c) {
  if (vme_enable && c->pdir != NULL) {
    set_satp(c->pdir);
  }
}

#define PGSIZE_WIDTH  12
#define PGTABLE_WIDTH 10
#define	VALID_MASK 1
void map(AddrSpace *as, void *va, void *pa, int prot) {
	uint32_t* pg_dic = as->ptr;
	uint32_t pg_dic_num = (uint32_t)va >> (PGSIZE_WIDTH + PGTABLE_WIDTH);
	uint32_t pg_table_num = ((uint32_t)va >> (PGSIZE_WIDTH)) & ((1 << PGTABLE_WIDTH) - 1);
	if ((pg_dic[pg_dic_num] & VALID_MASK) == 0) {
		pg_dic[pg_dic_num] = (uint32_t)pgalloc_usr(PGSIZE);
		pg_dic[pg_dic_num] |= 1;
	}	
	//		printf("va=0x%x, pa=0x%x\n", va, pa);
	//		printf("pg_dic[num]=0x%x\n", pg_dic[pg_dic_num]);
	uint32_t* pg_table = (uint32_t*)(((((uint32_t)pg_dic[pg_dic_num]) >> (PGSIZE_WIDTH))) << (PGSIZE_WIDTH)); 
	pg_table[pg_table_num] = (uint32_t)pa;
	pg_table[pg_table_num] |= 1;
//	printf("pg_table=0x%x\n", pg_table);
}

Context *ucontext(AddrSpace *as, Area kstack, void *entry) {
	Context* newContext = kstack.end - sizeof(Context);
	newContext->mepc = (uintptr_t)entry;
	newContext->mstatus = 0x1808;
	newContext->pdir = as->ptr;
	newContext->gpr[2] = (uint32_t)newContext;
	asm volatile("csrw mscratch, %0": : "r"(newContext));
  return newContext;
}

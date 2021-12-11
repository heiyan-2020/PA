#include <memory.h>

static void *pf = NULL;

void* new_page(size_t nr_page) {
	assert(pf != NULL);
	if (nr_page == 0) return NULL;
	void* ret = pf;
 	pf += PGSIZE * nr_page;	
  return ret;
}

#ifdef HAS_VME
static void* pg_alloc(int n) {
	assert((n & (PGSIZE - 1)) == 0);
	int pg_num = n >> 12;
	void * pages = new_page(pg_num);
	memset(pages, 0, n);
  return pages;
}
#endif

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);

#ifdef HAS_VME
  vme_init(pg_alloc, free_page);
#endif
}

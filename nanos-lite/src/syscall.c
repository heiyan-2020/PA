#include <common.h>
#include "syscall.h"
#include <strace.h>
void sys_yield(Context *c) {
	yield();
	c->GPRx = 0;
}

void sys_exit(Context* c) {
	halt(c->GPR2);
	c->GPRx = 0;
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
#ifdef STRACE
	printf("[STRACE]\tcalled %s, with arguments\t0x%x\t0x%x\t0x%x,returned 0x%x\n", CALL_STRING[a[0]], c->GPR2, c->GPR3, c->GPR4, c->GPRx);
#endif
  switch (a[0]) {
		case SYS_exit:sys_exit(c);break;
		case SYS_yield: sys_yield(c);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

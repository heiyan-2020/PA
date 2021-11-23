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

  switch (a[0]) {
		case SYS_exit: sys_exit(c);log_write("[STRACE]called[SYS_exit], returned 0x%x\n", c->GPRx);break;
		case SYS_yield: sys_yield(c);log_write("[STRACE]called[SYS_yield], returned 0x%x\n", c->GPRx);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

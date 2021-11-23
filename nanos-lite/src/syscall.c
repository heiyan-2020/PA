#include <common.h>
#include "syscall.h"
void sys_yield(Context *c) {
	yield();
	c->GPRx = 0;
}

void sys_exit(Context* c) {
	halt(c->GPR2);
	c->GPRx = c->GPR2 & 0xFF;
}

void sys_write(Context* c) {
	int fd = c->GPR2;
	const char* buf = (char*)c->GPR3;
	size_t count = c->GPR4;
	c->GPRx = count;
	if (fd == 1 || fd == 2) {
		for (int i = 0; i < count; i++) {
			putch(buf[i]);
		}	
	}
	//Attention!I have not handled situation that the number of wtitten bytes less than count.
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
#ifdef STRACE
	if (a[0] < 0 || a[0] > 19) {
		panic("Illegal Syscall ID = %d\n", a[0]);
	}
	printf("[STRACE]\tcalled %s, with arguments\t0x%x\t0x%x\t0x%x,returned 0x%x\n", CALL_STRING[a[0]], c->GPR2, c->GPR3, c->GPR4, c->GPRx);
#endif
  switch (a[0]) {
		case SYS_exit:sys_exit(c);break;
		case SYS_yield: sys_yield(c);break;
		case SYS_write: sys_write(c);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

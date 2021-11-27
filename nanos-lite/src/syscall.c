#include <common.h>
#include <fs.h>
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
	if (fd == 1 || fd == 2) {
		for (int i = 0; i < count; i++) {
			putch(buf[i]);
		}	
		c->GPRx = count;
	} else {
		c->GPRx = fs_write(fd,(const void*) buf, count);
	}
}

void sys_read(Context* c) {
	int fd = c->GPR2;
	void* buf = (void*)c->GPR3;
	size_t count = c->GPR4;
	if (fd == 1 || fd == 2) {
		//ignore
		c->GPRx = 0;
	} else {
		c->GPRx = fs_read(fd, buf, count);
	}
}

void sys_open(Context* c) {
	const char* filepath = (char*)c->GPR2;
	int flags = c->GPR3;
	int mode = c->GPR4;
	c->GPRx = fs_open(filepath, flags, mode);
}

void sys_lseek(Context* c) {
	int fd = c->GPR2;
	size_t offset = c->GPR3;
	int whence = c->GPR4;
	if (fd == 1 || fd == 2) {
		//ignore
		c->GPRx = 0;
	} else {
		c->GPRx = fs_lseek(fd, offset, whence);
	}
}

void sys_brk(Context* c) {
	c->GPRx = 0;
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
		case SYS_brk: sys_brk(c);break;
		case SYS_open: sys_open(c);break;
		case SYS_read: sys_read(c);break;
		case SYS_lseek: sys_lseek(c);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

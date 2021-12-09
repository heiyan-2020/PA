#include <common.h>
#include <fs.h>
#include <sys/time.h>
#include "syscall.h"
#include <proc.h>
void naive_uload(PCB*, const char*);
void context_uload(PCB* proc, const char* pathname, char* const argv[], char* const envp[]);
void switch_boot_pcb();

int sys_execve(const char* pathname, char* const argv[], char* const envp[]) {
				printf("sys_execve\n");
	context_uload(current, pathname, argv, envp);
	switch_boot_pcb();
	yield();	
	assert(0);
}
void sys_yield(Context *c) {
	yield();
	c->GPRx = 0;
}

void sys_exit(Context* c) {
	sys_execve("/bin/menu", NULL, NULL);
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

void sys_close(Context* c) {
	c->GPRx = 0;
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

void sys_gettimeofday(Context* c, uintptr_t _GPR2) {
	struct timeval* tv = (struct timeval*)_GPR2;
	if (tv == NULL) {
			c->GPRx = -1;
	} else {
		time_t usec = io_read(AM_TIMER_UPTIME).us;
		tv->tv_sec = usec / 1000000;
		tv->tv_usec = usec - tv->tv_sec * 1000000;
		c->GPRx = 0;
	}
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
#ifdef STRACE
	if (a[0] < 0 || a[0] > 19) {
		panic("Illegal Syscall ID = %d\n", a[0]);
	}
	printf("[STRACE]\tcalled %s, with arguments\t0x%x\t0x%x\t0x%x,returned 0x%x\n", SYS_STRING[a[0]], c->GPR2, c->GPR3, c->GPR4, c->GPRx);
#endif
  switch (a[0]) {
		case SYS_exit:sys_exit(c);break;
		case SYS_yield: sys_yield(c);break;
		case SYS_write: sys_write(c);break;
		case SYS_brk: sys_brk(c);break;
		case SYS_open: sys_open(c);break;
		case SYS_read: sys_read(c);break;
		case SYS_lseek: sys_lseek(c);break;
		case SYS_close: sys_close(c);break;
		case SYS_gettimeofday: {
								sys_gettimeofday(c, a[1]);
								break;
							   }
		case SYS_execve: c->GPRx = sys_execve((const char*)c->GPR2, (char** const )c->GPR3, (char** const)c->GPR4);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

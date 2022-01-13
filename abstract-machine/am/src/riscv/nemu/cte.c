#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>
#define USER 1
#define CONTEXT_SIZE  ((32 + 4) * XLEN)
#define XLEN  4
static Context* (*user_handler)(Event, Context*) = NULL;
void __am_get_cur_as(Context *c);
void __am_switch(Context *c);
Context* __am_irq_handle(Context *c) {
	__am_get_cur_as(c);
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
			case 0xb:{
							 if (c->GPR1 == -1) {
							 		ev.event = EVENT_YIELD;
									c->mepc += 4;
							 	} else {
							 		ev.event = EVENT_SYSCALL;
									c->mepc += 4;
							 		}
								break;
							 }	
			case 0x80000007: {
											 	ev.event = EVENT_IRQ_TIMER;
										//		c->mepc += 4;
												break;
											 }
      default: ev.event = EVENT_ERROR; break;
    }
	c->mstatus = ((c->mstatus & ~(1 << 3)) | (1 << 3));
    c = user_handler(ev, c);
    assert(c != NULL);
  }
	__am_switch(c);
  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
	int tmp = 0;
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));
	asm volatile("csrw mscratch, %0": : "r"(tmp));
  // register event handler
  user_handler = handler;

  return true;
}
#define a0 10
#define sp 2
Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
	Context* newContext = kstack.end - sizeof(Context);
	newContext->mepc = (uintptr_t)entry;
	newContext->mstatus = 0x1808;
	//support arguments passing.
	newContext->gpr[a0] = (uint32_t)arg;
	newContext->gpr[2] = (uint32_t)newContext;
	newContext->np = 0;
	int tmp = 0;
	asm volatile("csrw mscratch, %0": : "r"(tmp));
  return newContext;
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}

#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
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
							 }	break;
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}
#define a0 10
Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
	Context* newContext = (Context*) malloc(sizeof(Context));
	newContext->mepc = (uintptr_t)entry;
	newContext->mstatus = 0x1800;
	//support arguments passing.
	newContext->gpr[a0] = (uint32_t)arg;
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

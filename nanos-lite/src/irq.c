#include <common.h>
Context* schedule(Context *prev);
void do_syscall(Context*);
extern uint32_t _stack_top;
extern uint32_t _stack_pointer;
static Context* do_event(Event e, Context* c) {
  switch (e.event) {
		case EVENT_YIELD: 
				{
					Context* ret = schedule(c);
					assert(ret != NULL);
					return ret;
				break;
				}
		case EVENT_SYSCALL:{ 
						do_syscall(c); 
						break;
						  }
		case EVENT_IRQ_TIMER: {
														printf("TIMER_INTERRUPT SUCC!\n");
														Context* ret = schedule(c);
														assert(ret != NULL);
														return ret;
													}
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}

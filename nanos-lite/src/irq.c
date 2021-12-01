#include <common.h>
void do_syscall(Context*);
extern uint32_t _stack_top;
extern uint32_t _stack_pointer;
static Context* do_event(Event e, Context* c) {
		printf("stack_pointer = 0x%x\n", _stack_pointer);
		printf("0x%x\n", &_stack_top);
  switch (e.event) {
		case EVENT_YIELD: 
				{	printf("yield succuss!\n"); 
					c->mepc += 4;
					break;
				}
		case EVENT_SYSCALL:{ 
						do_syscall(c); 
						c->mepc += 4;
						break;
						  }
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}

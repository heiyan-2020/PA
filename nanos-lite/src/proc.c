#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%s' for the %dth time!", (char*)arg, j);
    j ++;
    yield();
  }
}

void context_kload(PCB* proc, void (*entry)(void *), void* arg) {
	Area stackArea = {
		.start = proc->stack, 
		.end = proc->stack + STACK_SIZE
	};
	proc->cp = kcontext(stackArea, entry, arg);	
}

void naive_uload(PCB*, const char*);
void init_proc() {
	context_kload(&pcb[0], hello_fun, "first!!!");
	context_kload(&pcb[1], hello_fun, "second!!!");
  switch_boot_pcb();

  Log("Initializing processes...");

  // load program here
	naive_uload(NULL, "/bin/dummy");
}

Context* schedule(Context *prev) {
	current->cp = prev;
	current = (current == &pcb[0] ? &pcb[0] : &pcb[1]);
	return current->cp;
}

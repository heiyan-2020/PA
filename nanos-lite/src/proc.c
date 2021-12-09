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
uintptr_t loader(PCB *pcb, const char *filename);

void context_uload(PCB* proc, const char* pathname, char* const argv[], char* const envp[]) {
	Area stackArea = {
		.start = proc->stack, 
		.end = proc->stack + STACK_SIZE
	};
	void* entry = (void*)loader(pcb, pathname);
	proc->cp = ucontext(NULL, stackArea, entry);	
	void* stack_space = new_page(8);
	proc->cp->GPRx = (uint32_t)stack_space;
	size_t argc = 0; 
	while (argv != NULL && argv[argc] != NULL) {argc++;}
	printf("argv[0] = %s\n", argv[0]);
	printf("argv[1] = %s\n", argv[1]);
	assert(argc == 1);
	*(int*)stack_space = argc;
	char** argv_start = (char**)stack_space + 1;
	char** argv_end = (char**)argv_start + argc;
	*(argv_end++) = NULL;

	size_t envc = 0;
	while (envp != NULL && envp[envc] != NULL) {envc++;}
	char** envp_start = argv_end;
	char** envp_end = envp_start + envc;
	*(envp_end++) = NULL;
	char* string_area = (char*) envp_end;
	for (int i = 0; i < argc; i++) {
		memcpy(string_area, argv[i], strlen(argv[i]) + 1);
		printf("%s\n", argv[i]);
		argv_start[i] = string_area;
		string_area += strlen(string_area) + 1;
	}
	int i = 0;
	while (envp != NULL && envp[i] != NULL){
		memcpy(string_area, envp[i], strlen(envp[i]) + 1);
		envp_start[i] = string_area;
		string_area += strlen(string_area) + 1;
		i++;
	}
	envp_start[i] = NULL;
}
void naive_uload(PCB*, const char*);
void init_proc() {
	context_kload(&pcb[0], hello_fun, "first!!!");
	char* const argv[] = {"/bin/nterm", NULL};
	context_uload(&pcb[1], "/bin/nterm", argv, NULL);
//	context_kload(&pcb[1], hello_fun, "second!!!");
  switch_boot_pcb();

  Log("Initializing processes...");

  // load program here
//	naive_uload(NULL, "/bin/nterm");
}

Context* schedule(Context *prev) {
	current->cp = prev;
	current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
	return current->cp;
}

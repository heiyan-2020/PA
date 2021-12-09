#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
	int argc = *((int*) args);
	args = (char**) args + 1;
	char** const argv = (char**) args;
	char** const envp = ((char**) args)	+ argc + 1;
  environ = envp;
  exit(main(argc, argv, envp));
  assert(0);
}

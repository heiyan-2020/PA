#include <common.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();
word_t expr(char *a, bool *b);
/*
 * this function is to test eval
 */

void test_eval() {
	FILE *file = fopen("../tools/gen-expr/input", "r");
	assert(file != NULL);

	char *line = NULL;
	size_t len = 0;
	ssize_t read = 0;

	while ((read = getline(&line, &len, file)) != -1) {
		char *result = strtok(line, " ");
		char *expression = result + strlen(result) + 1;
		uint32_t ures;
		sscanf(result, "%u", &ures);
		bool succ = true;
		uint32_t eval = expr(expression, &succ);
		assert(ures == eval);
	}
	fclose(file);
	printf("All test passed!\n");
}

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif

  /* Start engine. */
	test_eval();
  //engine_start();
  return is_exit_status_bad();
}

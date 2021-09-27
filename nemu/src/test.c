#include <stdio.h>
#include <assert.h>

int main() {
	FILE *fp = fopen("../tools/gen-expr/input", "r");
	assert(fp != NULL);
	fclose(fp);
	printf("open successfully!\n");
}

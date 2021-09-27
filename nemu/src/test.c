#include <stdio.h>
#include <assert.h>

int main() {
	FILE *fp = fopen("../a.c", "r");
	assert(fp != NULL);
	fclose(fp);
	printf("open successfully!\n");
}

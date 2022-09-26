#include <time.h>
#include <stdio.h>
#include <assert.h>

int main() {
	int count = 0;
	uint32_t preTime = 0;
	while (1) {
		uint32_t current = NDL_GetTicks();
		if (current - preTime >= 500) {
			printf("past 0.5 seconds\n");
			count ++;
		}
			if (count == 10) {
				break;
			}
			preTime = current;
		}
	}

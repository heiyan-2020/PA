#include <time.h>
#include <stdio.h>

int main() {
	int count = 0;
	struct timeval tv[1];
	time_t preTime = 0;
	while (1) {
		gettimeofday(tv, NULL);
		if (tv->tv_sec - preTime >= 0.5) {
			printf("past 0.5 seconds\n");
		}
			count++;
			if (count == 10) {
				break;
			}
			preTime = tv->tv_sec;
		}
	}

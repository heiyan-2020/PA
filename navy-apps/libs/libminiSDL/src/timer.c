#include <NDL.h>
#include <sdl-timer.h>
#include <stdio.h>

SDL_TimerID SDL_AddTimer(uint32_t interval, SDL_NewTimerCallback callback, void *param) {

printf("Not implemented SDL_AddTimer\n");
  return NULL;
}

int SDL_RemoveTimer(SDL_TimerID id) {

printf("Not implemented SDL_RemoveTimer\n");
  return 1;
}

uint32_t SDL_GetTicks() {
	return NDL_GetTicks();
}

void SDL_Delay(uint32_t ms) {
	uint32_t begin = NDL_GetTicks();
	uint32_t current = NDL_GetTicks();
	//ms = ms >= 10 ? ms : 10;
	while (current - begin < ms) {
		printf("begin = %d, current = %d\n", begin, current);
		current = NDL_GetTicks();
	}
}

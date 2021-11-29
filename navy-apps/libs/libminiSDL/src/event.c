#include <NDL.h>
#include <SDL.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
printf("Not implemented SDL_PushEvent\n");
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
			printf("Not implemented SDL_PollEvent\n");
  return 0;
}

static uint8_t find_keycode(char* buf) {
	//strong assume that SDL keycode is exactly same as AM keycode. So, if bug appears, Don't forget to check this.
	char trash1[16];
	char trash2[16];
	int keycode;
	sscanf(buf, "%s %s, keycode=%d\n", trash1, trash2, &keycode);
	printf("key code is %d\n", keycode);
	return (uint8_t) keycode;
}

int SDL_WaitEvent(SDL_Event *event) {
	if (event == NULL) {
		return 0;
	} else {
		//indefinitely wait for next event.
			char buf[64];
			while (1) {
				if (NDL_PollEvent(buf, sizeof(buf))) {
					SDL_KeyboardEvent	wrapEvent  = {
						.type = buf[1] == 'd' ? SDL_KEYDOWN : SDL_KEYUP,
						.keysym = {
														.sym = find_keycode(buf)
												 }
					};	
					printf("buf is %s, keycode is %d\n", buf, (int)wrapEvent.keysym.sym);
					event->type = wrapEvent.type;
					event->key = wrapEvent;
					return 0; 
			}
				printf("No event.\n");
		} 
	}
			return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
printf("Not implemented SDL_PeepEvents\n");
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
printf("Not implemented SDL_GetKeyState\n");
  return NULL;
}

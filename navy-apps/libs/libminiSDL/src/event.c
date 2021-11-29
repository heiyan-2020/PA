#include <NDL.h>
#include <SDL.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define keyname(k) #k,
static uint8_t find_keycode(char* buf);
static const char* keys[256] = {
	[SDLK_NONE] = "NONE",
	_KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
printf("Not implemented SDL_PushEvent\n");
  return 0;
}

int SDL_PollEvent(SDL_Event *event) {
	char buf[64];
	if (!NDL_PollEvent(buf, sizeof(buf))) {
		return 0;
	}	else {
		SDL_KeyboardEvent	wrapEvent  = {
						.type = buf[1] == 'd' ? SDL_KEYDOWN : SDL_KEYUP,
						.keysym = {
														.sym = find_keycode(buf)
												 }
					};	
					event->type = wrapEvent.type;
					event->key = wrapEvent;
					return 1; 
	}
}

static uint8_t find_keycode(char* buf) {
	//strong assume that SDL keycode is exactly same as AM keycode. So, if bug appears, Don't forget to check this.
//	char trash1[16];
//	char trash2[16];
//	int keycode;
//	sscanf(buf, "%s %s keycode=%d\n", trash1, trash2, &keycode);
	char trash[16];
	char _keyname[16];
	sscanf(buf, "%s %s\n", trash, _keyname);
	for (int i = 0; i < 256; i++) {
		if (strcmp(keys[i], _keyname) == 0) {
			return (uint8_t) i;
		}
	}	
	assert(0);
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
					event->type = wrapEvent.type;
					event->key = wrapEvent;
					return 0; 
			}
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

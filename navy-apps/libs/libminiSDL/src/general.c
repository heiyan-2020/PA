#include <NDL.h>

int SDL_Init(uint32_t flags) {
printf("Not implemented SDL_Init\n");
  return NDL_Init(flags);
}

void SDL_Quit() {
  NDL_Quit();
}

char *SDL_GetError() {
  return "Navy does not support SDL_GetError()";
}

int SDL_SetError(const char* fmt, ...) {

printf("Not implemented SDL_SetError\n");
  return -1;
}

int SDL_ShowCursor(int toggle) {

printf("Not implemented SDL_ShowCursor\n");
  return 0;
}

void SDL_WM_SetCaption(const char *title, const char *icon) {
				
printf("Not implemented SDL_WM_SetCaption\n");
}

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static struct timeval current[1];
static int canvas_x = 0, canvas_y = 0, canvas_w = 0, canvas_h = 0;
uint32_t NDL_GetTicks() {
  gettimeofday(current, NULL);
	return current->tv_sec * 1000;
}

int NDL_PollEvent(char *buf, int len) {
  int fd = open("/dev/events", 0, 0);
	return read(fd, buf, len);
}

void NDL_OpenCanvas(int *w, int *h) {
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  } else {
		int fd = open("/proc/dispinfo");
		char tmpBuf[64];
		int bytes = read(fd, (void*)tmpBuf, 64);
		sscanf(tmpBuf, "WIDTH : %d\nHEIGHT: %d\n", &screen_w, &screen_h);
		if (*w == 0 && *h == 0) {
			canvas_w = screen_w;
			canvas_h = screen_h;
			*w = screen_w;
			*h = screen_h;
		} else {
				canvas_w = *w;
				canvas_h = *h;	
				canvas_x = (screen_w - canvas_w) / 2;
				canvas_y = (screen_h - canvas_h) / 2;
		}
	}
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
		int real_x = canvas_x + x;
	 	int real_y = canvas_y + y;
		int fd = open("/dev/fb", 0, 0);
		lseek(fd, (real_y * screen_w + real_x) * 4, SEEK_SET);
		for (int i = 0; i < h; i++) {
			write(fd, pixels, w*4);
			pixels += w;
			real_y += 1;
			lseek(fd, (real_y * screen_w + real_x) * 4, SEEK_SET);
		}	
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
	int w = 0, h = 0;
	NDL_OpenCanvas(&w, &h);
  return 0;
}

void NDL_Quit() {
}

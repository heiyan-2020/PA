#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
	const char* charBuf = (const char*) buf;
	size_t count = 0;
	while (count < len) {
		putch(*charBuf++);
		count++;
	}
	return count;
}

size_t events_read(void *buf, size_t offset, size_t len) {
	char buffer[32];
	int bytesReadIn = 0;
  bool keydown = io_read(AM_INPUT_KEYBRD).keydown;
	int keycode = io_read(AM_INPUT_KEYBRD).keycode;
	if (keycode == AM_KEY_NONE) {
		return 0;
	}
	if (keydown) {
		bytesReadIn = sprintf(buffer, "kd %s\n", keyname[keycode], keycode);
	} else {
		bytesReadIn = sprintf(buffer, "ku %s\n", keyname[keycode], keycode);
	}
	if (bytesReadIn <= len) {
		memcpy(buf, buffer, bytesReadIn);
		return bytesReadIn;
	} else {
		memcpy(buf, buffer, len);
		return len;
	}
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
		int screen_w = io_read(AM_GPU_CONFIG).width;
		int screen_h = io_read(AM_GPU_CONFIG).height;
		char buffer[64];
		int bytesReadIn = sprintf(buffer, "WIDTH : %d\nHEIGHT: %d\n", screen_w, screen_h);
		if (bytesReadIn <= len) {
			memcpy(buf, buffer, bytesReadIn);
			return bytesReadIn;
		} else {
			memcpy(buf, buffer, len);
			return len;
		}
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
	int screen_w = io_read(AM_GPU_CONFIG).width;
	int current_h = (offset / 4) / screen_w;
	int current_w = (offset / 4) % (screen_w);
	printf("current_h = %d, current_w = %d\n", current_h, current_w);
	int remain = len - ((screen_w) - (current_w)) * 4;
	if (remain <  0) {
		io_write(AM_GPU_FBDRAW, current_w, current_h, (void*)buf, (len / 4), 1, true);
		return len;
	} else {
		io_write(AM_GPU_FBDRAW, current_w, current_h, (void*)buf, (screen_w - current_w), 1, true);
		return (screen_w - current_w) * 4;
	}
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}

#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
//	int i;
//	int w = (inl(VGACTL_ADDR) >> 16);  // TODO: get the correct width
//	int h = ((inl(VGACTL_ADDR) << 16) >> 16);  // TODO: get the correct height
//	uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
//	for (i = 0; i < w * h; i ++) fb[i] = i;
//	outl(SYNC_ADDR, 1);			
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = inl(VGACTL_ADDR) >> 16, .height = (inl(VGACTL_ADDR) << 16) >> 16,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
	uint32_t* pixel = (uint32_t*) ctl->pixels;
	int width = (inl(VGACTL_ADDR) >> 16); 
	int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
	for (int j = y; j < y + h; j++) {
		for (int i = x; i < x + w; i++) {
			outl(FB_ADDR + i + j * width, *pixel++);
		}
	}
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}

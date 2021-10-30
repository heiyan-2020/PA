#include <am.h>
#include <nemu.h>
#include <stdio.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
		uint32_t state = inl(KBD_ADDR);
		kbd->keydown = state >> 31;
		kbd->keycode = (int)((state << 1) >> 1);
		printf("keydown = %d, keycode = %d\n", kbd->keydown, kbd->keycode);
}

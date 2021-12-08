#include <am.h>
#include <nemu.h>

extern char _heap_start;
int main(const char *args);
#define HEAP_SIZE 0x4000000
Area heap = RANGE(&_heap_start, &_heap_start + HEAP_SIZE);
#ifndef MAINARGS
#define MAINARGS ""
#endif
static const char mainargs[] = MAINARGS;

void putch(char ch) {
  outb(SERIAL_PORT, ch);
}

void halt(int code) {
  nemu_trap(code);

  // should not reach here
  while (1);
}

void _trm_init() {
  int ret = main(mainargs);
  halt(ret);
}

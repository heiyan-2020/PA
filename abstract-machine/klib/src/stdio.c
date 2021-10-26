#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>
#include <stdint.h>
#include <klib-utils.h>
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	char buffer[256];
	print_template(buffer, fmt, ap);
	char *ptr = buffer;
	while (*ptr != '\0') {
		putch(*ptr++);
	}
	return strlen(buffer);
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
		va_list ap;
		va_start(ap, fmt);
		print_template(out, fmt, ap);
		int len = strlen(out);
		va_end(ap);
		return len;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif

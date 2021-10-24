#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>
char* convert(uint32_t num, int base);
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
		va_list ap;
		int d;
		char *s;
		char *ptr = out;
		va_start(ap, fmt);
		while (*fmt) {
			switch (*fmt++) {
				case '%': {
					switch (*fmt++) {
						case '%': *out++ = '%';break;
						case 'd': {
							d = va_arg(ap, int);
							char* tmp_str = convert(d, 10);
							memcpy(out, tmp_str, strlen(tmp_str));
							out += strlen(tmp_str);
							break;
						}
						case 's': {
							s = va_arg(ap, char*);
							memcpy(out, s, strlen(s));
							out += strlen(s);
							break;					
						}
						default: return -1;
					}	
				}
				default : *out++ = *(fmt - 1);break;
			}
		}
		va_end(ap);
		int num = out - ptr - 1;
		*out = '\0';
		out = ptr;
		return num;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif

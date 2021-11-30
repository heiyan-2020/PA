#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
		if (s == NULL) {
			return 0;
		}
		const char *itr = s;
		while (*itr++ != '\0');
		return itr - s - 1;
}

char *strcpy(char *dst, const char *src) {
		memcpy(dst, src, strlen(src));
		return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
		memcpy(dst + strlen(dst), src, strlen(src));
		return dst;
}

int strcmp(const char *s1, const char *s2) {
		const unsigned char *t1 =(const unsigned char*) s1;
		const unsigned char *t2 =(const unsigned char*) s2;
		unsigned char c1, c2;

		do {
			c1 = (unsigned char) *t1++;
			c2 = (unsigned char) *t2++;
		if (c1 == '\0') {
			return c1 - c2;
			} 
		} while (c1 == c2);

	return c1 - c2;	
}

int strncmp(const char *s1, const char *s2, size_t n) {
  panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
	unsigned char* ptr =(unsigned char*) s;
	while (n-- > 0) {
		*ptr++ = c;
	}
	return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
		char *d =(char *) out;
		const char *s =(char *) in;
		while (n-- > 0) {
			*d++ = *s++;
		}
		return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
		const unsigned char* p1 = s1;
		const unsigned char* p2 = s2;

		while (n-- > 0) {
			printf("%d\n", n);
			if (*p1++ != *p2++) {
				return *(p1 - 1) - *(p2 - 1);
			}
		}
		return 0;
}

#endif

#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
char * convert(uint32_t num, int base) {
	static char Table[] = "0123456789ABCDEF";
	static char buffer[50];
	char* ptr;

	ptr = &buffer[49];
	*ptr = '\0';

	do {
		*--ptr = Table[num % base];
		num /= base;
	}while (num != 0);

	return ptr;
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
							printf("%s\n%s\n%s\n", ptr,fmt, tmp_str);
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
					break;
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
int main() {
	char str[] = "hello, world";
	char result[50];
	sprintf(result, "%s is my %d string\n", str, 23);
	printf("%s", result);
}

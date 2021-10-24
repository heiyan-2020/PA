#include <stdint.h>

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

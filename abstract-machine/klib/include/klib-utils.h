#include <stdint.h>
#include <stdarg.h>
char* convert(uint32_t num, int base);

char* print_template(char* out, const char* fmt, va_list ap);

#define STRACE "y"
#include <stdarg.h>

#define log_write(...) \
				printf(__VA_ARGS__);

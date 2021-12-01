//convert int/uint number to string accroding to base.
#include <stdarg.h>
#include <klib.h>
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
char * convert_64(uint64_t num, int base) {
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

//abstract the repetitive code of print family func.
char* print_template(char* out, const char* fmt, va_list ap) {
		int d;
		char *s;
		char *ptr = out;
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
						case 'x': {
							d = va_arg(ap, int);
							char* tmp_str = convert(d, 16);
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
						case 'l': {
								  switch(*fmt++) {
								  	case 'd': {
							d = va_arg(ap, long);
							char* tmp_str = convert(d, 10);
							memcpy(out, tmp_str, strlen(tmp_str));
							out += strlen(tmp_str);
							break;
											  }
									default : {
											  return NULL;
											  }
								  }
								  break;
								  }
						default: return NULL;
					}	
				}
									break;
				default : *out++ = *(fmt - 1);break;
			}
		}
		*out = '\0';
		return ptr;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
FILE *log_fp = NULL;

void init_log(const char *log_file) {
  log_fp = stdout;
  if (log_file != NULL) {
    FILE *fp = fopen(log_file, "w");
    log_fp = fp;
  }
	printf("nanos log is written to %s", log_file ? log_file : "stdout");
}


#define log_write(...) \
				fprintf(log_fp, __VA_ARGS__);\
				fflush(log_fp);

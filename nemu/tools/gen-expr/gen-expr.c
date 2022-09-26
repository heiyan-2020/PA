#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static uint32_t choose (uint32_t n) {
		uint32_t rtn = rand() % n;
		return rtn;
}	

static int pos;

static void gen(char ch) {
	buf[pos++] = ch;
}

static void gen_str(char *chs) {
	for(int i = 0; i < strlen(chs); i++) {
		gen(chs[i]);
	}
}

static void gen_num() {
	int num_len = choose (4) + 1;
	for (int i = 0; i < num_len; i ++) {
			if (i == 0) {
				gen ('1' + choose(9));
			} else {
			gen('0' + choose(10));
			}
	}
}

static void gen_rand_op() {
	switch (choose(8)) {
		case 0: gen('+');break;
		case 1: gen('-');break;
		case 2: gen('*');break;
		case 3: gen_str("==");break;
		case 4: gen_str("!=");break;
		case 5: gen_str("&&");break;
		default: gen('/');break;
	}
}

static void gen_rand_expr() {
	switch (choose(3)) {
		case 0: gen_num();break;
		case 1: gen('(');gen_rand_expr();gen(')');break;
		default: gen_rand_expr();gen_rand_op();gen_rand_expr();break;
	}
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
	if (argc > 1) {
		sscanf(argv[1], "%d", &loop);
	}
	int i;
	for (i = 0; i < loop; i ++) {
		pos = 0;
		gen_rand_expr();
		buf[pos] = '\0';
		sprintf(code_buf, code_format, buf);

		FILE* fp = fopen("/tmp/.code.c", "w");
		assert(fp != NULL);
		fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc -Werror  /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);
	
    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
			}
			return 0;
}

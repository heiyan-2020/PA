#include <stdio.h>
#include <fixedptc.h>
#include <assert.h>
int main() {
	fixedpt a = fixedpt_rconst(1.2);
	fixedpt b = fixedpt_fromint(10);
	int c = 0;
	if (b > fixedpt_rconst(7.9)) {
  	c = fixedpt_toint(fixedpt_div(fixedpt_mul(a + FIXEDPT_ONE, b), fixedpt_rconst(2.3)));
	}
	printf("c = %d\n", c);
	assert(c == 9);
	fixedpt d = fixedpt_rconst(-1.7);
	c = fixedpt_floor(d) + fixedpt_ceil(a);
	assert(fixedpt_rconst(c) == 0);
}

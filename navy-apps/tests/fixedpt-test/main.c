#include <stdio.h>
#include <fixedptc.h>
#include <assert.h>
int volatile main() {
	volatile	fixedpt a = fixedpt_rconst(1.2);
}

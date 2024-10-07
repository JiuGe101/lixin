#include "my_code.h"
#include <stdint.h>


void A()
{
	uint32_t a = 1;
	uint32_t b = 2;
	uint32_t c = 3;
	B();
}

void B()
{
	uint32_t *p = (uint32_t *)0xFFFFFFFF;
	*p = 4;
	uint32_t d = 5;
}

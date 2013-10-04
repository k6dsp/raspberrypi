#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

extern "C" void _init() {};
#if 0
void swap_s(char * x)
{
	if (x)
	{
		char * end = x;
		while(*end)
		{
			++end;
		}
		--end;
		while(x < end)
		{
			*x = *x ^ *end;
			*end = *x ^ *end;
			*x = *x ^ *end;
			x++;
			end--;
		}
	}
}
#else
void swap_s(char * x)
{
	if (x)
	{
		char * end = x;
		while(*end)
		{
			++end;
		}
		--end;
		while(x < end)
		{
			char tmp = *x;
			*x = *end;
			*end = tmp;
			x++;
			end--;
		}
	}
}
#endif

int main()
{
	char st[] = "this is a test";
	printf("%s\n\r", st);
	swap_s(st);
	printf("%s\n\r", st);
	return 0;
}

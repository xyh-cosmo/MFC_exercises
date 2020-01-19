#include <stdio.h>
#include <stdlib.h>

int main()
{

	int a=0x1234;
	char b=*(char*)&a;
	if(b==0x12)
		printf("big end\n");
	else
		printf("little end\n");

	return 0;
}

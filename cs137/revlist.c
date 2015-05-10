#include <stdio.h>

int main(void) {
	int input;
	scanf("%d", &input);

	if(input != 0) {
		main();
	}
		
	printf("%d\n", input);
	return 0;
}

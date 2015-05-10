#include <stdio.h>

int isTwin(int number);

void testTwin (int number) {
	if (isTwin(number))
		printf("%d is a twin prime\n", number);
	else
		printf("%d is a not a twin prime\n", number);
}

int main (void) {
	testTwin (18409199);
	testTwin (18408989);
	testTwin (3571);
	testTwin (3559);
	testTwin (3547);
	testTwin (3533);
	testTwin (3541);
	return 0;
}

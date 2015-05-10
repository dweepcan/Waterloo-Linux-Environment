#include <stdio.h>

int dayOfYear(int day, int month, int year);

void testDayOfYear(int day, int month, int year) {
	printf("%d/%d/%d => %d\n", day, month, year, dayOfYear(day, month, year));
}

int main(void) {
	testDayOfYear (-1, 1, 1583);
	testDayOfYear (29, -5, 1582);
	testDayOfYear (31, 5, 100);
	testDayOfYear (31, 5, 2009);
	testDayOfYear (31, 5, 2008);
	testDayOfYear (31, 5, 2100);
	testDayOfYear (31, 12, 2400);
	testDayOfYear (0, 0, 0);
	testDayOfYear (0, -1, -100);
	return 0;
}

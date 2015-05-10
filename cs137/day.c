#include <stdio.h>

int isLeap(int year) {
	if(year%4 == 0 && (year%100 != 0 || year%400 == 0))
		return 1;
	else 
		return 0;
}

int dayOfYear(int day, int month, int year) {
	int dayOfYear = 0;

	if(year < 1583 || month < 0 || month > 12 || day <= 0) 
		return -1;
	else if((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day > 31) 
		return -1;
	else if((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
		return -1;
	else if(month == 2 && !isLeap(year) && day > 28)
		return -1;
	else if(month == 2 && isLeap(year) && day > 29)
		return -1;
	
	int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	if (isLeap(year))
		daysInMonth[1] = 29;
	
	int i;	
	for (i = 0; i <= month-2; i++) {
		dayOfYear += daysInMonth[i];
	}

	dayOfYear += day;
	
	return dayOfYear;	
}

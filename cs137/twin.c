#include <stdio.h>

int isPrime(int number){
	//Variable to store the divisor
	int divisor = 2;

	// If number is less than 1 then it is not prime
	if(number <= 1)
		return 0;
	
	// Check if number is divisible by numbers between 2 to sqrt(number)
	while(number/divisor >= divisor) {
		if(number % divisor == 0)
			return 0;
		divisor++;
	}
	return 1;
}

int isTwin(int number) {
	// Check if number is a twin prime
	if(isPrime(number) && (isPrime(number+2) || isPrime(number-2)))
		return 1;	
	else
		return 0;
}


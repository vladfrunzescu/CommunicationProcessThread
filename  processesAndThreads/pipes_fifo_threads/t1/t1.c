#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;
int sum = 0;

int isPrime(int number){
	if(number < 2){
		return 0;
	}

	for(int i = 2; i * i <= number; i++){
		if(number % i == 0){
			return 0;
		}
	}

	return 1;
}

int factorial(int number){
	int result = 1;

	for(int i = 2; i <= number; i++){
		result *= i;
	}

	return result;
}

void* addPrimes(void *a){
	int number = atoi((char*)a);
	if(isPrime(number) == 1){
		pthread_mutex_lock(&mutex);
		char *delimiter = " ";
		//sum += strtod((char*)a, &delimiter);
		sum += number;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void* calculateFactorial(void *a){
	char *delimiter = " ";
	int number = strtod((char*)a, &delimiter);
	number = atoi((char*)a);
	printf("%d! = %d\n", number, factorial(number));

	return NULL;
}

int main(int argc, char **argv){
	pthread_mutex_init(&mutex, NULL);
	pthread_t *threads;
	int numbers = argc - 1;

	printf("Arguments: %d\n", numbers);
	threads = (pthread_t*) malloc(sizeof(pthread_t) * numbers * 2);

	for(int i = 0; i < numbers * 2; i += 2){
		pthread_create(&threads[i], NULL, calculateFactorial,
			argv[i/2 + 1]);
		pthread_create(&threads[i + 1], NULL, addPrimes,
			argv[i/2 + 1]);
	}

	for(int i = 0; i < numbers * 2; i++){
		pthread_join(threads[i], NULL);
	}

	printf("Sum of primes: %d\n", sum);

	pthread_mutex_destroy(&mutex);

	return 0;
}

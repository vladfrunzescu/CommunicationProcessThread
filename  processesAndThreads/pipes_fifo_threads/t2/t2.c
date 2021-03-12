#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t sum_mutex, palindrome_mutex;

int sum = 0, palindrome_sum = 0;

int isPalindrome(char *string){
	int length = strlen(string);

	for(int i = 0; i < length / 2; i++){
		if(string[i] != string[length - i - 1]){
			return 0;
		}
	}

	return 1;
}

void* addRangeSum(void *a){
	int number = atoi((char*)a),
		range_sum = 0;

	for(int i = 1; i <= number; i++){
		range_sum += i;
	}

	printf("sum 1->%d = %d\n", number, range_sum);

	pthread_mutex_lock(&sum_mutex);
	sum += range_sum;
	pthread_mutex_unlock(&sum_mutex);

	return NULL;
}

void* addPalindromes(void *a){
	if(isPalindrome((char*)a) == 1){
		pthread_mutex_lock(&palindrome_mutex);
		palindrome_sum += atoi((char*)a);
		pthread_mutex_unlock(&palindrome_mutex);
	}

	return NULL;
}

void* addSmallNumbers(void *a){
	int number = atoi((char*)a);
	
	if(number < 10){
		pthread_mutex_lock(&sum_mutex);
		sum += number;
		pthread_mutex_unlock(&sum_mutex);
	}

	return NULL;
}

int main(int argc, char **argv){
	int numbers = argc - 1;
	pthread_t *threads;

	threads = (pthread_t*) malloc(sizeof(pthread_t) * numbers * 3);

	for(int i = 0; i < numbers * 3; i += 3){
		pthread_create(&threads[i], NULL, addRangeSum,
			argv[i / 3 + 1]);
		pthread_create(&threads[i + 1], NULL, addPalindromes,
			argv[i / 3 + 1]);
		pthread_create(&threads[i + 2], NULL, addSmallNumbers,
			argv[i / 3 + 1]);
	}

	for(int i = 0; i < numbers * 3; i++){
		pthread_join(threads[i], NULL);
	}

	printf("Sum of palindromes: %d\n", palindrome_sum);
	printf("Total sum: %d\n", sum);

	pthread_mutex_destroy(&sum_mutex);
	pthread_mutex_destroy(&palindrome_mutex);

	return 0;
}

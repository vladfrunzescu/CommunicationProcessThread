#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>

#define STRING_LENGTH 16

int main(int argc, char **argv){
	char string[STRING_LENGTH];
	int a2c, c2b, b2a;
	int current_string_length;

	unlink("a2c");
	unlink("c2b");
	unlink("b2a");

	mkfifo("a2c", 0600);
	mkfifo("c2b", 0600);
	mkfifo("b2a", 0600);

	if(fork() == 0){
		// B

		c2b = open("c2b", O_RDONLY);
		b2a = open("b2a", O_WRONLY);

		if(read(c2b, string, sizeof(char) * STRING_LENGTH)
			< sizeof(char) * STRING_LENGTH){

			current_string_length = STRING_LENGTH;
		} else{
			current_string_length = strlen(string);
		}
		while(current_string_length < STRING_LENGTH - 1){
			printf("B: %s -> ", string);
			strcat(string, "b");
			printf("%s\n", string);
			write(b2a, string, sizeof(char) * STRING_LENGTH);

			if(read(c2b, string, sizeof(char) * STRING_LENGTH)
				< sizeof(char) * STRING_LENGTH){

				current_string_length = STRING_LENGTH;
			} else{
				current_string_length = strlen(string);
			}
		}

		close(c2b);
		close(b2a);

		exit(EXIT_SUCCESS);
	}
	if(fork() == 0){
		// C

		a2c = open("a2c", O_RDONLY);
		c2b = open("c2b", O_WRONLY);

		if(read(a2c, string, sizeof(char) * STRING_LENGTH)
			< sizeof(char) * STRING_LENGTH){

			current_string_length = STRING_LENGTH;
		} else{
			current_string_length = strlen(string);
		}
		while(current_string_length < STRING_LENGTH - 1){
			printf("C: %s -> ", string);
			strcat(string, "c");
			printf("%s\n", string);
			write(c2b, string, sizeof(char) * STRING_LENGTH);

			if(read(a2c, string, sizeof(char) * STRING_LENGTH)
				< sizeof(char) * STRING_LENGTH){

				current_string_length = STRING_LENGTH;
			} else{
				current_string_length = strlen(string);
			}
		}

		close(a2c);
		close(c2b);

		exit(EXIT_SUCCESS);
	}

	// A
	
	a2c = open("a2c", O_WRONLY);
	b2a = open("b2a", O_RDONLY);

	string[0] = '\0';

	printf("A: %s\n", string);
	write(a2c, string, sizeof(char) * STRING_LENGTH);

	if(read(b2a, string, sizeof(char) * STRING_LENGTH)
		< sizeof(char) * STRING_LENGTH){

		current_string_length = STRING_LENGTH;
	} else{
		current_string_length = strlen(string);
	}
	while(current_string_length < STRING_LENGTH - 1){
		printf("A: %s -> ", string);
		if(current_string_length % 2 == 0){
			strcat(string, "A");
		} else{
			strcat(string, "a");
		}
		printf("%s\n", string);
		write(a2c, string, sizeof(char) * STRING_LENGTH);

		if(read(b2a, string, sizeof(char) * STRING_LENGTH)
			< sizeof(char) * STRING_LENGTH){

			current_string_length = STRING_LENGTH;
		} else{
			current_string_length = strlen(string);
		}
	}

	close(a2c);
	close(b2a);

	unlink("a2c");
	unlink("c2b");
	unlink("b2a");
	return 0;
}

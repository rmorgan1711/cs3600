#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]){
	
	if(argc != 2){
		fprintf(stderr, "More than one argument passed in.\n");
		exit(EXIT_FAILURE); // not checking fprintf for error since exit anyway
	}

	// getpid() and getppid() are always successful
	if( printf("Child PID: %d\n", getpid()) <= 0 ){
		perror("printf error");
		exit(EXIT_FAILURE);
	}

	if( printf("Parend PID: %d\n", getppid()) <= 0){
		perror("printf error");
		exit(EXIT_FAILURE);
	}

	long count = strtol(argv[1], NULL, 10);	
	if(count == EINVAL || count == ERANGE || errno == EINVAL || errno == ERANGE){
		fprintf(stderr, "Error converting string to long value.\n");
		exit(EXIT_FAILURE);
	}

	for(int i=0; i<count; i++){
		if( printf("Process: %d %d\n", getpid(), (i+1)) <= 0 ){
			perror("printf error");
			exit(EXIT_FAILURE);
		}
	}

	return count;
}

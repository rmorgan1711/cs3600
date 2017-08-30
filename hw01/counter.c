#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	
	if(argc != 2){
		fprintf(stderr, "More than one argument passed in.\n");
		exit(EXIT_FAILURE);
	}

	printf("Child PID: %d\n", getpid());
	printf("Parend PID: %d\n", getppid());

	long count = strtol(argv[1], NULL, 10);	
	for(int i=0; i<count; i++){
		printf("Process: %d %d\n", getpid(), (i+1));
	}

	return count;
}


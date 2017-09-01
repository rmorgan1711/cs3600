#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	int pid = fork();
	if(pid < 0){
		perror("fork error");
		exit(EXIT_FAILURE);
	}

	if(pid > 0){ // in parent; child pid returned;
		int childState = 0;
		wait(&childState);
		if(WIFEXITED(childState)){
			printf("Process %d exited with status %d\n", pid, WEXITSTATUS(childState));
		}
	}
	else if(pid == 0){ // in child 
		execl("counter", "./counter", "5", (char *)NULL);
		perror("execl error");
	}

	exit(EXIT_SUCCESS);	

}

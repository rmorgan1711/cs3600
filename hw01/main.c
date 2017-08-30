#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[]){
	int pid = fork();

	if(pid != 0 && pid > 0){
		int childState = 0;
		wait(&childState);
		if(WIFEXITED(childState)){
			printf("Process %d exited with status %d\n", pid, WEXITSTATUS(childState));
		}
	}
	else if(pid == 0){
		int temp = execl("counter", "./counter", "5", (char *)NULL);
	}

	return 0;

}

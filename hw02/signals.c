#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

void handler(int sigNum){
	const char * const sigName = sys_signame[sigNum];
	write(STDOUT_FILENO, sys_signame[sigNum], strlen(sigName));
	write(STDOUT_FILENO, "\n", 1);
}


extern const char * const sys_signame[];

int main(int argc, char *argv[]){

	struct sigaction sigAct;
	sigAct.sa_handler = handler;
	sigemptyset(&sigAct.sa_mask);

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

	handler(4);
	exit(EXIT_SUCCESS);
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <assert.h>
#include <string.h>

#define WRITES(a) { const char *s = a;  assert (write (STDOUT_FILENO, s, strlen (s)) >= 0); }

#define MAX_SIG_NUM 100

char names[MAX_SIG_NUM][100]; // global array of signal number descriptions

void printInterupt(int sigNum)
{
	WRITES("--- Signal received: ");
	//WRITES(sys_signame[sigNum]);
	//WRITES(strsignal(sigNum));
	
	if(sigNum < MAX_SIG_NUM){
		WRITES(names[sigNum]);
	}else{
		WRITES("Unsupported signal received");			
	}
	WRITES(" ---\n");
}

void beepOnInterrupt(int sigNum){
	WRITES("\a");		
}



int main(int argc, char *argv[])
{
	// fill global array of sigNum descriptions
	int i;
	for(i=0; i< 100; i++)
		strcpy(names[i], strsignal(i));		

	// create sigactions
	struct sigaction printSig;
	struct sigaction beepSig;

	printSig.sa_handler = printInterupt;
	beepSig.sa_handler = beepOnInterrupt;

	sigset_t signalSet[2];
	signalSet[0] = printSig.sa_mask;
	signalSet[1] = beepSig.sa_mask;
	assert(sigemptyset(signalSet) > -1);

	assert(sigaction (SIGALRM, &beepSig, NULL) == 0);
	assert(sigaction (SIGILL, &printSig, NULL) == 0);
	assert(sigaction (SIGUSR1, &printSig, NULL) == 0);
	assert(sigaction (SIGUSR2, &printSig, NULL) == 0);
	assert(sigaction (SIGFPE, &printSig, NULL) == 0);

	int	pid; 
	assert( (pid = fork()) >= 0 );
	if (pid > 0) {
		//in parent; child pid returned
		int	childState = 0;
		wait(&childState);
		if (WIFEXITED(childState)) {
			printf("Process %d exited with status %d\n", pid, WEXITSTATUS(childState));
		}
	} else if (pid == 0) {
		//in child
		int ppid = getppid(); 
		assert(kill(ppid, SIGALRM) > -1);
		assert(kill(ppid, SIGILL) > -1);
		assert(kill(ppid, SIGUSR1) > -1);
		assert(kill(ppid, SIGUSR2) > -1);
		

		assert(kill(ppid, SIGFPE) > -1);
		assert(kill(ppid, SIGFPE) > -1);
		assert(kill(ppid, SIGFPE) > -1);
	}

	exit(EXIT_SUCCESS);
}

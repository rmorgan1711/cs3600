#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <assert.h>
#include <string.h>

using namespace std;

/* Write to a file descriptor */
#define WRITEFD(fd, a) {    const char *b = a; \
                            assert(write(fd, b, strlen(b)) != -1); }
/* Write a newline */
#define WRITENL { assert (write (STDOUT_FILENO, "\n", 1) == 1); }

#define READ 0
#define WRITE 1

int main (int argc, char **argv)
{
	int pipeFDs[2];

	cout << "Argument list: " << endl;
    for (int i=1; i<argc; i++){
        int fd = strtol(argv[i], NULL, 10);	
        cout << "Arg " << i << ": " << fd << endl;

        if (i == 1)
        	pipeFDs[READ] = fd;
        else if (i == 2)
        	pipeFDs[WRITE] = fd;
    }

    char buf[1];
    buf[0] = (char)0x1;
    WRITEFD(pipeFDs[WRITE], buf);
    assert( kill(getppid(), SIGTRAP) != -1 );

    char buffer[1024];
    int len = read(pipeFDs[READ], buffer, sizeof(buffer));
    assert( len != -1 );
    buffer[len] = '\0';
    cout << "******read " << len << " bytes." << endl;

    WRITEFD(STDOUT_FILENO, buffer);
    WRITEFD(STDOUT_FILENO, "\n");

    
}
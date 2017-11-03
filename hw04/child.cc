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

void writeAndSignal(int fileDesc, char callNum){
	char buf[1];
    buf[0] = callNum;
    WRITEFD(fileDesc, buf);
    assert( kill(getppid(), SIGTRAP) != -1 );
}

int main (int argc, char **argv)
{
	int pipeFDs[2];
    for (int i=1; i<argc; i++){
        int fd = strtol(argv[i], NULL, 10);	

        if (i == 1)
        	pipeFDs[READ] = fd;
        else if (i == 2)
        	pipeFDs[WRITE] = fd;
    }

    int len;

    writeAndSignal(pipeFDs[WRITE], 0x1);
    char buff1[1024];
    len = read(pipeFDs[READ], buff1, sizeof(buff1));
    assert( len != -1 );
    buff1[len] = '\0';
    WRITEFD(STDOUT_FILENO, "System time is: ");
    WRITEFD(STDOUT_FILENO, buff1);
    WRITEFD(STDOUT_FILENO, "\n");

    writeAndSignal(pipeFDs[WRITE], 0x2);
    char buff2[2048];
    len = read(pipeFDs[READ], buff2, sizeof(buff2));
    assert( len != -1 );
    buff2[len] = '\0';
    WRITEFD(STDOUT_FILENO, buff2);
    WRITEFD(STDOUT_FILENO, "\n");


    writeAndSignal(pipeFDs[WRITE], 0x3);
    char buff3[2048];
    len = read(pipeFDs[READ], buff3, sizeof(buff3));
    assert( len != -1 );
    buff3[len] = '\0';
    WRITEFD(STDOUT_FILENO, "System time is: ");
    WRITEFD(STDOUT_FILENO, buff3);
    WRITEFD(STDOUT_FILENO, "\n");

    char buff4[1024];
    buff4[0] = (char)0x4;
    buff4[1] = '\0';
    strcat(buff4, "Parent, please print this to STDOUT for me!\n");
    WRITEFD(pipeFDs[WRITE], buff4);
    assert( kill(getppid(), SIGTRAP) != -1 );



    
}










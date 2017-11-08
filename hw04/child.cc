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

#define READ 0
#define WRITE 1

void writeAndSignal(int fileDesc, char callNum, const char *message = NULL){
	// byte for signal num + bye for '\0' + message length
	char buf[ 1 + 1 + (message == NULL ? 0 : strlen(message)) ];
    buf[0] = callNum;
    buf[1] = '\0';
    if (message != NULL)
    	strcat(buf, message);

    WRITEFD(fileDesc, buf);
    assert( kill(getppid(), SIGTRAP) != -1 );
}

int writeSignalAndReadResponse(int fileDesc[2], char callNum, char *respBuffer, size_t respSize, const char *message = NULL){
	writeAndSignal(fileDesc[WRITE], callNum, message);
	int len = read(fileDesc[READ], respBuffer, respSize);
    assert( len != -1 );

    return len;
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
    char returnBuf[4096];
    char messageBuf[4096];

    len = writeSignalAndReadResponse(pipeFDs, 0x1, returnBuf, sizeof(returnBuf), NULL);
    returnBuf[len] = '\n';
    returnBuf[len+1] = '\0';
    strcpy(messageBuf, "kernel responded system time request with....\n");
    strcat(messageBuf, returnBuf);
    strcat(messageBuf, "\n");
    len = writeSignalAndReadResponse(pipeFDs, 0x4, returnBuf, sizeof(returnBuf), messageBuf);


    len = writeSignalAndReadResponse(pipeFDs, 0x2, returnBuf, sizeof(returnBuf), NULL);
    returnBuf[len] = '\n';
    returnBuf[len+1] = '\0';
    strcpy(messageBuf, "kernel responded this process info request with....\n");
    strcat(messageBuf, returnBuf);
    strcat(messageBuf, "\n");
    len = writeSignalAndReadResponse(pipeFDs, 0x4, returnBuf, sizeof(returnBuf), messageBuf);


    len = writeSignalAndReadResponse(pipeFDs, 0x3, returnBuf, sizeof(returnBuf), NULL);
    returnBuf[len] = '\n';
    returnBuf[len+1] = '\0';
    strcpy(messageBuf, "kernel responded process list request with....\n");
    strcat(messageBuf, returnBuf);
    strcat(messageBuf, "\n");
    len = writeSignalAndReadResponse(pipeFDs, 0x4, returnBuf, sizeof(returnBuf), messageBuf);


    len = writeSignalAndReadResponse(pipeFDs, 0x4, returnBuf, sizeof(returnBuf), "Write me to std out please!");
    returnBuf[len] = '\n';
    returnBuf[len+1] = '\0';
    strcpy(messageBuf, "kernel responded process write to STD OUT request with....\n");
    strcat(messageBuf, returnBuf);
    strcat(messageBuf, "\n");
    len = writeSignalAndReadResponse(pipeFDs, 0x4, returnBuf, sizeof(returnBuf), messageBuf);

    // writeAndSignal(pipeFDs[WRITE], 0x1, NULL);
    // len = read(pipeFDs[READ], returnBuf, sizeof(returnBuf));
    // cout << "len = " << len << endl;
    // assert( len != -1 );
    // returnBuf[len] = '\n';
    // returnBuf[len+1] = '\0';
    // strcpy(messageBuf, "kernel responded system time request with....\n");
    // strcat(messageBuf, returnBuf);
    // strcat(messageBuf, "\n");
    // writeAndSignal(pipeFDs[WRITE], (char)0x4, messageBuf);
    // len = read(pipeFDs[READ], returnBuf, sizeof(returnBuf));
    // assert( len != -1 );

    // writeAndSignal(pipeFDs[WRITE], 0x2, NULL);
    // len = read(pipeFDs[READ], returnBuf, sizeof(returnBuf));
    // assert( len != -1 );
    // returnBuf[len] = '\n';
    // returnBuf[len+1] = '\0';
    // strcpy(messageBuf, "kernel responded this process info request with....\n");
    // strcat(messageBuf, returnBuf);
    // strcat(messageBuf, "\n");
    // writeAndSignal(pipeFDs[WRITE], (char)0x4, messageBuf);
    // len = read(pipeFDs[READ], returnBuf, sizeof(returnBuf));
    // assert( len != -1 );

    // writeAndSignal(pipeFDs[WRITE], 0x3, NULL);
    // char buff3[2048];
    // len = read(pipeFDs[READ], buff3, sizeof(buff3));
    // assert( len != -1 );
    // buff3[len] = '\n';
    // buff3[len+1] = '\0';
    // strcpy(messageBuf, "kernel responded process list request with....\n");
    // strcat(messageBuf, buff3);
    // strcat(messageBuf, "\n");
    // writeAndSignal(pipeFDs[WRITE], (char)0x4, messageBuf);
    // len = read(pipeFDs[READ], returnBuf, sizeof(returnBuf));
    // assert( len != -1 );

    // writeAndSignal(pipeFDs[WRITE], 0x1, NULL);
    // len = read(pipeFDs[READ], returnBuf, sizeof(returnBuf));
    // assert( len != -1 );
    // returnBuf[len] = '\n';
    // returnBuf[len+1] = '\0';
    // strcpy(messageBuf, "kernel responded system time request with....\n");
    // strcat(messageBuf, returnBuf);
    // strcat(messageBuf, "\n");
    // writeAndSignal(pipeFDs[WRITE], (char)0x4, messageBuf);
    // len = read(pipeFDs[READ], returnBuf, sizeof(returnBuf));
    // assert( len != -1 );


    
}










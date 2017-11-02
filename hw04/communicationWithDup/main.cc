#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

// the pipes
struct {
    int main2child[2];
    int child2main[2];
} pipes;

#define READ 0
#define WRITE 1

#define SYSASSERT(a) if (!(a)) { \
    fprintf(stderr, "assertion %s failed: file %s, line %d, error %s\n", \
        #a, __FILE__, __LINE__, sys_errlist[errno]); exit(1); }

int main (int argc, char** argv)
{
    SYSASSERT(pipe(pipes.main2child) >= 0);
    SYSASSERT(pipe(pipes.child2main) >= 0);

    if (fork() == 0)
    {
        // close these for safety.
        SYSASSERT(close (pipes.child2main[READ]) >= 0);
        SYSASSERT(close (pipes.main2child[WRITE]) >= 0);

        // assign fildes 3 and 4 to the pipe ends in the child so that we
        // always know what they will be.
        SYSASSERT(dup2 (pipes.main2child[READ], 3) >= 0);
        SYSASSERT(dup2 (pipes.child2main[WRITE], 4) >= 0);

        execl ("./child", "./child", NULL);
        SYSASSERT(false);
    }

    char buf[1024];
    int num_read = read (pipes.child2main[READ], buf, 1023);
    SYSASSERT(num_read >= 0);
    buf[num_read] = '\0';
    SYSASSERT(printf("main read: %s\n", buf) >= 0);

    const char *message = "from main to child";
    SYSASSERT(write (pipes.main2child[WRITE], message, strlen (message)) >= 0);

    exit (0);
}

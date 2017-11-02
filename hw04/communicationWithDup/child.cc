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
#include <time.h>

#define SYSASSERT(a) if (!(a)) { \
    fprintf(stderr, "assertion %s failed: file %s, line %d, error %s\n", \
        #a, __FILE__, __LINE__, sys_errlist[errno]); exit(1); }
/*
#define SYSASSERT(a) if (!(a)) { fprintf(stderr, "shit"); exit(1); }
*/

int main (int argc, char** argv)
{
    const char *message = "from child to main";
    SYSASSERT(write (4, message, strlen (message)) >= 0);

    char buf[1024];
    int num_read = read (3, buf, 1023);
    SYSASSERT(num_read >= 0);
    buf[num_read] = '\0';
    SYSASSERT(printf ("child read: %s\n", buf) >= 0);

    exit (0);
}

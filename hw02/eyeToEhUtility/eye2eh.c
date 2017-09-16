#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

/* Write a string */
#define WRITES(a) { const char *foo = a;  assert (write (STDOUT_FILENO, foo, strlen (foo)) >= 0); }
/* Write an integer i no longer than l */
#define WRITEI(i,l) { char buf[l]; assert (eye2eh (i, buf, l, l) != -1); WRITES (buf); }
/* Write a newline */
#define WRITENL { assert (write (STDOUT_FILENO, "\n", 1) >= 0); }

/*
** Convert an integer to a string. i is assumed to be positive. The number
** of characters converted is returned; it may be less that it would take
** to hold the entire number.  Numbers are right justified. The base must
** be between 2 and 16; otherwise -1 is returned. The bufsize must be
** greater than one or a -1 is returned.
*/

int eye2eh (int i, char *buf, int bufsize, int base)
{
    if (1 > bufsize) return (-1);
    buf[bufsize-1] = '\0';
    if (1 == bufsize) return (0);
    if (2 > base || 16 < base ) return (-1);

    int count = 0;
    const char *digits = "0123456789ABCDEF";
    for (int j = bufsize-2; j >= 0; j--)
    {
        /* if we're done, pad with spaces */
        if (i == 0)
        {
            buf[j] = ' ';
        }
        else
        {
            buf[j] = digits[i%base];
            i = i/base;
            count++;
        }
    }
    return (count);
}

#ifdef TEST_EYE2EH
int main()
{
    char buf[5];

    /* buffer of size 0 */
    assert (eye2eh (1, buf, 0, 10) == -1);

    /* bad base */
    assert (eye2eh (1, buf, 5, 1) == -1);

    /* buffer of size 0 */
    assert (eye2eh (0, buf, 1, 10) == 0);
    assert (strncmp (buf, "", 1) == 0);

    assert (eye2eh (1, buf, 5, 10) == 1);
    assert (strncmp (buf, "   1", 1) == 0);

    assert (eye2eh (10, buf, 5, 10) == 2);
    assert (strncmp (buf, "  10", 2) == 0);

    assert (eye2eh (12, buf, 5, 10) == 2);
    assert (strncmp (buf, "  12", 2) == 0);

    assert (eye2eh (1234, buf, 5, 10) == 4);
    assert (strncmp (buf, "1234", 4) == 0);

    /* too long */
    assert (eye2eh (12345, buf, 5, 10) == 4);
    assert (strncmp (buf, "2345", 4) == 0);

    assert (eye2eh (6, buf, 5, 2) == 3);
    assert (strncmp (buf, " 110", 3) == 0);

    assert (eye2eh (6, buf, 5, 16) == 1);
    assert (strncmp (buf, "   6", 1) == 0);

    assert (eye2eh (12, buf, 5, 16) == 1);
    assert (strncmp (buf, "   C", 1) == 0);

    assert (eye2eh (12, buf, 5, 8) == 2);
    assert (strncmp (buf, "  14", 2) == 0);
}
#endif

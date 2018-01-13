#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>

#include "orderedSet.h"

#define MAX_LINE_LEN (512)

/* call puts on string */
static int 
putsWrapper(void *arg, const char *s)
{ 
    puts(s); 
    return 0;
}

/* update hash value in *arg based on string */
static int
hashWrapper(void *arg, const char *s)
{
    uint64_t *h = arg;

    do {
        *h = *h * 97 + *s;
    } while(*s++);

    return 0;
}

/* keep string if if contains arg as a substring */
static int
grep(void *arg, const char *s)
{
    return strstr(s, (const char *) arg) != 0;
}

int
main(int argc, char **argv)
{
    char buf[MAX_LINE_LEN+1];
    size_t len;
    struct orderedSet *s;
    struct orderedSet *s2;
    uint64_t hashValue;

    if(argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }

    s = orderedSetCreate();

    while(fgets(buf, MAX_LINE_LEN, stdin) != 0) {
        /* eat the newline */
        len = strlen(buf);
        assert(buf[len-1] == '\n');
        buf[len-1] = '\0';

        switch(buf[0]) {
            case '+':
                /* add an element */
                orderedSetInsert(s, buf+1);
                break;
            case '-':
                /* remove an element */
                orderedSetDelete(s, buf+1);
                break;
            case 's':
                /* print size of the set */
                printf("%zu\n", orderedSetSize(s));
                break;
            case 'p':
                /* print all the elements */
                s2 = orderedSetFilter(s, putsWrapper, 0);
                assert(orderedSetSize(s2) == 0);
                orderedSetDestroy(s2);
                break;
            case 'h':
                /* print hash of all the elements */
                hashValue = 0;
                s2 = orderedSetFilter(s, hashWrapper, &hashValue);
                assert(orderedSetSize(s2) == 0);
                orderedSetDestroy(s2);
                printf("%016" PRIx64 "\n", hashValue);
                break;
            case 'f':
                /* filter */
                s2 = orderedSetFilter(s, grep, buf+1);
                orderedSetDestroy(s);
                s = s2;
                break;
            default:
                fprintf(stderr, "Unrecognized command %c\n", buf[0]);
                return 2;
        }
    }

    orderedSetDestroy(s);

    return 0;
}

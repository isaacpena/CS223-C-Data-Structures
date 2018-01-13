#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "safe.h"
#include "openSafe.h"

/*
 * It might make more sense to put the safe code in its own file,
 * but this keeps the compilation process simple.
 */

struct safe {
    int numTumblers;    /* how many tumblers this safe has */
    int numPositions;   /* how many positions each tumbler has */
    int *combination;   /* combination to safe */
    int turnsRemaining; /* self-destruct after this many turns, even if combination is correct */
    int contents;       /* contents of safe */
};

/*
 * Returns the number of tumblers on a safe.
 * If this is n, the possible tumbler indices will be 0 through n-1.
 * */
int
numTumblers(Safe *s)
{
    return s->numTumblers;
}

/* 
 * Returns the number of positions of each tumbler.
 * If this is n, the possible tumbler positions will be 0 through n-1.
 */
int
numPositions(Safe *s)
{
    return s->numPositions;
}

/*
 * Try a combination.
 *
 * This should be an array of ints of size numTumblers(s).
 *
 * Returns contents of safe (a non-negative int) if correct and safe
 * has not yet self-destructed.
 *
 * Returns SAFE_BAD_COMBINATION if incorrect and safe 
 * has not yet self-destructed.
 *
 * Returns SAFE_SELF_DESTRUCTED if safe has self-destructed.
 */
int 
tryCombination(Safe *s, int *combination)
{
    int i;
    int failed;

    if(s->turnsRemaining <= 0) {
        return SAFE_SELF_DESTRUCTED;
    } 

    /* else */
    s->turnsRemaining--;

    failed = 0;

    for(i = 0; i < s->numTumblers; i++) {
        /* some sanity checks */
        assert(combination[i] >= 0);
        assert(combination[i] < s->numPositions);

        /* test if this tumbler is correct */
        if(s->combination[i] != combination[i]) {
            failed++;
        }
    }

    /* clear the combination */
    for(i = 0; i < s->numTumblers; i++) {
        combination[i] = 0;
    } 

    if(failed) {
        return SAFE_BAD_COMBINATION;
    } else {
        return s->contents;
    }
}

/*
 * Create a new safe with given parameters.
 *
 * numTumblers  == number of tumblers
 * numPositions == number of positions
 * contents     == contents of the safe
 * maxTurns     == max tumbler turns before safe self destructs
 */
static Safe *
createSafe(int numTumblers, int numPositions, int contents, int maxTurns)
{
    Safe *s;
    int i;

    s = (Safe *) malloc(sizeof(struct safe));
    assert(s);

    s->numTumblers = numTumblers;
    s->numPositions = numPositions;
    s->contents = contents;
    s->turnsRemaining = maxTurns;

    s->combination = (int *) malloc(sizeof(int) * numTumblers);
    assert(s->combination);

    for(i = 0; i < numTumblers; i++) {
        s->combination[i] = rand() % numPositions;
    }

    return s;
}


/*
 * Free any space used by a safe.
 */
static void 
deleteSafe(Safe *s)
{
    free(s->combination);
    free(s);
}


int
main(int argc, char **argv)
{
    int n;
    int base;
    int contents;
    int maxTurns;
    Safe *s;

    if(argc != 5) {
        fprintf(stderr, "Usage %s numTumblers numPositions contents maxTurns\n", argv[0]);
        return 1;
    }

    /* always use the same seed to get consistent behavior */
    srand(1);

    n = atoi(argv[1]);
    base = atoi(argv[2]);
    contents = atoi(argv[3]);
    maxTurns = atoi(argv[4]);

    if(n <= 0) {
        fprintf(stderr, "At least one tumbler required\n");
        return 2;
    }

    if(base <= 0) {
        fprintf(stderr, "At least one position required\n");
        return 3;
    }

    s = createSafe(n, base, contents, maxTurns);

    printf("%d\n", openSafe(s));

    deleteSafe(s);

    return 0;
}

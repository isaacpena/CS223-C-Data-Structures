/*
 * API for safes made by the 
 * Hollywood Hackable Safe Company LLC.
 */

typedef struct safe Safe;  /* opaque data type for a safe */

/*
 * Returns the number of tumblers on a safe.
 * If this is n, the possible tumbler indices will be 0 through n-1.
 * */
int numTumblers(Safe *);

/* 
 * Returns the number of positions of each tumbler.
 * If this is n, the possible tumbler positions will be 0 through n-1.
 */
int numPositions(Safe *);

/* Return codes for tryCombination */
#define SAFE_BAD_COMBINATION (-1)
#define SAFE_SELF_DESTRUCTED (-2)

/*
 * Try a combination.
 *
 * This should be an array of numTumbler(s) ints.
 *
 * Returns contents of safe (a non-negative int) if combination is correct
 * and safe has not yet self-destructed.
 *
 * Returns SAFE_BAD_COMBINATION if combination is incorrect
 * and safe has not yet self-destructed.
 *
 * Returns SAFE_SELF_DESTRUCTED if safe has self-destructed.
 *
 * Note: may modify combination.
 */
int tryCombination(Safe *s, int *combination);

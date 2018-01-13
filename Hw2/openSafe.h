/* Include safe.h before this file to get the definition of Safe. */

/*
 * Open a safe and return the value returned by tryCombination,
 * or SAFE_SELF_DESTRUCTED if the safe self-destructed.
 */
int openSafe(Safe *);

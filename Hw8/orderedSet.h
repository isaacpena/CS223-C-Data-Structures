/*
 * Ordered set data structure.
 */

/* Make a new empty set */
struct orderedSet *orderedSetCreate(void);

/* Destroy a set */
void orderedSetDestroy(struct orderedSet *);

/* How many elements in this set? */
size_t orderedSetSize(const struct orderedSet *);

/* Insert a new element.  Has no effect if element is already present. */
void orderedSetInsert(struct orderedSet *, const char *);

/* Delete an element.  Has no effect if element is not already present. */
void orderedSetDelete(struct orderedSet *, const char *);

/* Return a new ordered set containing all elements e
 * for which predicate(arg, x) != 0.
 * The predicate function should be applied to the elements in increasing order. */
struct orderedSet *orderedSetFilter(const struct orderedSet *, int (*predicate)(void *arg, const char *), void *arg);

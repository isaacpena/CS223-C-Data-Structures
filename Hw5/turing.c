#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#define LOWERCASE_A (97)
#define DEQUE_FRONT (0)
#define DEQUE_BACK (1)

struct elt {
	struct elt *next;
	struct elt *prev;
	int value;
};

struct ll {
	struct elt *head;
	struct elt *tail;
};

struct ll *
llCreate(void) { /* Generates one element in a ll */
	struct ll *list;

	list = malloc(sizeof(struct ll));

	struct elt *e;

	e = malloc(sizeof(struct elt));

	e->value = LOWERCASE_A;

	e->next = NULL;
	e->prev = NULL;

	list->head = e;
	list->tail = e;
	
	return list;
}

void
llPush(struct ll *list, int direction, int value) {
	struct elt *e;

	e = malloc(sizeof(struct elt));
	assert(e);


	e->value = value;

	if(direction == DEQUE_FRONT) {/* the left end. 0. next is NULL. head is an element. */
		list->head->next = e;
		e->next = NULL; 
		e->prev = list->head; 
		list->head = e;
	} else { 
		list->tail->prev = e;
		e->next = list->tail;
		e->prev = NULL;
		list->tail = e;
	}
} 




int 
main(int argc, char **argv) {
	struct ll *list;

	list = llCreate();
		
	struct elt *reader;


	llPush(list, DEQUE_FRONT, LOWERCASE_A);

	llPush(list, DEQUE_BACK, LOWERCASE_A);

	reader = list->head->prev;


	int state = 1;
	int steps = 0;
	while(state != 0) {
		int letter = 3 * (reader->value - 'a');
		reader->value = argv[state][letter];
		int direction = letter + 1;
		if(argv[state][direction] == '+' && reader->prev == NULL)  {
			llPush(list, DEQUE_BACK, LOWERCASE_A);
			reader = reader->prev;
		} else if (argv[state][direction] == '+') {
			reader = reader->prev;
		}

		if(argv[state][direction] == '-' && reader->next == NULL) {
			llPush(list, DEQUE_FRONT, LOWERCASE_A);
			reader = reader->next;
		} else if(argv[state][direction] == '-') {
			reader = reader->next;
		}

		state = argv[state][letter + 2] - 48;
		steps++;
		
	}

	printf("%d\n", steps);

	struct elt *temp;
	while(list->head != NULL) {
		temp = list->head;
		list->head = list->head->prev;
		free(temp);	
	}

	free(list);

	return 0;
}
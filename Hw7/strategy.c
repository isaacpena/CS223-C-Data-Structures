#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <inttypes.h> /* for PRIx64 */
#include <limits.h>   /* for UCHAR_MAX */

#include "strategy.h"

struct pile {
    int pilesize;
	int slots;
	Card* heap;
};

struct strategy {
	int num;
	struct pile** piles;
};

/* Most code modified from lecture notes on priority queues by Jim Aspnes, 2015. */

Strategy *strategyCreate(int numPiles) {
	struct strategy *s;

	s = malloc(sizeof(struct strategy));
	s->piles = malloc(numPiles * sizeof(struct pile*));
	s->num = numPiles;


	int i;
	for(i = 0; i < numPiles; i++) {
		s->piles[i] = malloc(sizeof(struct pile));
		s->piles[i]->pilesize = 0;
		s->piles[i]->slots = 16;
		s->piles[i]->heap = malloc(s->piles[i]->slots * sizeof(Card));
	}

	return s;
}

void strategyDestroy(Strategy *s) {
	int i;
	for(i = 0; i < s->num; i++) {
		free(s->piles[i]->heap);
		free(s->piles[i]);
	}
	free(s->piles);
	free(s);
}

void addtoPile(Strategy *s, Card c, int i) {
	struct pile *lastpile = s->piles[i];

	if(lastpile->pilesize >= lastpile->slots) {
		lastpile->slots *= 2;
		lastpile->heap = realloc(lastpile->heap, sizeof(Card) * lastpile->slots);
	}

	size_t index = lastpile->pilesize++;
	size_t parent;
	lastpile->heap[index] = c;

	while(index > 0 && lastpile->heap[index] < lastpile->heap[parent = (index - 1) / 2]) {
		Card tmp = lastpile->heap[parent];
		lastpile->heap[parent] = lastpile->heap[index];
		lastpile->heap[index] = tmp;
		index = parent;
	}
}

void strategyDeal(Strategy *s, Card c) {
	addtoPile(s, c, s->num - 1);
}

Card strategyPlay(Strategy *s, int k) {
	struct pile *removefrom = s->piles[k-1];

	size_t pos;
	size_t left;
	size_t right;
	size_t smaller;
	Card retval = removefrom->heap[0];
	if(k != 1) {
		addtoPile(s, retval, k-2);
	}
	removefrom->heap[0] = removefrom->heap[--(removefrom->pilesize)];
	pos = 0;
	for(;;) {
		left = 2*pos + 1;
		right = 2*pos + 2;
		if(left >= removefrom->pilesize) {
			return retval;
		} else if (right >= removefrom->pilesize) {
			smaller = left;
		} else if (removefrom->heap[left] < removefrom->heap[right]) {
			smaller = left;
		} else {
			smaller = right;
		} 

		if(removefrom->heap[pos] > removefrom->heap[smaller]) {
			Card temp = removefrom->heap[pos];
			removefrom->heap[pos] = removefrom->heap[smaller];
			removefrom->heap[smaller] = temp;
			pos = smaller;
		} else {
			return retval;
		}
	}
}

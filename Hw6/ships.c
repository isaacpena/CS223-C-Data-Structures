#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>

#include "ships.h"

/* Shoutout to Georg Cantor for the rad hash function */
struct shipelt {
	struct ship sh;
	struct shipelt *next;
};
struct advposelt {
	struct position curr;
	struct position head;
	struct advposelt *next;
};
struct field {
	struct shipelt** shipsTable;
	struct advposelt** coordsTable;
};

void initfield(struct field *f) {
	int i = 0;
	for(i = 0; i < 10000; i++) {
		f->shipsTable[i] = malloc(sizeof(struct shipelt));
		f->coordsTable[i] = malloc(sizeof(struct advposelt));
		f->shipsTable[i]->sh.name = NO_SHIP_NAME;
		f->shipsTable[i]->next = NULL;
		f->coordsTable[i]->head.x = COORD_MAX;
		f->coordsTable[i]->head.y = COORD_MAX;
		f->coordsTable[i]->next = NULL;
	}
}

struct field *fieldCreate(void) {
	struct field *created = malloc(sizeof(struct field));
	created->shipsTable = malloc(10000 * sizeof(struct shipelt*));
	assert(created->shipsTable);
	created->coordsTable = malloc(10000 * sizeof(struct advposelt*));
	assert(created->coordsTable);
	initfield(created);
	return created;
}


void fieldDestroy(struct field *f) {
	int i = 0;
	for(i = 9999; i >= 0; i--) {
		struct shipelt *e = f->shipsTable[i];
		if(e->next == NULL) {
			free(e);
		} else {
			while(e->next != NULL) {
				struct shipelt *temp = e;
				e = e->next;
				free(temp);
			}
			free(e);
		}

		struct advposelt *j = f->coordsTable[i];
		if(j->next == NULL) {
			free(j);
		} else {
			while(j->next != NULL) {
				struct advposelt *tempj = j;
				j = j->next;
				free(tempj);
			}
			free(j);
		}
	}

	free(f->shipsTable);
	free(f->coordsTable);
	free(f);
}


int
posHash(coord x, coord y) {
	return ((((x + y) * (x + y + 1) / 2) + y) % 10000);
}
void removeThisCoordinate(struct field *f, coord x, coord y) {
	int hash = posHash(x, y);
	struct advposelt *e;
	e = f->coordsTable[hash];
	if(e->next == NULL) {
		e->head.x = COORD_MAX;
		e->head.y = COORD_MAX;
	} else {
		while(e->next != NULL) {
			struct advposelt *temp = e;
			e = e->next;
			if(e->curr.x == x && e->curr.y == y) {
				temp->next = e->next;
				free(e);
				return;
			}
		}
	}
}
char removeShip(struct field *f, coord headx, coord heady) {
	int hash = posHash(headx, heady); /* first off, locate the ship to remove. there /must/ always be one.*/
	struct shipelt *e = f->shipsTable[hash];
	char shipname;
	if(e->next == NULL) {
		shipname = e->sh.name;
		int i;
		int fin;
		if (e->sh.direction == HORIZONTAL) {
			fin = headx + e->sh.length;
			for(i = headx; i < fin; i++) {
				removeThisCoordinate(f, i, heady);
			}
		} else {
			fin = heady + e->sh.length;
			for(i = heady; i < fin; i++) {
				removeThisCoordinate(f, headx, i);
			}
		}
		free(e);
	} else {
		if(e->sh.topLeft.x == headx && e->sh.topLeft.y == heady) {
			struct shipelt *temp = e;
			int fin;
			int i;
			if(e->sh.direction == HORIZONTAL) {
				fin = headx + e->sh.length;
				for(i = headx; i < fin; i++) {
					removeThisCoordinate(f, i, heady);
				}
			} else {
				fin = heady + e->sh.length;
				for(i = heady; i < fin; i++) {
					removeThisCoordinate(f, headx, i);
				}
			}
			shipname = e->sh.name;
			temp = e->next;
			f->shipsTable[hash] = temp;
			free(e);
		} else {
			while(e->next != NULL) {
				struct shipelt *temp = e;
				e = e->next;
				int fin;
				int i;
				if(e->sh.topLeft.x == headx && e->sh.topLeft.y == heady) {
					if (e->sh.direction == HORIZONTAL) {
						fin = headx + e->sh.length;
						for(i = headx; i < fin; i++) {
							removeThisCoordinate(f, i, heady);
						}
					} else {
						fin = heady + e->sh.length;
						for(i = heady; i < fin; i++) {
							removeThisCoordinate(f, headx, i);
						}
					}
					shipname = e->sh.name;
					temp->next = e->next;
					free(e);
					break;
				}
			}
		}
	}
	return shipname;
}
void addThisCoordinate(struct field *f, coord shipx, coord shipy, coord x, coord y) {
	int hash = posHash(x, y); 
	struct advposelt *e;
	e = f->coordsTable[hash];
	if(e->next == NULL && e->head.x != COORD_MAX && e->head.y != COORD_MAX && e->curr.x == x && e->curr.y == y)  {
		removeShip(f, e->head.x, e->head.y);
	  	f->coordsTable[hash] = malloc(sizeof(struct advposelt));
	  	f->coordsTable[hash]->curr.x = x;
	  	f->coordsTable[hash]->curr.y = y;
	  	f->coordsTable[hash]->head.x = shipx;
	  	f->coordsTable[hash]->head.y = shipy;
	  	f->coordsTable[hash]->next = NULL;
	} else if(e->next == NULL && e->head.x == COORD_MAX && e->head.y == COORD_MAX)  {
		e->curr.x = x;
		e->curr.y = y;
		e->head.x = shipx;
		e->head.y = shipy;
	} else {
		struct advposelt *temp = e;
		while(e->next != NULL) {
			if(e->curr.x == x && e->curr.y == y && (e->head.x != shipx || e->head.y != shipy)) {
				removeShip(f, e->head.x, e->head.y);
			}
			temp = e;
			e = e->next;
		}
		temp->next = malloc(sizeof(struct advposelt));
		temp->next->curr.x = x;
		temp->next->curr.y = y;
		temp->next->head.x = shipx;
		temp->next->head.y = shipy;
		temp->next->next = NULL;
	}	
}


void fieldPlaceShip(struct field *f, struct ship s) {
	if(s.length < 1 || s.length > MAX_SHIP_LENGTH) {
		return;
	}
	if(s.direction == HORIZONTAL) {
		int sentinel;
		int end = s.topLeft.x + s.length;
		int numleft = s.length; 
		for(sentinel = s.topLeft.x; sentinel < end; sentinel++) {
			if(sentinel == COORD_MAX && numleft > 1) {
				return;
			}
			numleft--;
		}
	} else {
		int sentinel;
		int end = s.topLeft.y + s.length;
		int numleft = s.length;
		for(sentinel = s.topLeft.y; sentinel < end; sentinel++) {
			if(sentinel == COORD_MAX && numleft > 1)  { 
				return;
			}
			numleft--;
		}
	}

	if(s.name == NO_SHIP_NAME) {
		return;
	}
	int hash;
	hash = posHash(s.topLeft.x, s.topLeft.y);
	struct shipelt *e = f->shipsTable[hash];
	int i;
	int fin;
	if(e->sh.name == NO_SHIP_NAME && e->next == NULL) {
		if (s.direction == HORIZONTAL) {
			fin = s.topLeft.x + s.length;
			for(i = s.topLeft.x; i < fin; i++) {
				addThisCoordinate(f, s.topLeft.x, s.topLeft.y, i, s.topLeft.y);
			}
		} else {
			fin = s.topLeft.y + s.length;
			for(i = s.topLeft.y; i < fin; i++) {
				addThisCoordinate(f, s.topLeft.x, s.topLeft.y, s.topLeft.x, i);
			}
		}
		e->sh.name = s.name;
		e->sh.length = s.length;
		e->sh.direction = s.direction;
		e->sh.topLeft.x = s.topLeft.x;
		e->sh.topLeft.y = s.topLeft.y;
	} else {
		while(e->next != NULL) {
			e = e->next;
		}
		if (s.direction == HORIZONTAL) {
			fin = s.topLeft.x + s.length;
			for(i = s.topLeft.x; i < fin; i++) {
				addThisCoordinate(f, s.topLeft.x, s.topLeft.y, i, s.topLeft.y);
			}
		} else {
			fin = s.topLeft.y + s.length;
			for(i = s.topLeft.y; i < fin; i++) {
				addThisCoordinate(f, s.topLeft.x, s.topLeft.y, s.topLeft.x, i);
			}
		}
		e->next = malloc(sizeof(struct shipelt));
		e->next->sh.name = s.name;
		e->next->sh.direction = s.direction;
		e->next->sh.length = s.length;
		e->next->sh.topLeft.x = s.topLeft.x;
		e->next->sh.topLeft.y = s.topLeft.y;
		e->next->next = NULL;
	}
}

char fieldAttack(struct field *f, struct position p) {
	int attackedhash = posHash(p.x, p.y);
	char retval;
	struct advposelt *e = f->coordsTable[attackedhash];
	if(e->head.x == COORD_MAX && e->head.y == COORD_MAX && e->next == NULL ) { /* it's empty... */
		return NO_SHIP_NAME;
	} else if (e->head.x != COORD_MAX && e->head.y != COORD_MAX && (e->curr.x != p.x || e->curr.y != p.y) && e->next == NULL) { /* it's not empty, but a false hash... */
		return NO_SHIP_NAME;
	} else if (e->head.x != COORD_MAX && e->head.y != COORD_MAX && e->curr.x == p.x && e->curr.y == p.y && e->next == NULL) { /* it's not empty, and a correct hash. */
		retval = removeShip(f, e->head.x, e->head.y);
		return retval;
	} else { /* There are other elements ... */
		while(e != NULL) {
			if(e->curr.x == p.x && e->curr.y == p.y) { /* The elements here are false hashes, with one correct mark. */
				retval = removeShip(f, e->head.x, e->head.y);
				break;
			}
			e = e->next;
		}
		return retval;
	}
}

size_t fieldCountShips(const struct field *f) {
	size_t count = 0;
	int i = 0;
	for(i = 0; i < 10000; i++) {
		if(f->shipsTable[i]->next == NULL) {
			if(f->shipsTable[i]->sh.name != NO_SHIP_NAME) {
				count++;
			}
		} else {
			count++;
			struct shipelt *e = f->shipsTable[i];
			while(e->next != NULL) {
				e = e->next;
				count++;
			}
		}
	}
	return count;
}

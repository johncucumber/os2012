#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "queue.h"

#define M 100

struct Node p[M];

int spos = 0;
int rpos = 0;

void push(struct Node *q);
struct Node *pop(void);

void push(struct Node *q) {
	if (spos == M) {
		return;
	}
	p[spos] = *q;
	spos++;
}

struct Node *pop(void) {
	if (rpos == spos) {
		return NULL ;
	}
	rpos++;
	return p + rpos - 1;
}

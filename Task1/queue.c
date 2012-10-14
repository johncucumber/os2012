#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "queue.h"

#define MAX 100

struct Node p[MAX];

int spos = 0;
int rpos = 0;

void push(struct Node *q);
struct Node *pop(void);

void push(struct Node *q)
{
  if(spos==MAX) {
    printf("List Full\n");
    return;
  }
  p[spos] = *q;
  spos++;
}

struct Node *pop(void)
{
  if(rpos==spos) {
    printf("No more.\n");
    return NULL;
  }
  rpos++;
  return p+rpos-1;
}

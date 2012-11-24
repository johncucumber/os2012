#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include "queue.h"

struct Queue *new_queue(void)
{
  struct Queue *q =(struct Queue*) malloc(sizeof(struct Queue));
  q->head = NULL;
  q->tail = NULL;
  return q;
}

void push(struct Queue* q, int sock) {
	struct Node  *n = (struct Node*) malloc(sizeof(struct Node));
	n->sock = sock;
    n->next = NULL;
    if (q->tail==NULL){
		q->head=n;
		q->tail=n;
    } else {
    q->tail->next=n;
    q->tail=q->tail->next;
    }
}

struct Node *pop(struct Queue* q) {
	struct Node *n;
	
	if(q->head)
    {
	   n=q->head;
       if(q->head->next)
       {
         q->head=q->head->next;
       }
       else
       {
		     q->head = NULL;
             q->tail = NULL;
		   }
    }
		

	return n;
}

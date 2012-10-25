#include <stdio.h>
#include "queue.h"

int main(int argc, char **argv)
{
	struct Queue *q;
	q=new_queue();
	int i1 = 1;
	int i2 = 2;
	int i3 = 3;
    push(q, &i1,4);
	push(q, &i2,5);
	push(q, &i3,6);
	
	struct Node *param1 = pop(q);
	struct Node *param2 = pop(q);
	struct Node *param3 = pop(q);
	
	printf("p1 F=%d; S=%d;\n",*(param1->first),param1->size);
	printf("p2 F=%d; S=%d;\n",*(param2->first),param2->size);
	printf("p3 F=%d; S=%d;\n",*(param3->first),param3->size);
	
	return 0;
}


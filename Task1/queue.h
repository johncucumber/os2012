struct Node {
	int *first;
	int size;
	struct Node * next;
};

struct Queue {
	struct Node * head;
	struct Node * tail;
};

struct Queue *new_queue(void);
void push(struct Queue* q, int *first ,int size) ;
struct Node *pop(struct Queue* q);

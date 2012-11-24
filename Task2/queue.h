struct Node {
	int sock;
	struct Node * next;
};

struct Queue {
	struct Node * head;
	struct Node * tail;
};

struct Queue *new_queue(void);
void push(struct Queue* q, int sock);
struct Node *pop(struct Queue* q);

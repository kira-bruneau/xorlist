#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void * ptrXor(void * left, void * right) {
	return (void *)((uintptr_t)left ^ (uintptr_t)right);
}

typedef struct Node {
	struct Node * mask;
	int value;
} Node;

typedef struct List {
	struct Node * mask;
	struct Node * head;
} List;

List * createList() {
	List * list = malloc(sizeof(List));
	list->mask = 0;
	list->head = (Node *)list;
	return list;
}

Node * createNode(Node * mask, int value) {
	Node * node = malloc(sizeof(Node));
	node->mask = mask;
	node->value = value;
	return node;
}

void prependList(List * list, int value) {
	Node * head = list->head;
	Node * node = createNode(ptrXor((Node *)list, head), value);

	head->mask = ptrXor(node, ptrXor(head->mask, (Node *)list));
	list->mask = ptrXor(node, ptrXor(list->mask, head));
	list->head = node;
}

void printList(List * list) {
	Node * prev = (Node *)list;
	Node * node = list->head;

	while (node != (Node *)list) {
		printf("%i\n", node->value);
		Node * next = ptrXor(prev, node->mask);
		prev = node;
		node = next;
	}
}

int main() {
	List * list = createList();

	int i;
	for (i = 0; i < 100; i++) {
		prependList(list, i);
	}

	printList(list);
	return 0;
}

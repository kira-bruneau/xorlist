#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Node {
	struct Node * mask;
	void * value;
} Node;

typedef struct List {
	struct Node * mask;
	struct Node * head;
} List;

////////////////////////////////////////////////////////////////////////////////

void * ptrXor(void * left, void * right) {
	return (void *)((uintptr_t)left ^ (uintptr_t)right);
}

List * createList() {
	List * list = malloc(sizeof(List));
	list->mask = 0;
	list->head = (Node *)list;
	return list;
}

Node * createNode(Node * mask, void * value) {
	Node * node = malloc(sizeof(Node));
	node->mask = mask;
	node->value = value;
	return node;
}

void prependList(List * list, void * value) {
	Node * head = list->head;
	Node * node = createNode(ptrXor(list, head), value);

	head->mask = ptrXor(node, ptrXor(head->mask, list));
	list->mask = ptrXor(node, ptrXor(list->mask, head));
	list->head = node;
}

void iterateList(List * list, void (*callback)(void *)) {
	Node * prev = (Node *)list;
	Node * node = list->head;

	while (node != (Node *)list) {
		callback(node->value);
		Node * next = ptrXor(prev, node->mask);
		prev = node;
		node = next;
	}
}

void freeList(List * list) {
	Node * prev = (Node *)list;
	Node * node = list->head;

	while (node != (Node *)list) {
		Node * next = ptrXor(prev, node->mask);
		prev = node;
		free(node);
		node = next;
	}
}

////////////////////////////////////////////////////////////////////////////////

void printList(void * value) {
	printf("%i\n", value);
}

int main() {
	List * list = createList();

	uintptr_t i;
	for (i = 0; i < 100; i++) {
		prependList(list, (void *)i);
	}

	iterateList(list, printList);
	freeList(list);
	return 0;
}

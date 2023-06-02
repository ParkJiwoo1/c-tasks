#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include<stdio.h>
#include<stdlib.h>

struct Node{
	const char* data;
	int port;
	struct Node* next;
};

struct List{
	struct Node* head;
};

struct List* init_list();

void add_node(struct List* list, const char* data, int port);

void delete_node(struct List* list, const char* data, int port);

void display_list(struct List* list, int port);
#endif

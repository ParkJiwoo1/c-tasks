#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include<stdio.h>
#include<stdlib.h>

struct Node{
	const char* data;
	struct Node* next;
};

struct List{
	struct Node* head;
};

struct List* init_list();

void add_node(struct List* list, const char* data);

//void insert_node(List* list, int data, int position);

void delete_node(struct List* list, const char* data);

void display_list(struct List* list);
#endif

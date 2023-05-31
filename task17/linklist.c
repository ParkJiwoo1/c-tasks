#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linklist.h"

struct List* init_list()
{
	struct List* list = (struct List*)malloc(sizeof(struct List));
	if (list!=NULL){
		list->head = NULL;
	}
	return list;
}

void add_node(struct List* list, const char* data, int port)
{
	struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
	struct Node* current;
	if (new_node !=NULL){
		new_node->data = strdup(data);
		new_node->port = port;
		new_node->next = NULL;
		if (list->head ==NULL){
			list->head = new_node;
		}else{
			current = list->head;
			while(current->next!=NULL){
				current = current->next;
			}
			current->next = new_node;
		}
	}
}

void delete_node(struct List* list, const char* data, int port)
{
	struct Node* current;
	struct Node* temp;

	if (list->head==NULL){
		printf("list is empty\n");
		return;
	}

	if(list->head->data == data){
		temp = list->head;
		list->head = list->head->next;
		free(temp);
		return;
	}

	current = list->head;
	while(current->next!=NULL&&strcmp(current->next->data,data)!=0){
		current = current->next;
	}

	if (current->next==NULL){
		printf("no such data found\n");
		return;
	}

	temp = current->next;
	current->next = current->next->next;
	//free(temp->data);
	free(temp);


}

void display_list(struct List* list, int port){
	struct Node* current = list->head;
	while(current!=NULL){
		if(current->port==port)
			printf("[%s:%d] ",current->data,current->port);
		current = current->next;
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char name[30];
	int korean;
	int english;
	int math;
} score;

typedef struct node {
	score data;
	struct node* next;
} node;

typedef struct {
	node* head;
} linkedlist;

void initialize(linkedlist* list) {
	list->head = NULL;
}

linkedlist* 
insertion_sort (linkedlist* list, int std)
{
	node *p, *temp;
	int standard, next_standard;
	p = list->head;

	while(p->next!=NULL){

	if (std==1)
	{
		standard = p->data.korean;
		next_standard = p->next->data.korean;
	}
	else if (std==2)
	{
		standard = p->data.english;
		next_standard = p->next->data.english;
	}
	else
	{
		standard = p->data.math;
		next_standard = p->next->data.math;
	}

		if (standard>next_standard)
		{
			temp = p->next;
			p->next = p->next->next;
			temp->next = list->head;
			list->head = temp;
			p = temp;
			continue;
		}
		p = p->next;
	}
	return list;
}

node* create_node(score data){
	node* new_node = (node*)malloc(sizeof(node));
	if (new_node == NULL) {
		printf("Memory allocation failed.\n");
		exit(1);
	}
	new_node->data = data;
	new_node->next = NULL;
	return new_node;
}

void add_node(linkedlist* list, score data){
	node* new_node = create_node(data);
	if (list->head == NULL) {
		list->head = new_node;
	} else {
		node* current = list->head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_node;
	}
}

void delete_node(linkedlist* list, char* name){
	node* current = list->head;
	node* prev = NULL;

	if(list->head==NULL){
		printf("list is empty\n");
		return;
	}

	while(current!=NULL){
		if(strcmp(name,current->data.name)==0){
			if(prev==NULL){
				list->head = current->next;
			}else{
				prev->next = current->next;
			}
			free(current);
			printf("deleted %s\n",name);
			return;
		}

		prev = current;
		current = current->next;
	}
	printf("data not found\n");

}

void read_file(linkedlist* list){
	FILE* file = fopen("hakjum.txt","r");
	char line[100];
	char name[30];
	score data;

	if (file==NULL){
		printf("file open error\n");
		exit(1);
	}
	while (fgets(line, sizeof(line), file)) {
		if (strlen(line) > 1) {
			if (line[strlen(line) - 1] == ':') {
				line[strlen(line) - 1] = '\0';
			}
			else {
				sscanf(line, "%[^\n]%*c", name);
				strcpy(data.name, name);
				fgets(line, sizeof(line), file);
				sscanf(line, "%*[^:]: %d", &data.korean);
				fgets(line, sizeof(line), file);
				sscanf(line, "%*[^:]: %d", &data.english);
				fgets(line, sizeof(line), file);
				sscanf(line, "%*[^:]: %d", &data.math);
				add_node(list, data);
			}
		}
	}
	fclose(file);
}

void print(linkedlist* list) {
	node* current = list->head;
	while (current != NULL) {
		score data = current->data;
		printf("length : %d\n",strlen(data.name));
		printf("Name: %s\n", data.name);
		printf("국어: %d\n", data.korean);
		printf("영어: %d\n", data.english);
		printf("수학: %d\n", data.math);
		printf("\n");
		current = current->next;
	}
}
int main(){
	char name[30];
	char command;
	char str[10];
	int standard;

	linkedlist list;
	initialize(&list);
	read_file(&list);
	while(1){
		printf("p:print, d:delete, s:sort,  q:quit\n");
		printf("enter menu : ");
		scanf(" %c",&command);
		getchar();
		switch(command){
			case 'p':
				print(&list);
				break;
			case 'd':
				printf("input name to delete : ");
				fgets(name,sizeof(name),stdin);
				strncpy(str,name+0,strlen(name)-1);
				delete_node(&list,str);
				break;
			case 's':
				printf("sort standard\n");
				printf("1:korean, 2:english, 3:math : ");
				scanf("%d",&standard);
				insertion_sort(&list,standard);
				break;
			case 'q':
				exit(0);
			default :
				break;
		}
	}
}

/*
 * vim:ai:bs=2:sts=2:sw=2:expandtab:nowrap
 */




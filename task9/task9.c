#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define MAX_SIZE 5

struct node{
	int data;
	struct node* next;
};

struct circular_queue{
	struct node* front;
	struct node* rear;
	int size;
};

struct circular_queue* init_queue(){
	struct circular_queue* queue=(struct circular_queue*)malloc(sizeof(struct circular_queue));
	queue->front = NULL;
	queue->rear = NULL;
	queue->size = 0;
	return queue;
}

bool is_empty(struct circular_queue* queue){
	return queue->size == 0;
}

bool is_full(struct circular_queue* queue){
	return queue->size>=MAX_SIZE;
}

void enqueue(struct circular_queue* queue, int data){
	struct node* new_node = (struct node*)malloc(sizeof(struct node));
	struct node* temp;

	new_node->data = data;
	new_node->next = NULL;

	if (is_empty(queue)) {
		new_node->next = new_node;
		queue->front = new_node;
		queue->rear = new_node;
	} else if (is_full(queue)) {
		temp = queue->rear->next;
		queue->rear->next = new_node;
		new_node->next = temp->next;
		queue->rear = new_node;
		queue->front = new_node->next;
		free(temp);
	} else {
		new_node->next = queue->front;
		queue->rear->next = new_node;
		queue->rear = new_node;
	}

	queue->size++;
}

int dequeue(struct circular_queue* queue){
	int data;
	struct node* temp;

	if(is_empty(queue)){
		printf("queue empty\n");
		return -1;
	}

	data = queue->front->data;
	temp = queue->front;

	if(queue->front==queue->rear){
		queue->front = NULL;
		queue->rear = NULL;
	}else{
		queue->front = queue->front->next;
		queue->rear->next = queue->front;
	}	

	free(temp);
	queue->size-=1;
	return data;
}

void print(struct circular_queue* queue){
	struct node* cur_node = queue->front;

	if(is_empty(queue)){
		printf("queue is empty\n");
		return;
	}

	do{
		printf("[%d] ",cur_node->data);
		cur_node = cur_node->next;
	}while(cur_node!=queue->front);

	printf("\n");
}

int main(){
	struct circular_queue* queue=init_queue();
	bool empty;
	char menu;
	int data;

	while(1){
		printf("e:enque, d:dequeue, p:print, q:quit\n");
		printf("input menu : ");
		scanf(" %c",&menu);
		switch(menu){
			case 'e':
				printf("input number data : ");
				scanf("%d",&data);
				enqueue(queue, data);
				break;
			case 'd':
				printf("delete data : ");
				dequeue(queue);
				break;
			case 'p':
				print(queue);
				break;
			case 'q':
				exit(1);
		}
	}

	return 0;
}



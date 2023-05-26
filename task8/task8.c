#include<stdio.h>
#include<stdlib.h>
#define HASH_SIZE 20

struct node{
	int key;
	int value;
	struct node* next;
};

struct table{
	struct node* head;
	int count;
};

struct table* hash_table = NULL;

int hash_func(int key){
	return key%HASH_SIZE; //make 20 hash table array
}

struct node* create_node(int key, int value){
	struct node* new_node;
	int idx = hash_func(key);
	new_node = (struct node *)malloc(sizeof(struct node));

	new_node->key=idx;
	new_node->value=value;
	new_node->next=NULL;

	return new_node;
}

void add(int key, int value){
	int idx = hash_func(key);

	struct node* new_node = create_node(key,value);

	/*if table is empty*/
	if (hash_table[idx].count==0){	
		hash_table[idx].head=new_node;
	}else{
		/*add next to node already exist*/
		new_node->next = hash_table[idx].head;
		hash_table[idx].head=new_node;
	}
	hash_table[idx].count+=1;
	return;
}

void delete(int key){
	int idx = hash_func(key);
	/*flag for inform node deleted*/
	int flag=0;

	struct node* node;
	struct node* before;

	node = hash_table[idx].head;

	while(node!=NULL){
		if (node->key==key){
			/*head node is what we're searching for*/
			if(node==hash_table[idx].head){
				hash_table[idx].head = node->next;
			}else{
				before->next = node->next;
			}

			hash_table[idx].count-=1;
			free(node);
			flag=1;
			break;
		}
		before = node;
		node = node->next;
	}

	if (flag==1){
		printf("%d deleted\n",key);
	}else{
		printf("%d not found\n");
	}
}

void display(){
	struct node* node;
	int i;

	for(i=0;i<HASH_SIZE;i++){
		node = hash_table[i].head;
		printf("table[%d] : ",i);
		while(node!=NULL){
			printf("[key : %d, val : %d] -> ",node->key,node->value);
			node = node->next;
		}
		printf("\n");
	}
	return;
}

int main(){
	hash_table = (struct table *)malloc(HASH_SIZE * sizeof(struct table));
	char command;
	int key,value;
	while(1){
		printf("a:add, d:delete, p:show table, q:quit\n");
		printf("input command : ");
		scanf(" %c",&command);
		switch(command){
			case 'a':
				printf("input key:");
				scanf("%d",&key);
				printf("input value:");
				scanf("%d",&value);
				add(key,value);
				break;
			case 'd':
				printf("input key:");
				scanf("%d",&key);
				delete(key);
				break;
			case 'p':
				display();
				break;
			case 'q':
				exit(0);
		}
	}
	return 0;
}

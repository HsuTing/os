#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

typedef struct node* nodePointer;
struct node {
	int data;
	nodePointer right;
	nodePointer left;
};
int count[8];
int search_num[8];
nodePointer head;

void add_node(nodePointer head, int data);
void print_node(nodePointer head);
void search_node(nodePointer head, int j);
void* thread(void *);

int main(int argc, char *argv[]) {
	FILE* fp;
	int num = 0, i = 0;
	char temp;
	void *result;
	pthread_t t[8];
	head = malloc(sizeof(struct node));

	memset(count, 0, sizeof(count));
	memset(search_num, 0, sizeof(search_num));

	fp = fopen(argv[1], "r");
	if(fp == NULL) {
		perror("Error opening file.");
	}
	else{
		while((temp = fgetc(fp)) != '\n') {
			if(temp == ' ') {
				search_num[i] = num;
				i++;
				num = 0;
			}
			else {
				num = num * 10 + (temp - 48);
			}
		}
		search_num[i] = num;
		i++;

		fscanf(fp, "%d", &num);
		head->data = num;
		head->right = NULL;
		head->left = NULL;

		int j = 1;
		for(; j < 1000; j++) {
			fscanf(fp, "%d", &num);
			add_node(head, num);
		}
		fclose(fp);

		for(j = 0; j < i; j++) {
			pthread_create(&t[j], NULL, thread, (void *)j);
		}
		for(j = 0; j < i; j++) {
			pthread_join(t[j], &result);
		}
		fp = fopen(argv[2], "w");
		for(j = 0; j < i; j++) {
			fprintf(fp, "%d %d\n", search_num[j], count[j]);
		}
		fclose(fp);
	}

	return 0;
}

void add_node(nodePointer head,int data) {
	nodePointer now = malloc(sizeof(struct node));
	now = head;

	while(now->right != NULL || now->left != NULL) {
		if(data > now-> data) 	{
			if(now->right == NULL) {
				break;
			}
			else {
				now = now->right;
			}
		}
		else {
			if(now->left == NULL) {
				break;
			}
			else {
				now = now->left;
			}
		}
	}

	nodePointer temp = malloc(sizeof(struct node));
	temp->data = data;
	temp->right = NULL;
	temp->left = NULL;
	if(data > now->data) {
		now->right = temp;
	}
	else {
		now->left = temp;
	}
}

void print_node(nodePointer head) {
	nodePointer now = malloc(sizeof(struct node));
	now = head;

	if(now->left != NULL) {
		print_node(now->left);
	}
	if(now->right != NULL) {
		print_node(now->right);
	}
	printf("%d ", now->data);
}

void search_node(nodePointer head, int j) {
	nodePointer now = malloc(sizeof(struct node));
	now = head;

	if(now->left != NULL) {
		search_node(now->left, j);
	}
	if(now->right != NULL) {
		search_node(now->right, j);
	}
	if(now->data == search_num[j]) {
		count[j]++;
	}
}

void* thread(void* arg) {
	int j = (int)arg;
	search_node(head, j);
}

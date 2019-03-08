#ifndef LIST_H_ 
#define LIST_H_
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>



typedef struct node_t 
{
	int num;
	struct node_t* next;
	struct node_t* prev;
	pthread_mutex_t* lock;
}*Node;

typedef struct list_t 
{
	Node head;
	Node last;
}*List;

List ListCreate(int N);
//int ListContains(List list,int num);
//int ListRemove(List list,int num);
void ListDestroy(List list);
//int ListSize(List list); // maybe should delete this

Node NodeCreate(int num,Node next,Node prev);


#endif

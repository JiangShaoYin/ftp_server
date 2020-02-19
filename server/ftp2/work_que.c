#include "factory.h"

void que_init(fd_queue* pq,int capacity)
{
	pq->que_capacity=capacity;
	pq->que_size=0;
	pthread_mutex_init(&pq->que_mutex,NULL);
}

void que_set(fd_queue* pq,node_t* pnew)
{
	if(pq->que_head==NULL)
	{
		pq->que_head=pnew;
		pq->fd_queueail=pnew;
	}else{
		pq->fd_queueail->pNext=pnew;
		pq->fd_queueail=pnew;
	}
	pq->que_size++;
}

void que_get(fd_queue* pq,pnode_t* p)
{
	*p=pq->que_head;
	pq->que_head=pq->que_head->pNext;
	pq->que_size--;
}

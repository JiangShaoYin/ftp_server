#include "factory.h"

void que_init(fd_queue* fd_que, int capacity) {
	fd_que->que_capacity = capacity;
	fd_que->que_size = 0;
	pthread_mutex_init(&fd_que->que_mutex, NULL);
}

void que_set(fd_queue* fd_que, fd_node* pnew) {
	if(fd_que->que_head == NULL) {
		fd_que->que_head = pnew;
		fd_que->que_tail = pnew;
	}else{
		fd_que->que_tail->pNext = pnew;
		fd_que->que_tail = pnew;
	}
	fd_que->que_size++;
}

void que_get(fd_queue* fd_que, p_fd_node* p) {
	*p=fd_que->que_head;
	fd_que->que_head = fd_que->que_head->pNext;
	fd_que->que_size--;
}

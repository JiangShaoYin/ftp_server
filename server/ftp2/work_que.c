#include "ThreadPooltory.h"

void que_init(SocketQueue* socket_que, int capacity) {
	socket_que->que_capacity = capacity;
	socket_que->que_size = 0;
	pthread_mutex_init(&socket_que->mutex_, NULL); // 初始化锁
}

void que_add(SocketQueue* socket_que, SocketNode* socket_node) { 
	if(socket_que->que_head == NULL) {
		socket_que->que_head = socket_node;
		socket_que->que_tail = socket_node;
	} else { // add socket_node to the tail of que_add
		socket_que->que_tail->pNext = socket_node;
		socket_que->que_tail = socket_node;
	}
	socket_que->que_size++;
}

void que_get(SocketQueue* socket_que, p_SocketNode* p) {
	*p = socket_que->que_head;
	socket_que->que_head = socket_que->que_head->pNext;
	socket_que->que_size--;
}

#ifndef __WORK_QUE_H__
#define __WORK_QUE_H__
#include "md5.h"
//队列节点结构
typedef struct tag_node {
	int new_socket_fd;
	struct tag_node* pNext;
}socket_node;
//队列结构体
typedef struct{
	socket_node* que_head;
	socket_node* que_tail;
	int que_capacity;
	int que_size;
	pthread_mutex_t que_mutex;
}socket_queue;
//队列初始化
void que_init(socket_queue*, int);
//队列元素集合
void que_set(socket_queue*, socket_node*);
//获取队列元素
void que_get(socket_queue*, socket_node*);
#endif

#ifndef __WORK_QUE_H__
#define __WORK_QUE_H__
#include "md5.h"
//队列节点结构
typedef struct tag_node {
	int new_fd;
	struct tag_node* pNext;
}fd_node,*p_fd_node;
//队列结构体
typedef struct{
	pnode_t que_head, fd_queueail;
	int que_capacity;
	int que_size;
	pthread_mutex_t que_mutex;
}fd_queue, *p_fd_queue;
//队列初始化
void que_init(fd_queue*, int);
//队列元素集合
void que_set(fd_queue*, node_t*);
//获取队列元素
void que_get(fd_queue*, pnode_t*);
#endif

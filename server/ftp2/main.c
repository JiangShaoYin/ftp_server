#include"ThreadPooltory.h"

// typedef struct{
// 	pthread_t* pthids;
// 	pthread_cond_t cond;
// 	SocketQueue que;
// 	int pthread_num;
// 	p_func thread_func;
// 	int start_flag;
// }ThreadPool, *pThreadPool;


// typedef struct{
// 	SocketNode* que_head, que_tail;
// 	int que_capacity;
// 	int que_size;
// 	pthread_mutex_t mutex_;
// }SocketQueue, *SocketQueue*;

void* thread_handle(void* p) {
	ThreadPool* thread_pool = (ThreadPool*)p;
	SocketQueue* socket_que = &thread_pool->que;//取队列
	SocketNode* new_socket;
	char path[100] = {0};
	getcwd(path, sizeof(path));//获取当前目录路径
	while(1) {
		pthread_mutex_lock(&socket_que->mutex_);
		if(socket_que->que_size == 0) { // 阻塞在这个地方，等待客户连接。
			pthread_cond_wait(&thread_pool->cond, &socket_que->mutex_); //socket都在使用， 子线程都忙， 等待哪个子线程闲下来
		}
		que_get(socket_que, &new_socket);// 取队列元素
		pthread_mutex_unlock(&socket_que->mutex_);

		handle_cmd(new_socket->fd, path);//传递fd，当前绝对路径
		free(new_socket);
	}
}



int main(int argc,char** argv) {
	if(argc!=5) {
		printf("./server lack of args IP PORT Thread_NUM CAPACITY");
		return -1;
	}
	
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd == -1) {
		perror("socket");
		return -1;
	}
	int reuse = 1;
	int ret;
	ret = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)); // set reuse == 1,  when restart the program, the port is still available. 
	if(ret == -1) {
		perror("setsockopt");
		return -1;
	} // 必须放在bind之前
	struct sockaddr_in server;
	bzero(&server, sizeof(server));
	// memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2])); // 服务器端口号
	server.sin_addr.s_addr = inet_addr(argv[1]); // 服务器地址
	ret = bind(socket_fd, (struct sockaddr*)&server, sizeof(server)); // 端口号和ip交给server
	if(ret == -1) {
		perror("bind");
		return -1;
	}

	ThreadPool thread_pool; // pid, 线程数，维持的socket fd, 线程函数thread_func
	bzero(&thread_pool, sizeof(f));//初始化
	thread_pool.pthread_num = atoi(argv[3]);
	int capacity = atoi(argv[4]);
	ThreadPoolInit(&thread_pool, thread_handle, capacity); // 创建capacity个子线程
	ThreadPoolStart(&thread_pool);//创建线程。

	ret = listen(socket_fd, 10); // 一个socket可以排队的最大连接个数为10个
	if(ret == -1) {
		perror("listen");
		return -1;
	}
	int fd;
	SocketNode* socket_node;
	SocketQueue* socket_que = &thread_pool.que;

// typedef struct{
// 	SocketNode que_head, que_tail;
// 	int que_capacity;
// 	int que_size;
// 	pthread_mutex_t mutex_;
// }SocketQueue, *SocketQueue*;

	while(1) {
		new_fd = accept(socket_fd, NULL, NULL); // socket_fd是父进程监听的socket描述符，必须使用while循环，每次循环阻塞在accept函数，等待新的连接到来，这样才能返回新的socket。
		writeFile("connected");
		socket_node = (SocketNode*)calloc(1, sizeof(SocketNode));
		socket_node->fd = new_fd;

		pthread_mutex_lock(&socket_que->mutex_);
		que_add(socket_que, socket_node);//socket_node加入队尾
		pthread_mutex_unlock(&socket_que->mutex_);

		pthread_cond_signal(&thread_pool.cond);//发送信号，让等待cond条件的线程激活。
	}
	return 0;
}

// typedef struct tag_node {
// 	int fd;
// 	struct tag_node* pNext;
// }SocketNode,*SocketNode;
#include"factory.h"

// typedef struct{
// 	pthread_t* pthid;
// 	pthread_cond_t cond;
// 	fd_queue que;
// 	int pthread_num;
// 	p_func thread_func;
// 	int start_flag;
// }fac, *pfac;


// typedef struct{
// 	pfd_node que_head, que_tail;
// 	int que_capacity;
// 	int que_size;
// 	pthread_mutex_t que_mutex;
// }fd_queue, *p_fd_queue;

void* thread_handle(void* p) {
	fac* p_fac = (fac*)p;
	fd_queue* fd_que = &p_fac->que;//取队列
	fd_node* pcur;
	char path[100]={0};
	getcwd(path, sizeof(path));//获取目录路径
	while(1) {
		pthread_mutex_lock(&fd_que->que_mutex);
		if(fd_que->que_size == 0) {
			pthread_cond_wait(&p_fac->cond, &fd_que->que_mutex); // 排队等待条件成立
		}
		que_get(fd_que, &pcur);//取队列元素
		pthread_mutex_unlock(&fd_que->que_mutex);
		handle_cmd(pcur->new_fd, path);//传递new_fd，当前绝对路径
		free(pcur);
	}
}



int main(int argc,char** argv) {
	if(argc!=5) {
		printf("./server lack of args IP PORT Thread_NUM CAPACITY");
		return -1;
	}
	fac f; // pid, 线程数，维持的socket fd, 线程函数thread_func
	bzero(&f, sizeof(f));//初始化
	f.pthread_num = atoi(argv[3]);
	int capacity = atoi(argv[4]);
	factory_init(&f, thread_handle, capacity); // 创建capacity个子线程
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd == -1) {
		perror("socket");
		return -1;
	}
	int reuse = 1;
	int ret;
	ret = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)); // set reuse == 1,  when restart the program, the port is still available. 
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
	ret = bind(sfd, (struct sockaddr*)&server, sizeof(server)); // 端口号和ip交给server
	if(ret == -1) {
		perror("bind");
		return -1;
	}
	factory_start(&f);//创建线程。

	ret = listen(sfd, 10); // 一个socket可以排队的最大连接个数为10个
	if(ret == -1) {
		perror("listen");
		return -1;
	}
	int new_fd;
	fd_node* pnew;
	fd_queue* fd_que = &f.que;

// typedef struct{
// 	p_fd_node que_head, que_tail;
// 	int que_capacity;
// 	int que_size;
// 	pthread_mutex_t que_mutex;
// }fd_queue, *p_fd_queue;

	while(1) {
		new_fd = accept(sfd, NULL, NULL); // sfd是父进程监听的socket描述符，必须使用while循环，每次循环阻塞在accept函数，等待新的连接到来，这样才能返回新的socket。
		writeFile("connected");
		pnew = (fd_node*)calloc(1, sizeof(fd_node));
		pnew->new_fd = new_fd; 
		pthread_mutex_lock(&fd_que->que_mutex);
		que_set(fd_que, pnew);//pnew加入队尾
		pthread_mutex_unlock(&fd_que->que_mutex);
		pthread_cond_signal(&f.cond);//发送信号，让等待cond条件的线程激活。
	}
	return 0;
}

// typedef struct tag_node {
// 	int new_fd;
// 	struct tag_node* pNext;
// }fd_node,*p_fd_node;
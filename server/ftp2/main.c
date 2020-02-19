#include"factory.h"

void* thread_handle(void* p) {
	fac* pf=(fac*)p;
	que_t* pq=&pf->que;//取队列
	node_t* pcur;
	char path[100]={0};
	getcwd(path,sizeof(path));//获取目录路径
	while(1) {
		pthread_mutex_lock(&pq->que_mutex);
		if(!pq->que_size) {
			pthread_cond_wait(&pf->cond,&pq->que_mutex);
		}
		que_get(pq,&pcur);//取队列元素
		pthread_mutex_unlock(&pq->que_mutex);
		handle_cmd(pcur->new_fd,path);//传递new_fd，当前绝对路径
		free(pcur);
	}
}



int main(int argc,char** argv) {
	if(argc!=5) {
		printf("./server lack of args IP PORT Thread_NUM CAPACITY");
		return -1;
	}
	fac f;
	bzero(&f,sizeof(f));//初始化
	f.pthread_num=atoi(argv[3]);
	int capacity=atoi(argv[4]);
	factory_init(&f,thread_handle,capacity);
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1) {
		perror("socket");
		return -1;
	}
	int reuse=1;
	int ret;
	ret=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
	if(ret==-1) {
		perror("setsockopt");
		return -1;
	}//必须放在bind之前
	struct sockaddr_in sevr;
	bzero(&sevr,sizeof(sevr));
	//memset(&sevr,0,sizeof(sevr));
	sevr.sin_family = AF_INET;
	sevr.sin_port=htons(atoi(argv[2]));
	sevr.sin_addr.s_addr=inet_addr(argv[1]);
	ret=bind(sfd, (struct sockaddr*)&sevr, sizeof(sevr));
	if(ret==-1)	{
		perror("bind");
		return -1;
	}
	factory_start(&f);//线程动作

	ret=listen(sfd,10);
	if(ret==-1) {
		perror("listen");
		return -1;
	}
	int new_fd;
	node_t* pnew;
	que_t* pq=&f.que;
	while(1) {
		new_fd=accept(sfd,NULL,NULL);
		writeFile("connected");
		pnew=(node_t*)calloc(1, sizeof(node_t));
		pnew->new_fd = new_fd;
		pthread_mutex_lock(&pq->que_mutepqx);
		que_set(pq, pnew);//new_fd加入队列
		pthread_mutex_unlock(&pq->que_mutex);
		pthread_cond_signal(&f.cond);//有任务，触发条件变量
	}
	return 0;
}


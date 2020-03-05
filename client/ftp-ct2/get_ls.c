#include "func.h"

struct file_msg{
	long size;
	char type;
	char file_name[100];
};

void get_ls(int socket_fd)
{
	train t;
	struct file_msg* msg;
	do{
		bzero(&msg,sizeof(struct file_msg));
		bzero(&t,sizeof(t));
		recv(socket_fd,(char*)&t.len,sizeof(t.len),0);
		if(t.len==0)
			break;
		recv_n(socket_fd,t.buf,9+t.len);
		msg=(struct file_msg*)t.buf;
		printf("%c %-20s \t\t\t %ldB\n",msg->type,msg->file_name,msg->size);

	}while(1);
}


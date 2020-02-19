#include "func.h"

struct file_msg{
	long size;
	char type;
	char file_name[100];
};

void get_ls(int sfd)
{
	train t;
	struct file_msg* msg;
	do{
		bzero(&msg,sizeof(struct file_msg));
		bzero(&t,sizeof(t));
		recv(sfd,(char*)&t.len,sizeof(t.len),0);
		if(t.len==0)
			break;
		recv_n(sfd,t.buf,9+t.len);
		msg=(struct file_msg*)t.buf;
		printf("%c %-20s \t\t\t %ldB\n",msg->type,msg->file_name,msg->size);

	}while(1);
}


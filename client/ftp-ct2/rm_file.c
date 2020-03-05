#include"func.h"

void rm_file(int socket_fd,char* filename)
{
	char msg[100]={0};
	send(socket_fd,filename,strlen(filename),0);
	recv(socket_fd,msg,sizeof(msg),0);
	printf("%s\n",msg);
}

#include "func.h"

void get_pwd(int socket_fd)
{
	char pwdpath[100]={0};
	recv(socket_fd,pwdpath,sizeof(pwdpath),0);
	printf("current directory is:%s\n",pwdpath);
}

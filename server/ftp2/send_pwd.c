#include "ThreadPooltory.h"

void send_pwd(int fd,char* path)
{
	send(fd,path,strlen(path),0);
	writeFile("client get current pwd");
}


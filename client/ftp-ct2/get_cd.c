#include "func.h"

void get_cd(int sfd,char* cmd)
{
	if(cmd==NULL)
	{
		printf("please enter the right command\n");
		return;
	}
	char path[100]={0};
	send(sfd,cmd,strlen(cmd),0);
	recv(sfd,path,sizeof(path),0);
	printf("%s:\n",path);
}

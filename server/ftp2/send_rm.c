#include "factory.h"
void delete(char* filename,int new_fd)
{
	char msg[100]={0};
	if(unlink(filename)<0)
	{
		strcpy(msg,"Delete file failed,please check the file name");
		send(new_fd,msg,strlen(msg),0);
		bzero(&msg,sizeof(msg));
		sprintf(msg,"Delete file:%s failed!",filename);
		writeFile(msg);
	}
	else{
		strcpy(msg,"The file is deleted");
		send(new_fd,msg,strlen(msg),0);
		bzero(&msg,sizeof(msg));
		sprintf(msg,"Delete file:%s success!",filename);
		writeFile(msg);
	}
}
 
void send_rm(int new_fd,char* path)
{
	DIR* dir;
	struct dirent* p;
	char rmpath[100]={0};
	strcpy(rmpath,path);
	chdir(rmpath);
	dir=opendir(rmpath);
	char filename[100]={0};
	recv(new_fd,filename,sizeof(filename),0);
	while((p=readdir(dir))!=NULL)
	{
		if(!strcmp(p->d_name,filename)&&p->d_type==8)
		{
			delete(filename,new_fd);
			break;
		}
	}
	if(p==NULL)
	{
		delete(filename,new_fd);
	}//说明没有相应文件，发送失败信息
	chdir(path);
}


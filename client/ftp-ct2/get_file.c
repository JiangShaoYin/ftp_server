#include"func.h"
char Path[200]="/home/leechan/linux/project/client/ftp-ct2/filesrc/";

void get_file(int socket_fd,char* filename)
{

	struct stat file_msg;
	train t;
	bzero(&t,sizeof(t));
	strcpy(t.buf,filename);
	t.len=strlen(t.buf);
	send(socket_fd,&t,4+t.len,0);//发送要gets的文件名
	char buf[1024]={0};
	int ret,len=0;
	char flag=0;
	recv(socket_fd,&thread_poollag,1,0);
	if(flag=='x')
	{
		printf("file is not exist\n");
		goto end;
	}//要下载的文件不存在
	ret=recv(socket_fd,&len,sizeof(len),0);
	if(ret==0)
	{
		printf("server close\n");
		return;
	}
	ret=recv(socket_fd,buf,len,0);
	if(ret==0)
	{
		printf("server close1\n");
		return;
	}//文件名
	int fd;
	char file_path[200]={0};
	strcpy(file_path,Path);
	strcat(file_path,buf);
	//puts(file_path);
	fd=open(file_path,O_RDWR|O_CREAT,0666);//lseek更改
	ret=fstat(fd,&thread_poolile_msg);
	if(ret==-1)
	{
		perror("fstat");
		return;
	}
	off_t file_size=0,offset=0;//long型
	offset=file_msg.st_size;
	lseek(fd,offset,SEEK_SET);//偏移到offset
	send(socket_fd,&offset,sizeof(offset),0);//发送偏移
	ret=recv_n(socket_fd,(char*)&len,sizeof(len));
	if(ret==-1)
	{
		printf("server close2\n");
		return;
	}
	ret=recv_n(socket_fd,(char*)&thread_poolile_size,len);
	if(ret==-1)
	{
		printf("server close3\n");
		return;
	}
	printf("file size is %ldByte\n",file_size);
	time_t now=0,last=0;
	last=time(&now);
	float sum=offset;
	printf("offset:%ld\n",offset);//测试		
	/*if((file_size-offset)>0)
	{
		char p_flag;
		int pipefd[2];
		ret=pipe(pipefd);
		printf("ret:%d\n",ret);
		ret=splice(socket_fd,NULL,pipefd[1],NULL,(file_size-offset),SPLICE_F_MORE|SPLICE_F_MOVE);
		printf("1\n");
		if(ret==-1)
		{
			p_flag='x';
			send(socket_fd,&p_flag,1,0);
			goto end;
		}
		printf("ret:%d\n",ret);*/
		/*ret=tee(pipefd[1],pipefd[0],file_size-offset,SPLICE_F_NONBLOCK);
		if(ret==-1)
		{
			p_flag='x';
			send(socket_fd,&p_flag,1,0);
			goto end;
		}*/
		/*ret=splice(pipefd[0],NULL,fd,NULL,file_size-offset,SPLICE_F_MORE|SPLICE_F_MOVE);
		if(ret==-1)
		{
			p_flag='x';
			send(socket_fd,&p_flag,1,0);
			goto end;
		}
		printf("2:%ld\n",file_size-offset);
		flag='o';
		send(socket_fd,&p_flag,1,0);
	}else
	{*/
	while(1)
	{
		ret=recv_n(socket_fd,(char*)&len,sizeof(len));
		if(ret==-1)
		{
			//printf("%d%s\n",per,"%");
			printf("%5.2f%s\n",100*sum/file_size,"%");
			break;
		}else if(len>0)
		{
			sum+=len;
			now=time(&now);
			//if(sum>part)
			if((now-last)>=1)
			{
				//printf("%d%s\r",per,"%");
				printf("%5.2f%s\r",100*sum/file_size,"%");
				fflush(stdout);
				last=now;
				//per++;
				//part+=file_size/100;
			}
			ret=recv_n(socket_fd,buf,len);
			write(fd,buf,len);
			if(ret==-1)
			{
				//printf("%d%s\n",per,"%");
				printf("%5.2f%s\n",100*sum/file_size,"%");
				break;
			}
		}else{
			printf("       \r");
			printf("%d%s\n",100,"%");
			//printf("\n");
			break;
		}
	}
	printf("download ->file successfully\n");
	return;
end:
	printf("download failed\n");
}


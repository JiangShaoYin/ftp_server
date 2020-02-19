#include"factory.h"
char Path[200]="/home/leechan/linux/project/server/ftp2/source/upload/";

void get_file(int new_fd,char* username)
{
	struct stat file_msg;
	train t;
	bzero(&t,sizeof(t));
	char buf[1024]={0};
	char encrypted[35]={0};
	int ret,len=0;
	char flag=0;
	recv(new_fd,&flag,1,0);
	if(flag=='x')//客户端是否出错
	{
		writeFile("client upload wrong files 1!");
		printf("user enter wrong filename\n");
		goto end;
	}//收到x就退出case
	ret=recv(new_fd,&len,sizeof(len),0);
	if(ret==0)
	{
		printf("client close 1\n");
		goto end;
	}
	ret=recv(new_fd,buf,len,0);
	if(ret==0)
	{
		printf("client close 2\n");
		goto end;
	}//文件名
	//拿文件名去匹配MD5码
	recv(new_fd,&flag,1,0);//客户端是否出错
	if(flag=='x')
	{
		writeFile("client upload wrong files 2!");
		printf("user enter wrong filename\n");
		goto end;
	}
	recv(new_fd,encrypted,sizeof(encrypted),0);//收MD5码
	char filename[50]={0};
	strcpy(filename,buf);
	ret=Is_file_md5(username,filename,encrypted);//用户名，文件名，MD5码
	if(ret!=1)
	{
		flag='x';
		send(new_fd,&flag,1,0);//数据库有文件
		writeFile("client upload wrong files MD5 existed!");
		goto end;
	}else{
		flag='o';
		send(new_fd,&flag,1,0);//可以上传
	}
	int fd;
	char file_path[200]={0};
	char mysql_path[200]={0};
	sprintf(file_path,"%s%s%s%s",Path,username,"/",filename);//保证用户只能上传到自己的目录
	sprintf(mysql_path,"%s%s",Path,username);
	puts(file_path);//文件目录绝对路径
	fd=open(file_path,O_RDWR|O_CREAT,0666);
	ret=fstat(fd,&file_msg);
	if(ret==-1)
	{
		perror("fstat");
		return;
	}
	off_t file_size=0,offset=0;
	offset=file_msg.st_size;
	lseek(fd,offset,SEEK_SET);
	send(new_fd,&offset,sizeof(offset),0);
	ret=recv_n(new_fd,(char*)&len,sizeof(len));
	if(ret==-1)
	{
		printf("client close 3\n");
		goto end;
	}
	ret=recv_n(new_fd,(char*)&file_size,len);
	if(ret==-1)
	{
		printf("client close 4\n");
		goto end;
	}
	//文件长度
	printf("file size is %ldByte\n",file_size);
	time_t now=0,last=0;
	last=time(&now);
	float sum=offset;
	while(1)
	{
		ret=recv_n(new_fd,(char*)&len,sizeof(len));
		if(ret==-1)
		{
			printf("%5.2f%s\n",100*sum/file_size,"%");
			break;
		}else if(len>0)
		{
			sum+=len;
			now=time(&now);
			if((now-last)>=1)
			{
				printf("%5.2f%s\r",100*sum/file_size,"%");
				fflush(stdout);
				last=now;
			}
			ret=recv_n(new_fd,buf,len);
			write(fd,buf,len);
			if(ret==-1)
			{
				printf("%5.2f%s\n",100*sum/file_size,"%");
				break;
			}
		}else{
			printf("          \r");
			printf("%d%s\n",100,"%");
			break;
		}
	}
	writeFile("client upload file success!");
	close(fd);
	ret=md5_insert_mysql(username,filename,encrypted,mysql_path);//接收完文件判断后存数据库
	return;
end:
	writeFile("client upload file failed!");
	close(fd);
}

#include"func.h"
char path[200]="/home/leechan/linux/project/client/ftp-ct2/filesrc/";
void tran_file(int sfd,char*filename)
{
	train t;
	bzero(&t,sizeof(t));
	strcpy(t.buf,filename);
	t.len=strlen(t.buf);
	char file_path[200]={0};
	strcpy(file_path,path);
	strcat(file_path,filename);
	int fd=open(file_path,O_RDWR);
	char ok='o',wr='x';
	if(fd==-1)
	{
		send(sfd,&wr,sizeof(wr),0);//通知对端跳出case
		perror("open");
		goto end;
	}
	else{
		send(sfd,&ok,sizeof(ok),0);
	}//有此文件也要发送
	send(sfd,&t,4+t.len,0);//发文件名
	int ret;
	char encrypted[35]={0};
	ret=MD5_file(file_path,encrypted);
	puts(encrypted);
	if(ret==-1)
	{
		send(sfd,&wr,1,0);
		goto end;
	}else{
		send(sfd,&ok,1,0);
	}
	send(sfd,encrypted,strlen(encrypted),0);//发MD5码
	char flag;
	recv(sfd,&flag,1,0);
	if(flag=='x')
	{
		goto end;
	}//有相同文件不可上传
	struct stat buf;
	ret=fstat(fd,&buf);
	if(ret==-1)
	{
		perror("fstat");
		goto end;
	}
	off_t offset=0;
	recv(sfd,&offset,sizeof(offset),0);
	t.len=sizeof(off_t);
	memcpy(t.buf,&buf.st_size,t.len);
	send_n(sfd,(char*)&t,4+t.len);//文件长度
	lseek(fd,offset,SEEK_SET);
	if((buf.st_size-offset)>1024*1024*100)//大于100M
	{
		off_t m_size=buf.st_size-offset;
		char* p=(char*)mmap(NULL,buf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);//支持断点传输
		if(p==(char*)-1)
		{
			perror("mmap");
			goto end;
		}
		int cnt=m_size/1000;
		int cnt_mod=m_size%1000;
		p=p+offset;//断点传送
		for(int i=0;i<cnt;i++)
		{
			bzero(&t,sizeof(t));
			t.len=1000;
			memcpy(t.buf,p,1000);
			ret=send_n(sfd,(char*)&t,4+t.len);
			if(ret==-1)
			{
				goto end;
			}
			p+=1000;
		}
		bzero(&t,sizeof(t));
		t.len=cnt_mod;
		memcpy(t.buf,p,t.len);
		send_n(sfd,(char*)&t,4+t.len);
		t.len=0;
		send_n(sfd,(char*)&t,4);
		munmap(p,buf.st_size);
	}else
	{
		while((t.len=read(fd,t.buf,sizeof(t.buf)))>0)
		{
			ret=send_n(sfd,(char*)&t,4+t.len);
			if(ret==-1)
			{
				goto end;
			}
		}
		t.len=0;
		send_n(sfd,(char*)&t,4);
	}
	printf("upload ->file successfully\n");
	close(fd);
	return;
end:
	printf("puts failed\n");
	close(fd);
}


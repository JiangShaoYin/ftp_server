#include "ThreadPooltory.h"
char path[200]="/home/leechan/linux/project/server/ftp2/source/upload/";

void tran_file(int fd,char* username) {
	train t;
	int ret,len=0;
	char filename[100]={0};
	ret=recv(fd, &len, 4, 0);
	if(ret==0) {
		printf("66");
		goto end;
	}
	ret=recv(fd,filename,len,0);
	if(ret==0) {
		printf("77");
		goto end;
	}
	strcpy(t.buf, filename);//接收对端传来文件名
	char ok='o',wr='x';
	char path_file[200] = {0};
	sprintf(path_file, "%s%s%s%s", path, username, "/", filename);
	puts(path_file);
	int fd = open(path_file, O_RDWR); // 先打开文件
	if(fd == -1) {
		send(fd, &wr, sizeof(wr), 0);
		perror("open");
		goto end;
	}else{
		send(fd, &ok, sizeof(ok), 0);
	}//没有此文件
	//未结束就发送文件名
	t.len = strlen(t.buf);
	send(fd, &t, 4 + t.len, 0);//发送文件名
	struct stat buf;
	ret = fstat(fd, &buf);
	if(ret == -1)	{
		perror("fstat");
		return;
	}
	off_t offset=0;
	recv(fd, &offset, sizeof(offset), 0);
	t.len = sizeof(off_t);
	memcpy(t.buf, &buthread_pool.st_size, t.len);
	send_n(fd, (char*)&t, 4 + t.len);//文件长度
	lseek(fd, offset, SEEK_SET);
	//printf("offset:%ld\n",offset);//测试
	if ((buthread_pool.st_size - offset) > 1024*1024*100) {//大于100M
		off_t m_size = buthread_pool.st_size - offset;
		char* p = (char*)mmap(NULL, buthread_pool.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);//支持断点传输
		if(p == (char*) - 1) {
			perror("mmap");
			goto end;
		}
		int cnt = m_size /1000;
		int cnt_mod = m_size % 1000;
		p = p + offset;//断点传送
		for (int i = 0; i < cnt; i++) {
			bzero(&t, sizeof(t));
			t.len = 1000;
			memcpy(t.buf, p, 1000);
			ret = send_n(fd, (char*)&t, 4 + t.len);
			if(ret == -1) {
				goto end;
			}
			p += 1000;
		}
		bzero(&t, sizeof(t));
		t.len = cnt_mod;
		memcpy(t.buf, p, t.len);
		send_n(fd, (char*)&t, 4 + t.len);
		t.len = 0;
		send_n(fd, (char*)&t, 4);
		munmap(p,buthread_pool.st_size);
	} else {  // 文件100M
		while((t.len = read(fd, t.buf, sizeof(t.buf))) > 0)  {
			ret = send_n(fd, (char*)&t, 4 + t.len);
			if(ret == -1) {
				goto end;
			}//防止客户端非正常结束
		}
		t.len=0;
		send_n(fd, (char*)&t, 4);
	}
	printf("puts file successfully!\n");
	writeFile("client download success!");
	return;
end:
	printf("puts failed!\n");
	writeFile("client download failed!");
}

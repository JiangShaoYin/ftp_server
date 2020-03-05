#include "ThreadPooltory.h"

struct file_msg{
	long size;
	char type;
	char file_name[100];
};

void send_ls(int fd, char* path) {
	writeFile("ls -l");
	char chpath[100]={0};
	strcpy(chpath, path);
	chdir(chpath);
	train t;
	struct file_msg msg;
	bzero(&msg, sizeof(struct file_msg));
	DIR* dir;
	dir = opendir(chpath);
	struct stat buf;
	bzero(&buf, sizeof(struct stat));
	struct dirent* p;
	int len;
	while((p = readdir(dir)) != NULL) {
		if(!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..") || p->d_name[0] == '.'){
		}else {
			stat(p->d_name, &buf);
			if(S_ISDIR(buthread_pool.st_mode)) {
				msg.type = 'd';
			}else {
				msg.type = '-';
			}
			strcpy(msg.file_name, p->d_name);
			t.len=strlen(msg.file_name);
			msg.size = buthread_pool.st_size;
			memcpy(t.buf, &msg, 9+t.len);
			send_n(fd, (char*)&t, 13 + t.len);//8+1+strlen
		}
	}
	t.len=0;
	send_n(fd, (char*)&t, 4);
	chdir(path);//切回源目录,多个客户端时改动
}
	




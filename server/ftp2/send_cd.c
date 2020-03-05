#include "ThreadPooltory.h"
//cd ..
//cd 相对路径名
void send_wrg(int fd) {
	char wrg[100]= "please enter the right directory";
	send(fd, wrg, sizeof(wrg), 0);
	return;
}

void send_cd(int fd, char* path) { //path为当前目录路径
	DIR* dir;
	dir = opendir(path);
	struct dirent* p;
	char msg[100] = {0};//接收client的cd命令
	int i = 0, count = 0;
	while(path[i] != '\0') {
		if(path[i] == '/')
			count++;
		i++;
	}
	printf("%s\n", path);
	recv(fd, msg, sizeof(msg), 0);
	char log[100]= {0};
	sprintf(log, "%s %s", "cd", msg);
	writeFile(log);//记录cd
	while(count > 2) {
		if(strcmp(msg, "..") == 0) { //保证在home目录下,上一级
			while(path[i] != '/')
				i--;
			path[i] = '\0';
			count--;  //逆序遍历遇到第一个‘/’结束
			printf("%s\n", path);
			bzero(&msg, sizeof(msg));
			strcpy(msg, path);
			send(fd, msg, strlen(msg), 0);
			goto end;
		}else{
			while((p = readdir(dir)) != NULL) {
				if(!strcmp(p->d_name, msg) && p->d_type == 4) {
					sprintf(path, "%s%s%s", path, "/", msg);
					printf("%s\n", path);
					count++;
					bzero(&msg, sizeof(msg));
					strcpy(msg, path);
					send(fd, msg, strlen(msg), 0);
					goto end;
				}
			}
			send_wrg(fd);
			goto end;
		}
	}
	bzero(&msg,sizeof(msg));
	strcpy(msg,"access denied");
	send(fd, msg, strlen(msg), 0);
	return;
end:
	return;
}



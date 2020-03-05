#ifndef __FUNC_H__
#define __FUNC_H__
#include "md5.h"

typedef struct{
	int len;
	char buf[1024];
}train;

void get_file(int,char*);
void get_ls(int);
void get_cd(int,char*);
void get_pwd(int);
void rm_file(int,char*);

int send_n(int,char*,int);
int recv_n(int,char*,int);
void tran_file(int,char*);

int login_client(int socket_fd);


#endif

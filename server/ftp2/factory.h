#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "work_que.h"
//函数指针
typedef void* (*pfunc)(void*);
//线程动作集合结构体
typedef struct{
	pthread_t* pthids;
	pthread_cond_t cond;
	SocketQueue que;
	int pthread_num;
	pfunc thread_func;
	int start_flag;
}fac, *pfac;
//控制发送数据结构体
typedef struct{
	int len;
	char buf[1024];
}train;
//初始化
void factory_init(fac*,pfunc,int);
//启动子线程开始工作
void factory_start(fac*);
//传递fd函数
int send_n(int,char*,int);
int recv_n(int,char*,int);
//处理文件
void tran_file(int,char*);
void get_file(int,char*);
void handle_cmd(int,char*);
void send_ls(int,char*);
void send_cd(int,char*);
void send_pwd(int,char*);
void send_rm(int,char*);

int login_account(int,char*);
int new_account(int,char*);
int insert_mysql(char*,char*,char*);
int query_mysql(int,char*);
int Isuser_mysql(int,char*);

FILE* creat_log(char*);
void writeFile(char*);
int Is_file_md5(char*,char*,char*);
int md5_insert_mysql(char*,char*,char*,char*);
#endif

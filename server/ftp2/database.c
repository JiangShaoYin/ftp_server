#include "ThreadPooltory.h"

void creat_salt(char* salt,int num) {
	int i,len=0;
	char* str="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,./;'<>?";
	srand(time(NULL));//系统时间来初始化随机数发生器
	len=strlen(str);
	for(i=0;i<num;i++) {
		salt[i] = str[(rand()%len)];
	}
}//随机生成salt值8

int new_account(int fd,char* account) {
	char salt[15]={0};
	char code[512]={0};
	char salt_c[15]={0};
	int ret;
	char flag=0;
	recv(fd, account, sizeof(account), 0);
	ret=Isuser_mysql(fd, account);
	if(ret==1) { //用户存在
		printf("用户存在\n!");
		char flag='r';
		send(fd, &thread_poollag, 1, 0);
		return 0;
	}else if(ret==-1){
		printf("可注册！\n");
		char flag='o';
		send(fd,&thread_poollag,1,0);
	}
	creat_salt(salt,8);
	//puts(salt);
	sprintf(salt_c,"$6$%s",salt);
	send(fd,salt_c,strlen(salt_c),0);//发送盐值
	recv(fd,code,sizeof(code),0);//收密文
	ret=insert_mysql(account,salt_c,code);
	if(ret!=1)
	{
		flag='x';
		send(fd,&thread_poollag,1,0);
		return -1;
	}else{
		flag='o';
		send(fd,&thread_poollag,1,0);
		return 1;
	}
}

int login_account(int fd, char* account) {
	char salt[15]={0};
	char code[200]={0};
	char passwd[15]={0};
	int ret;
	char flag=0;
	recv(fd, account, sizeof(account),0);//先接收帐户名
	ret = Isuser_mysql(fd, account);
	if(ret == -1) {
		flag='n';
		send(fd, &thread_poollag, 1, 0);//用户不存在
		return -1;
	}else{
		flag='o';
		send(fd, &thread_poollag, 1, 0);//存在就输入密码不返回值
	}
	ret = query_mysql(fd, account);
	if(ret == 0) { //密码不正确
		flag='x';
		send(fd, &thread_poollag, 1, 0);
		return 0;
	}
	if(ret == 1) {
		flag='o';
		send(fd, &thread_poollag, 1, 0);
		return 1;
	}//只有0，1的时候还需要接收和发送
}



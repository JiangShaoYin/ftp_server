#include"ThreadPooltory.h"

void handle_cmd(int fd, char* path) {
	char username[15] = {0};
	int i = 3;
	int res;
	int flag = -2;
lable:
	recv(fd, (char*)&thread_poollag, sizeof(flag), 0);//0.退出，1.登陆，2.注册
	//printf("flag:%d\n",flag);//测试
	if (flag == 1) { //登陆
		bzero(&username, sizeof(username));
		res = login_account(fd, username); //res=1证明登陆成功
		writeFile("client log in...");
		if(res == 0) {      //密码不正确
			i--;
			goto lable;
			if(i == 0)
				return;    //3次强制退出
		}else if(res == -1) {
			goto lable;    //用户不存在，要注册
		}
	}
	else if (flag == 2) { //注册
		bzero(&username, sizeof(username));
		res = new_account(fd, username);
		writeFile("client redit new account...");
		if(res == 0) {//用户存在
			goto lable;
		} else if(res == -1) {//注册失败
			goto lable;
		} else if(res == 1) { //注册成功
			goto lable;
		}else//其它无效命令
			return;
	}
	else if (flag == 0) { //退出
		writeFile("client exited...");
		return;
	}
	else {
		printf("命令无效！\n");	
		goto lable;
	}



	while(1) {
		int command=-1;
		int ret = recv(fd, &command, 4, 0);
		if(ret==0) {
			return;
		}
		//printf("cmd:%d ret:%d\n",command,ret);
		switch(command) {
			case 0 : send_cd(fd, path); break;//cd命令
			case 1 : send_ls(fd, path); break;//ls命令
			case 2 : get_file(fd, username); break;//client端puts命令
			case 3 : tran_file(fd, username); break;//client端gets命令
			case 4 : send_rm(fd, path); break;//remove命令
			case 5 : send_pwd(fd, path); break;//pwd命令
			case 6 : return;
			default : break;
		}
	}
end:
	writeFile("client exited...");
}

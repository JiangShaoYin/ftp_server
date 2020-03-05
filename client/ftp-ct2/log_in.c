#include "func.h"

int login_client(int socket_fd) {
  char account[15] = {0};
  char salt[15] = {0};
  char buf[200] = {0};
  char *code;
  char *password;
  char flag = 0;
  int i;
  int cnt = 3;
lable:
  printf("请输入选项：0.退出，1.登陆，2.注册\n");
  read(0, buf, sizeof(buf));
  i = atoi(buf);
//	printf("%d\n",i);
exit:
  send(socket_fd, (char *)&i, sizeof(i), 0);
  if (i == 0) {
    printf("即将退出...\n");
    sleep(2);
    return -1;
  } else if (i == 1) {
    printf("请输入用户名：");
    bzero(&buf, sizeof(buf));
    bzero(&account, sizeof(buf));
    fgets(account, 15, stdin);
    send(socket_fd, account, strlen(account) - 1, 0);
    recv(socket_fd, &thread_poollag, 1, 0);
    if (flag == 'n') {  //用户名不存在
      printf("用户名不存在，请注册！(1.注册，0.退出)\n");
    reoption:
      bzero(&buf, sizeof(buf));
      read(0, buf, sizeof(buf));
      i = atoi(buf);
      if (i == 1) {
        goto lable;
      } else if (i == 0) {
        goto exit;
      } else {
        printf("命令无效！\n");
        goto reoption;
      }

    }  //否则输入密码
  login:
    bzero(&password, sizeof(password));
    password = getpass("请输入登陆密码：");
    recv(socket_fd, salt, sizeof(salt), 0);
    // puts(salt);//测试
    code = crypt(password, salt);
    // puts(code);//测试
    send(socket_fd, code, strlen(code), 0);  //收盐值发明文
    recv(socket_fd, &thread_poollag, 1, 0);
    if (flag == 'o') {
      printf("登陆成功\n");
    } else {
      printf("登陆失败，密码不正确\n");
      if (--cnt)
        goto lable;
      else
        return -1;
    }
  } else if (i == 2) {
  redit:
    bzero(&account, sizeof(account));
    printf("请输入要注册的用户名：");
    fgets(account, 15, stdin);
    send(socket_fd, account, strlen(account) - 1, 0);
    recv(socket_fd, &thread_poollag, 1, 0);
    if (flag == 'r') {
      printf("用户名已存在！（2，注册，1.登陆，0.退出)\n");
      bzero(&buf, sizeof(buf));
      read(0, buf, sizeof(buf));
      i = atoi(buf);
      if (i == 2) {
        goto redit;
      } else if (i == 0) {
        goto exit;
      } else if (i == 1) {
        goto lable;
      } else {
        printf("命令无效！\n");
        goto lable;
      }
    }
    bzero(&password, sizeof(password));
    password = getpass("请输入注册密码：");
    bzero(&salt, sizeof(salt));
    bzero(&code, sizeof(code));
    recv(socket_fd, salt, sizeof(salt), 0);
    // puts(salt);//测试
    code = crypt(password, salt);
    // puts(code);//测试
    send(socket_fd, code, strlen(code), 0);
    recv(socket_fd, &thread_poollag, 1, 0);
    if (flag == 'o') {
      printf("注册成功！\n");
      goto lable;
    } else {
      goto lable;
    }
  } else {
    printf("命令无效！\n");
    goto lable;
  }
}

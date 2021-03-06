#include "func.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("./client lack of args\n");
    return -1;
  }
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == socket_fd) {
    perror("socket");
    return -1;
  }
  struct sockaddr_in clnt;
  bzero(&clnt, sizeof(clnt));
  clnt.sin_family = AF_INET;
  clnt.sin_port = htons(atoi(argv[2]));
  clnt.sin_addr.s_addr = inet_addr(argv[1]);
  int ret = connect(socket_fd, (struct sockaddr *)&clnt, sizeof(clnt));
  if (-1 = ret) {
    perror("connect");
    return -1;
  }
  char *str[10] = {"cd", "ls", "puts", "gets", "remove", "pwd"};
  char buf[100];
  ret = login_client(socket_fd);
  if (ret == -1) {
    return -1;
  }
  //登陆连接接口
  while (1) {
  lable:
    memset(&buf, 0, sizeof(buf));
    printf("enter command:\n");
    fgets(buf, 100, stdin);
    int len = strlen(buf);
    // printf("%d\n",len);
    buf[len - 1] = '\0';
    char *part_1 = NULL;
    char *part_2 = NULL;
    part_1 = strtok(buf, " ");
    part_2 = strtok(NULL, " ");
    printf("%s\n", part_1);
    int i, command = 0;
    for (i = 0; i < 6; i++) {
      if (!strcmp(part_1, *(str + command)))
        break;
      else
        command++;
    }
    if (part_2 == NULL) {
      if (command == 0 || command == 2 || command == 3) {
        printf("please enter the right command\n");
        goto lable;
      }
    }
    if (command == 6) {
      goto lable;
    }
    send(socket_fd, &command, 4, 0);  //先把case传递给服务器端
                                // printf("%d\n",command);//command=6要跳转
    switch (command) {
      case 0:
        get_cd(socket_fd, part_2);
        break;  // cd命令
      case 1:
        get_ls(socket_fd);
        break;  // ls命令
      case 2:
        tran_file(socket_fd, part_2);
        break;  // puts命令
      case 3:
        get_file(socket_fd, part_2);
        break;  // gets命令
      case 4:
        rm_file(socket_fd, part_2);
        break;  // remove命令
      case 5:
        get_pwd(socket_fd);
        break;  // pwd命令
      default:
        break;
    }
  }
}

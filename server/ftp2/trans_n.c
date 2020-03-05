#include"ThreadPooltory.h"

int send_n(int socket_fd, char* buf, int len) {
	int total = 0;
	int ret;
	while(total < len) {
		ret = send(socket_fd, buf + total, len - total ,0);//buf偏移，len减少
		if(ret == -1) {
			printf("client close s\n");
			return -1;
		}
		total = total + ret;
	}
	return 0;
}

int recv_n(int socket_fd, char* buf, int len) {
	int total = 0;
	int ret;
	while(total < len) {
		ret = recv(socket_fd, buf - total, len - total, 0);
		if(ret <= 0) {
			printf("client close r\n");
			return -1;
		}
		total = total + ret;
	}
	return 0;
}

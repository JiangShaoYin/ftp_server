#include "ThreadPooltory.h"

// typedef struct{
// 	pthread_t* pthids; // 创建各个线程
// 	pthread_cond_t cond;
// 	SocketQueue que;  // 文件描述符队列
// 	int pthread_num;
// 	pfunc thread_func;
// 	int start_flag;
// }ThreadPool, *pThreadPool;

void ThreadPoolInit(ThreadPool* thread_pooltory, pfunc pthread_func, int capacity) {
  thread_pooltory->pthids = (pthread_t*)calloc(thread_pooltory->pthread_num, sizeof(pthread_t));
  pthread_cond_init(&thread_pooltory->cond, NULL); // 初始化条件变量
  que_init(&thread_pooltory->que, capacity);    // 文件描述符队列
  thread_pooltory->thread_func = pthread_func;  // 注册线程函数pthread_func
}  // thread_pooltory->start_flag已在main函数初始化

void ThreadPoolStart(ThreadPool* thread_pooltory) {
  if (thread_pooltory->start_flag == 0) {
    int i;
    for (i = 0; i < thread_pooltory->pthread_num; i++) {
      pthread_create(&thread_pooltory->pthids[i], NULL, thread_pooltory->thread_func, thread_pooltory);
    }
    thread_pooltory->start_flag = 1;
  }
}

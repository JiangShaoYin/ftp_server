#include"factory.h"

// typedef struct{
// 	pthread_t* pthid;
// 	pthread_cond_t cond;
// 	fd_queue que;
// 	int pthread_num;
// 	pfunc thread_func;
// 	int start_flag;
// }fac, *pfac;

void factory_init(fac* p_factory, pfunc pthread_func, int capacity) {
	p_factory->pthid = (pthread_t*)calloc(p_factory->pthread_num, sizeof(pthread_t));
	pthread_cond_init(&p_factory->cond, NULL);
	que_init(&p_factory->que, capacity);
	p_factory->thread_func = pthread_func;
}//p_factory->start_flag已在main函数初始化

void factory_start(fac* p_factory) {
	if(p_factory->start_flag==0) {
		int i;
		for(i=0; i < p_factory->pthread_num; i++) {
			pthread_create(&p_factory->pthid[i], NULL, p_factory->thread_func, p_factory);
		}
		p_factory->start_flag = 1;
	}
}

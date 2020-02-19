#include"factory.h"

void factory_init(fac* pf,pfunc pthread_func,int capacity)
{
	pf->pthid=(pthread_t*)calloc(pf->pthread_num,sizeof(pthread_t));
	pthread_cond_init(&pf->cond,NULL);
	que_init(&pf->que,capacity);
	pf->thread_func=pthread_func;
}//pf->start_flag已在main函数初始化

void factory_start(fac* pf)
{
	if(pf->start_flag==0)
	{
		int i;
		for(i=0;i<pf->pthread_num;i++)
		{
			pthread_create(&pf->pthid[i],NULL,pf->thread_func,pf);
		}
		pf->start_flag=1;
	}
}

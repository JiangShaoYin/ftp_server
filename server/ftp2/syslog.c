#include "factory.h"

char Path1[50]= "/home/leechan/linux/project/server/ftp2/log/log";

FILE* creat_log(char *Path1)
{
	FILE* fp=fopen(Path1,"a+");
	if(!fp)
	{
		return NULL;
	}
	fseek(fp,1024*1024*2-1,SEEK_SET);
	fwrite(" ",1,1,fp);
	fseek(fp,0,SEEK_SET);
	return fp;
}

void writeFile(char* buf)
{
	FILE* fp=creat_log(Path1);
	char pszTimeStr[1024]={0};
	struct tm tSysTime={0};
	struct timeval tTimeVal={0};
	time_t tCurrentTime={0};

	uint8_t szUsec[20]={0};//微秒
	uint8_t szMsec[20]={0};//毫秒

	tCurrentTime=time(NULL);
	localtime_r(&tCurrentTime,&tSysTime);

	gettimeofday(&tTimeVal,NULL);
	sprintf(szUsec,"%06ld",tTimeVal.tv_usec);
	strncpy(szMsec,szUsec,3);

	sprintf(pszTimeStr,"[%04d.%02d.%02d %02d:%02d:%02d]-> %s\n",
			tSysTime.tm_year+1900,tSysTime.tm_mon+1,tSysTime.tm_mday,
			tSysTime.tm_hour,tSysTime.tm_min,tSysTime.tm_sec,buf);
	fwrite(pszTimeStr,strlen(pszTimeStr),1,fp);
	fflush(fp);
}

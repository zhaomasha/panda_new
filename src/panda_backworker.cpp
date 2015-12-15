/*************************************************************************
 * File Name: panda_backworker.cpp
 * Author: wangxin
 * Mail: wangxin4@iie.ac.cn
 * Created Time: Tue 15 Dec 2015 10:23:26 AM CST
 * Description:
 * 
************************************************************************/

#include<iostream>
using namespace std;

void kill_func(int signum){	
	pthread_cancel(thread_process);
}

void process(void* args)
{
	
}

int main()
{
	context_t ctx(16);
	signal(SIGTERM,kill_func);
	signal(SIGINT,kill_func);
	pthread_create(&thread_process,NULL,process,&ctx);
	pthread_join(thread_process,NULL);
}

#include "panda_util.hpp"
long getTime(){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec*1000+tv.tv_usec/1000;
}

//解析字符串，取出一个集合
void parse_strings(string params,vector<string>& v,string delims){
	char* result;
	char* p=(char*)params.c_str();
	char* ptr=NULL;
	while((result=strtok_r(p,delims.c_str(),&ptr))!=NULL){
		v.push_back(string(result));
		p=NULL;
	}
	
}
//针对系统变量中有些集合形式的参数，取出来，存入集合中
void parse_env(string param_name,vector<string>& v,string delims){
	string params(getenv(param_name.c_str()));
	parse_strings(params,v,delims);
}
//解析字符串，delims为分割符，得到子图和ip的pair对
pair<uint32_t,string> parse_sub_ip(string params,string delims){
	char* ptr=NULL;
	char* key=strtok_r((char*)params.c_str(),delims.c_str(),&ptr);
	char* ip=strtok_r(NULL,delims.c_str(),&ptr);
	return pair<uint32_t,string>(atoi(key),ip);
}
//解析字符串，delims为分割符，得到ip和负载的pair对
pair<string,uint32_t> parse_ip_num(string params,string delims){
	char* ptr=NULL;
	char* ip=strtok_r((char*)params.c_str(),delims.c_str(),&ptr);
	char* num=strtok_r(NULL,delims.c_str(),&ptr);
	return pair<string,uint32_t>(ip,atoi(num));
}
//元数据的路径中取出图的名字
string metapath_key(char *path){
	int begin=strlen(getenv("SERVER_DIR_NAME"))+1;
	int len=0;
	for(int i=begin;path[i]!='.';i++) len++;
	return string (path+begin,len);	
}
//图的路径中取出图的名字
string graph_key(char*path){
	int begin=strlen(getenv("DIR_NAME"))+1;
	return string(path+begin);
}
/*lock_t* Getlock(){
        return (lock_t*)malloc(sizeof(lock_t));
}
void Lock(lock_t *lock){
        pthread_mutex_lock(lock);
}
void Unlock(lock_t *lock){
        pthread_mutex_unlock(lock);
}
void Initlock(lock_t *lock,void *p){
        pthread_mutex_init(lock,(pthread_mutexattr_t*)p);
}
void Destroylock(lock_t *lock){
        pthread_mutex_destroy(lock);
}
//获得锁，返回0，否则返回非0的错误码
int Trylock(lock_t *lock){
        return pthread_mutex_trylock(lock);
}*/
lock_t* Getlock(){
        return (lock_t*)malloc(sizeof(lock_t));
}
void Lock(lock_t *lock){
        while(!__sync_bool_compare_and_swap(lock,1,0)){
		}
}
void Unlock(lock_t *lock){
        *lock=1;
}
void Initlock(lock_t *lock,void *p){
        *lock=1;
}
void Destroylock(lock_t *lock){
        //free(lock);
}
//获得锁，返回0，否则返回非0的错误码
int Trylock(lock_t *lock){ 
        if(__sync_bool_compare_and_swap(lock,1,0)){
			return 0;
		}
		return 1;
}
bool GetCpuMem(float &cpu,size_t &mem, int pid,int tid ) 
{ 
    bool ret = false; 
    char cmdline[100]; 
    sprintf(cmdline, "ps -o %%cpu,rss,%%mem,pid,tid -mp %d", pid); 
    FILE *file; 
    file = popen(cmdline, "r"); 
    if (file == NULL)  
    { 
        printf("file == NULL\n"); 
        return false; 
    } 
    char line[300]; 
    float l_cpuPrec=0; 
    int l_mem=0; 
    float l_memPrec=0; 
    int l_pid=0; 
    int l_tid=0; 
    if (fgets(line, 300, file) != NULL)  
    { 
    //  printf("1st line:%s",line); 
        if (fgets(line, 300, file) != NULL)  
        { 
    //      printf("2nd line:%s",line); 
            sscanf( line, "%f %d %f %d -", &l_cpuPrec, &l_mem, &l_memPrec, &l_pid ); 
            cpu = l_cpuPrec; 
            mem = l_mem; 
            if( tid == -1 ) 
                ret = true; 
            else 
            { 
                while( fgets(line, 300, file) != NULL ) 
                { 
                    sscanf( line, "%f - - - %d", &l_cpuPrec, &l_tid ); 
    //              printf("other line:%s",line); 
    //              cout<<l_cpuPrec<<'\t'<<l_tid<<endl; 
                    if( l_tid == tid ) 
                    { 
                        printf("cpuVal is tid:%d\n",tid); 
                        cpu = l_cpuPrec; 
                        ret = true; 
                        break; 
                    } 
                } 
                if( l_tid != tid ) 
                    printf("TID not exist\n"); 
            } 
        } 
        else 
            printf("PID not exist\n"); 
    } 
    else 
        printf("Command or Parameter wrong\n"); 
    pclose(file); 
    return ret; 
}

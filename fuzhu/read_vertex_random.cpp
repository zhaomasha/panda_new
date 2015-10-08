/*
   随机读取某个范围内的顶点的信息
   参数1是图的名字，参数2是顶点的id
*/
#include <stdlib.h>
#include"panda_client.hpp"
using namespace std;
int main(int argc,char* argv[])
{
    if(argc!=4){
        cout<<"run like : a.out arg1(graph name) arg2(start vertex id)  arg3(end vertex id)"<<endl;
        return 1;
    }
    string graph_name(argv[1]);
    v_type start_id=atoi(argv[2]);
    v_type end_id=atoi(argv[3]);
    uint32_t repeat_num = 100000;
    Client c;
    int res=c.graph_is_in(graph_name);
    if(res){
        c.connect(graph_name);
    }
    else {
        cout<<"graph "<<graph_name<<" has not exsit"<<endl;
        return 1;
    }
    srand(unsigned(time(0)));
	long vt1=getTime();
    uint32_t found_num = 0;
    uint32_t unfound_num = 0;
    Vertex_u v;
    for(int i=0; i<repeat_num; ++i){
        v_type cur_id =start_id + rand()%(end_id - start_id);
        uint32_t num;
        if(c.read_vertex(cur_id,v,&num)==0){
            //cout<<"vertex id:"<<v.id<<endl;
            //cout<<"vertex nick_name:"<<v.nick_name<<endl;
            //cout<<"edge number of vertex:"<<num<<endl;    
            ++found_num;
        }else{
            //cout<<"vertex "<<id<<" has not exsit"<<endl;
            ++unfound_num;
        }
    }
	long vt2=getTime();
    cout << "Speed of query of vertex: " << (vt2-vt1) << "ms for "<< repeat_num << " queries."<< endl;
}

/*
   读取某个顶点的信息
   参数1是图的名字，参数2是顶点的id
*/
#include"panda_client.hpp"
using namespace std;
int main(int argc,char* argv[])
{
    if(argc!=3){
        cout<<"run like : a.out arg1(graph name) arg2(vertex id)"<<endl;
        return 1;
    }
    string graph_name(argv[1]);
    v_type id=atoi(argv[2]);
    Client c;
    int res=c.graph_is_in(graph_name);
    if(res){
        c.connect(graph_name);
    }
    else {
        cout<<"graph "<<graph_name<<" has not exsit"<<endl;
        return 1;
    }
    Vertex_u v;
    uint32_t num;
    if(c.read_vertex(id,v,&num)==0){
        cout<<"vertex id:"<<v.id<<endl;
        cout<<"vertex nick_name:"<<v.nick_name<<endl;
        cout<<"edge number of vertex:"<<num<<endl;    
    }else
        cout<<"vertex "<<id<<" has not exsit"<<endl;
}

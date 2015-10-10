/*
   读取某个图所有的顶点
   参数1是图的名字
*/
#include"panda_client.hpp"
using namespace std;
int main(int argc,char* argv[])
{
    if(argc!=2){
        cout<<"run like : a.out arg1(graph name) "<<endl;
        return 1;
    }
    string graph_name(argv[1]);
    Client c;
    int res=c.graph_is_in(graph_name);
    if(res){
        c.connect(graph_name);
    }
    else {
        cout<<"graph "<<graph_name<<" has not exsit"<<endl;
        return 1;
    }
    list<Vertex_u>* vertexes;
    uint32_t size;
    c.read_all_vertex(&vertexes,&size);
    uint32_t all_num=0;
    for(int i=0;i<size;i++){
        all_num+=vertexes[i].size();
		list<Vertex_u>::iterator it;
		for(it=vertexes[i].begin();it!=vertexes[i].end();++it){
			cout<<it->id<<" "<<it->nick_name<<" "<<it->edge_num<<endl;
		}
    }
    cout<<"graph "<<graph_name<<"'s vertex number:"<<all_num<<endl;
}

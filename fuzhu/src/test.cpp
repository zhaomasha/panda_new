/*
  把文件中的数据存储到panda系统中
  参数1是图的名字，参数2是每次分批操作的数量
*/
#include"panda_client.hpp"
using namespace std;
int main(int argc,char* argv[])
{
    
    Client c;
	string graph_name="zms";
    int res=c.graph_is_in(graph_name);
    if(res){
        c.connect(graph_name);
    }
    else {
        c.create_graph(graph_name); 
        c.connect(graph_name);
    }
    Vertex_u v(2,"zms");
    cout<<c.add_vertex(v);
    Edge_u edge(3,3,"asasa",1,2112121);
    cout<<c.add_edge(edge);
}

/*
   读取某个图所有的顶点
   参数1是图的名字
*/
#include"panda_client.hpp"
using namespace std;
int main(int argc,char* argv[])
{
    if(argc!=3){
        cout<<"run like : a.out arg1(graph name) arg2(verbose)"<<endl;
        return 1;
    }
    string graph_name(argv[1]);
    int verbose = atoi(argv[2]);
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
	long vt1=getTime();
    c.read_all_vertex(&vertexes,&size);
	long vt2=getTime();
    cout << "Speed of query all vertex of "<<graph_name << ": "<< (vt2-vt1) << " ms for ";
    uint32_t all_num=0;
    for(int i=0;i<size;i++){
        all_num+=vertexes[i].size();
		/*list<Vertex_u>::iterator it;
		for(it=vertexes[i].begin();it!=vertexes[i].end();++it){
			cout<<it->id<<" "<<it->nick_name<<" "<<it->edge_num<<endl;
		}*/
        list<Vertex_u>::iterator it;
        for(it=vertexes[i].begin();it!=vertexes[i].end();++it){
            if(verbose){
                cout<<it->id<<" nick_name"<<it->nick_name<<" edge_num:"<<it->edge_num<<endl;
            }
        }
    }
    cout<< all_num << " queries."<< endl;
}

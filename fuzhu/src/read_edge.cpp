/*
   读取某个顶点的所有边或者两个顶点之间的所有边
   参数1是图的名字，参数2是顶点id（参数3是顶点的id）
*/
#include"panda_client.hpp"
using namespace std;
int main(int argc,char* argv[])
{
    if((argc!=3)&&(argc!=4)){
        cout<<"run like : a.out arg1(graph name) arg2(vertex id)"<<endl;
        cout<<"           a.out arg1(graph name) arg2(vertex id1) arg3(vertex id2)"<<endl;
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
    list<Edge_u> es;
    if(argc==3){
        v_type id=atoi(argv[2]);
        c.read_edges(id,es);
    }
    if(argc==4){
        v_type s_id=atoi(argv[2]);
        v_type d_id=atoi(argv[3]);
        c.read_edge(s_id,d_id,es);
    }
    if(es.size()==0) cout<<"thers is no edge"<<endl;
    else{
        int i=0;
        for(list<Edge_u>::iterator it=es.begin();it!=es.end();it++){
             i++;
             cout<<"edge "<<i<<":"<<(*it).s_id<<" "<<(*it).d_id<<" "<<(*it).blog_id<<" "<<(*it).type<<" "<<ctime(&(*it).timestamp)<<endl;
        }
    }
    
}

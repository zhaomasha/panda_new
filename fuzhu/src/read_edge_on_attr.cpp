/*
   根据属性读取所有边
   参数1是图的名字，参数2是属性值
*/
#include"panda_client.hpp"
using namespace std;
int main(int argc,char* argv[])
{
    if(argc!=3){
        cout<<"run like : a.out arg1(graph name) arg2(attribute value of edge)"<<endl;
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
    list<Edge_u> *es;
    uint32_t num,edge_num=0;
    string blog_id(argv[2]);
    c.read_edge_index_pthread(blog_id,&es,&num);
    for(int i=0;i<num;i++){
        edge_num+=es[i].size();
    }
    if(edge_num==0) cout<<"thers is no edge"<<endl;
    else{
        int j=0;
        for(int i=0;i<num;i++){
             for(list<Edge_u>::iterator it=es[i].begin();it!=es[i].end();it++){
                  j++;
                  cout<<"edge "<<j<<":"<<(*it).s_id<<" "<<(*it).d_id<<" "<<(*it).blog_id<<" "<<(*it).type<<" "<<ctime(&(*it).timestamp)<<endl;
             }
        }  
    }
    
}

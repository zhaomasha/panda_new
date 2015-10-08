/*
  把文件中的数据存储到panda系统中
  参数1是图的名字，参数2是每次分批操作的数量
*/
#include"panda_client.hpp"
using namespace std;
int main(int argc,char* argv[])
{
    if(argc!=4){
        cout<<"run like :a.out arg1(dir name,with '/') arg2(graph name) arg3(numbers of every operation)"<<endl;
        return 1;
    }
    ifstream fin,fin1;
    string graph_name(argv[2]);
    int pack_num=atoi(argv[3]);
    string dir(argv[1]);
    string vertex_file(dir+graph_name+".vertex");
    string edge_file(dir+graph_name+".edge");
    Client c;
    int res=c.graph_is_in(graph_name);
    if(res){
		cout<<"graph has exist"<<endl;
        c.connect(graph_name);
    }
    else {
		cout<<"graph has not exist"<<endl;
        c.create_graph(graph_name); 
        c.connect(graph_name);
    }
    //存储顶点
    fin.open(vertex_file.c_str());
    if(!fin){
         cout<<"vertex file missing!"<<endl;
         return 1;
    }
    string data;
    list<Vertex_u> vs;
    uint32_t ok_all_num=0;//成功插入的顶点总数目
    uint32_t ok_pack_num;//每次批量插入成功的顶点数目
    int r=0;//记录一次pack
    while(getline(fin,data)){
         v_type id=atol(data.c_str());
         getline(fin,data);
         string nickname=data;
         Vertex_u v(id,nickname,30);
         vs.push_back(v);
         r++;
         if(r==pack_num){
              c.add_vertexes(vs,&ok_pack_num);
              vs.clear();
              r=0;
              ok_all_num+=ok_pack_num;
              cout<<"finish vertex "<<ok_all_num<<endl;
         }
    }
    if(r!=0){
         c.add_vertexes(vs,&ok_pack_num);
         vs.clear();
         r=0;
         ok_all_num+=ok_pack_num;
    }
    cout<<"success insert vertex:"<<ok_all_num<<endl;
    ok_all_num=0;
    list<Edge_u> es;
    fin.close();
    //存储边
    fin1.open(edge_file.c_str());
    if(!fin1){
         cout<<"edge file missing!"<<endl;
         return 1;
    }
    while(getline(fin1,data)){  
         v_type s_id=atol(data.c_str()); 
         getline(fin1,data);
         v_type d_id=atol(data.c_str());
         getline(fin1,data);
         string blog_id=data; 
         getline(fin1,data);
         int type=atol(data.c_str());
         getline(fin1,data);
         t_type timestamp=atol(data.c_str());
         Edge_u edge(s_id,d_id,blog_id,type,timestamp);
         es.push_back(edge);
         r++;
         if(r==pack_num){
              c.add_edges_pthread(es,&ok_pack_num);
              es.clear();
              r=0;
              ok_all_num+=ok_pack_num;
              cout<<"finish edge "<<ok_all_num<<endl;
         }
    } 
    if(r!=0){
         c.add_edges(es,&ok_pack_num);
         es.clear();
         r=0;
         ok_all_num+=ok_pack_num;
    } 
    cout<<"success insert edge:"<<ok_all_num<<endl;
    fin1.close(); 
}

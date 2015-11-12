/*
	创建arg1到arg2的顶点
*/
#include"panda_client.hpp"
using namespace std;
long getTime(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*1000+tv.tv_usec/1000;
}
int main(int argc,char* argv[])
{
    if(argc!=4){
        cout<<"run like :a.out arg1(start vertex num) arg2(end vertex num) arg3(numbers of every operation)"<<endl;
        return 1;
    }
    int pack_num=atoi(argv[3]);
    int start_num=atoi(argv[1]);
    int end_num=atoi(argv[2]);
	string graph_name="test";
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
    string data;
    list<Vertex_u> *vss=new list<Vertex_u>();
	list<Vertex_u> &vs=*vss;
    uint32_t ok_all_num=0;//成功插入的顶点总数目
    uint32_t ok_pack_num=0;//每次批量插入成功的顶点数目
    int r=0;//记录一次pack
	long vt1=getTime();
	int v_num=start_num;
    while(v_num<=end_num){
         v_type id=v_num;
         string nickname="funny";
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
		 v_num++;
    }
    if(r!=0){
         c.add_vertexes(vs,&ok_pack_num);
         vs.clear();
         r=0;
         ok_all_num+=ok_pack_num;
    }
	long vt2=getTime();
    cout<<"success insert vertex:"<<ok_all_num<<" time:"<<(vt2-vt1)/1000<<" 性能:"<<ok_all_num/((vt2-vt1)/1000)<<endl;
}

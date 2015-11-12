/*
	对顶点arg1到顶点arg2中的每一个顶点，创建三十条边
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
    if(argc!=5){
        cout<<"run like :a.out arg1(start_vertex_num) arg2(end_vertex_num) arg3(numbers of every operation) arg4(edge num)"<<endl;
        return 1;
    }
    ifstream fin,fin1;
    int pack_num=atoi(argv[3]);
	int start_num=atoi(argv[1]);
    int end_num=atoi(argv[2]);
    int edge_num=atoi(argv[4]);
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
	uint32_t ok_all_num=0;
    uint32_t ok_pack_num=0;
    list<Edge_u> es;
    //存储边
	int r=0;
	long et1=getTime();
	int v_num=start_num;
    while(v_num<=end_num){  
         v_type s_id=v_num; 
		 for(int ii=1;ii<=edge_num;ii++){
         	v_type d_id=s_id+ii;
         	string blog_id="123456"; 
         	int type=1;
         	t_type timestamp=134507764;
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
		v_num++;
    } 
    if(r!=0){
         c.add_edges(es,&ok_pack_num);
         es.clear();
         r=0;
         ok_all_num+=ok_pack_num;
    } 
	long et2=getTime();
    cout<<"success insert edge:"<<ok_all_num<<" time:"<<(et2-et1)/1000<<" 性能:"<<ok_all_num/((et2-et1)/1000)<<endl;
}

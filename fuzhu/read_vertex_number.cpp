/*
   读取某个图所有的顶点数目，以及边的数目
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
    uint32_t*nums;
    uint32_t size;
    c.get_vertex_num_pthread(&nums,&size);
    uint32_t all_num=0;
    for(int i=0;i<size;i++){
        all_num+=nums[i];
    }
	uint32_t*nums1;
    uint32_t size1;
    c.get_edge_num_pthread(&nums1,&size1);
    uint32_t all_num1=0;
    for(int i=0;i<size1;i++){
		cout<<"~~ "<<nums1[i]<<endl;
        all_num1+=nums1[i];
    }
    cout<<"graph "<<graph_name<<"'s vertex number:"<<all_num<<"   edge number:"<<all_num1<<endl;
}

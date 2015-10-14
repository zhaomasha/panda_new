/*************************************************************************
 > File Name: read_edge_random.cpp
 > Author: wangxin
 > Mail: wangxin4@iie.ac.cn
 > Created Time: Wed 14 Oct 2015 11:14:05 AM CST
 > Description:
 ************************************************************************/

#include<iostream>
#include <stdlib.h>

#include"panda_client.hpp"

using namespace std;
int main(int argc,char* argv[])
{
    if(argc!=6){
        cout<<"error arg number"<<argc<<endl;
        cout<<"run like : a.out arg1(graph name) arg2(start vertex id) arg3(end vertex id) arg4(verbose) arg5(repeat_num)"<<endl;
        return 1;
    }
    string graph_name(argv[1]);
    v_type start_id=atoi(argv[2]);
    v_type end_id=atoi(argv[3]);
    int verbose=atoi(argv[4]);
    uint32_t repeat_num = atoi(argv[5]);


    Client c;
    int res=c.graph_is_in(graph_name);
    if(res){
        c.connect(graph_name);
    }
    else {
        cout<<"graph "<<graph_name<<" has not exsit"<<endl;
        return 1;
    }

    srand(unsigned(time(0)));
	long vt1=getTime();

    for(int i=0; i<repeat_num; ++i){
        list<Edge_u> es;
        v_type s_id =start_id + rand()%(end_id - start_id);
        v_type d_id =s_id + rand()%(35);
        c.read_edge(s_id,d_id,es);
        if(es.size()==0){
            if(verbose){
                cout<<s_id<<"->"<<d_id<<":edge not exist"<<endl;
            }
        }else{
            int j=0;
            for(list<Edge_u>::iterator it=es.begin();it!=es.end();it++){
                j++;
                if(verbose){
                    cout<<s_id<<"->"<<d_id<<" ==> ";
                    cout<<"num :"<<j<< " edge:"<<(*it).s_id<<"->"<<(*it).d_id;
                    cout<<" blog_id:"<<(*it).blog_id<<" type:"<<(*it).type;
                    cout<<" timestamp:"<<ctime(&(*it).timestamp);
                 }
            }
        }
    }
	long vt2=getTime();
    cout << "Speed of query of edge: " << (vt2-vt1) << "ms for "<< repeat_num << " queries."<< endl;
    
}

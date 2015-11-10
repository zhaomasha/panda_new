/*************************************************************************
 * File Name: panda_status.cpp
 * Author: wangxin
 * Mail: wangxin4@iie.ac.cn
 * Created Time: 2015年11月09日 星期一 16时00分06秒
 * Description:
 * 
************************************************************************/

#include<iostream>
#include "panda_status.h"

typedef std::vector<std::string>::iterator string_vit;
typedef std::vector<int>::iterator int_vit;

PandaStatus::PandaStatus(std::vector<std::string> slave_hosts, int _alive_threshold ):alive_threshold(_)
{
	for(string_vit host=slave_hosts.begin(); host!=slave_hosts.end(); ++host){
		unconnected_slaves.insert(*host)
	}
}


int PandaStatus::add_slave(std::string host_ip, unsigned int * serial_num)
{
	*serial_num = get_slave_serial();
	slave_hosts[host_ip] = *serial_num;
	unconnected_slaves.erase(host_ip);
}


int PandaStatus::check_alive(std::vector<std::string>& lost_hosts)
{
	unsigned int data_size=slave_alive_info.size();
	for(unsigned int serial_num=0; serial_num<data_size; ++serial_num){
		if(slave_alive_info[serial_num] > alive_threshold){
			slave_alive_info[serial_num] = -1;
			host = slave_hosts[serial_num];
			lost_hosts.push_back(host);
			slave_hosts.erase(serial_num);
		}
	}
	if( unconnected_slaves.size() != 0 ){
		for(std::set<std::string>::iterator it = unconnected_slaves.begin()
				it!= unconnected_slaves.end(); ++it){
			std::cout << "warning: haven't connected to slave host : " << *it << endl;
		}
	}
	return 0;
}


void PandaStatus::slave_time_lapse(unsigned int t)
{
	if( t<=0 )
		return;
	for(int_vit it=slave_alive_info.begin(); it!=slave_alive_info.end(); ++it){
		if(*it<0)
			continue;
		*it += t;
	}
}


unsigned int PandaStatus::get_slave_serial()
{
	unsigned serial_num = 0;
	for(;serial_num<slave_alive_info.size(); ++serial_num){
		if(slave_alive_info[serial_num]<0){
			slave_alive_info[serial_num] = 0;
			return serial_num;
		}
	}
	slave_alive_info.append(0);
	return serial_num;
}

void* keep_status_slave(void *)
{
	int period=atoi(getenv("PERIOD_PANDASLAVE"));
	int slave_serial = -1;
	master_ip = getenv("MASTER_IP");
	master_port = getenv("MASTER_PORT");
	local_ip = getenv("LOCAL_IP");
	try{
		socket_t* s=new socket_t(*ctx,ZMQ_REQ);
		string endpoint="tcp://"+master_ip+":"+master_port;
		s->connect(endpoint.c_str());
		while(1){
			sleep(period);
			Requester req_master(*s);
			if( slave_serial<0 ){
				int slave_serial;
			}else{
				
			}
			proto_graph_vertex_u mes_slave(graph_name,v);
			req_slave.ask(CMD_ADD_VERTEX,&mes_slave,sizeof(proto_graph_vertex_u));
			req_slave.parse_ans();
			return req_slave.get_status();
		}
	}catch(zmq::error_t& err){
		cout<<"thread " << pthread_self() << "error: "err.what();
	}

}
void* keep_status_master(void*)
{

}

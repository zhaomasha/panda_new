/*************************************************************************
 * File Name: panda_status.cpp
 * Author: wangxin
 * Mail: wangxin4@iie.ac.cn
 * Created Time: 2015???1???9???星期一 16???0???6???
 * Description:
 * 
************************************************************************/

#include<iostream>
#include <time.h>

#include "panda_zmq.hpp"
#include "panda_zmqproc.hpp"
#include "panda_status.h"

typedef std::vector<std::string>::iterator string_vit;
typedef std::vector<int>::iterator int_vit;

PandaStatus::PandaStatus(std::vector<std::string> slave_hosts, int _alive_threshold ):alive_threshold(_alive_threshold)
{
	for(string_vit host=slave_hosts.begin(); host!=slave_hosts.end(); ++host){
		unconnected_slaves.insert(*host);
	}
}


int PandaStatus::add_slave(std::string host_ip, unsigned int * serial_num)
{
	*serial_num = get_slave_serial();
	slave_hosts[*serial_num] = host_ip;
	unconnected_slaves.erase(host_ip);
	return 0;
}


int PandaStatus::check_alive(std::vector<std::string>& lost_hosts)
{
	unsigned int data_size=slave_alive_info.size();
	for(unsigned int serial_num=0; serial_num<data_size; ++serial_num){
		if(slave_alive_info[serial_num] > alive_threshold){
			slave_alive_info[serial_num] = -1;
			std::string host = slave_hosts[serial_num];
			lost_hosts.push_back(host);
			slave_hosts.erase(serial_num);
		}
	}
	if( unconnected_slaves.size() != 0 ){
		for(std::set<std::string>::iterator it = unconnected_slaves.begin();
				it!= unconnected_slaves.end(); ++it){
			std::cout << "warning: haven't connected to slave host : " << *it << std::endl;
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

int PandaStatus::update_slave(unsigned int serial_num)
{
	if(serial_num >= slave_alive_info.size()||slave_alive_info[serial_num]<0)
		return -1;//wrong serial number
	slave_alive_info[serial_num] = 0;
	return 0;
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
	slave_alive_info.push_back(0);
	return serial_num;
}

void request_add_slave(Requester& req_master, int & slave_serial)
{
	std::string local_ip = getenv("LOCAL_IP");
	proto_new_slave msg_new_slave(local_ip);
	req_master.ask(CMD_ADD_SLAVE,&msg_new_slave,sizeof(proto_new_slave));     
	req_master.parse_ans();
	int ret = req_master.get_status();
	if( ret!= STATUS_OK){
		std::cout << "error in adding slave,error code:"<<ret << std::endl;   
	}
	slave_serial = *(int*)req_master.get_data();
}

void request_keep_status(Requester& req_master, int slave_serial)
{

	proto_slave_status msg_slave_status(slave_serial);                        
	req_master.ask(CMD_KEEP_SLAVE_STATUS,&msg_slave_status,sizeof(proto_slave_status));
	req_master.parse_ans();
	int ret = req_master.get_status();
	if( ret != STATUS_OK ){
		std::cout << "error in keeping status,error code:"<<ret << std::endl; 
	}                                                                         
}

void* keep_status_slave(void*args)                                                           
{ 
	std::string master_ip = getenv("MASTER_IP");
	std::string status_master_port = getenv("STATUS_MASTER_PORT");  
	uint32_t period = atoi(getenv("SLAVE_PERIOD"));
	uint32_t con_timeout = atoi(getenv("SLAVE_TIMEOUT"));
    try{
		context_t& ctx=*(context_t*)args;
        socket_t s(ctx,ZMQ_REQ);
		std::string endpoint="tcp://"+master_ip+":"+ status_master_port;
        s.connect(endpoint.c_str());
		int slave_serial = -1;
		time_t start_time = time(0);
        while(1){
            sleep(period);
            Requester req_master(s);
            if( slave_serial<0 ){
				request_add_slave(req_master, slave_serial);
				time_t past_time = time(0) - start_time;
				if(past_time > con_timeout){
					std::cout<<"thread " << pthread_self() << ": cannot connected to master"<<std::endl;
				}
				std::cout<<"thread " << pthread_self() << " action: add to master ,get serial number:"<< slave_serial <<std::endl;
            }else{
				request_keep_status(req_master, slave_serial);
				std::cout<<"thread " << pthread_self() << " action: keep status to master "<<std::endl;
            }
        }
    }catch(zmq::error_t& err){
        std::cout<<"thread " << pthread_self() << " error: " << err.what()<<std::endl;
    }
}

void handle_add_slave(Replier &rep,PandaStatus* panda_status)
{
    proto_new_slave *msg_new_slave = (proto_new_slave*)rep.get_arg();
	unsigned int serial_num;
	int ret = panda_status->add_slave(msg_new_slave->ip, &serial_num);
	if( ret != 0 ){
		rep.ans(STATUS_REJECT_SLAVE,"slave alread exist",strlen("slave alread exist")+1);
	}else{
		rep.ans(STATUS_OK,&serial_num, sizeof(serial_num));
	}
}

void handle_keep_slave_status(Replier & rep,PandaStatus* panda_status)
{
	proto_slave_status *msg_slave_status = (proto_slave_status*)rep.get_arg();
	unsigned int serial_num = msg_slave_status->slave_serial;
	int ret = panda_status->update_slave(serial_num);
	if(0 == ret){
		rep.ans(STATUS_OK,"ok",strlen("ok")+1);
	}else{
		rep.ans(STATUS_INVALID_SLAVE,"invalid slave",strlen("invalid slave")+1);
	}
}

void send_redistribute(Requester& req_back, std::vector<RedistributeTerm> redistribute_info)
{
	for(std::vector<RedistributeTerm>::iterator it = redistribute_info.begin();
			it != redistribute_info.end(); ++it){
		proto_redistribute msg_redistribute(it->graph_name,it->src_slave,it->dst_slave,it->subgraph_id);
		req_back.ask(CMD_LOST_SLAVES, &msg_redistribute, sizeof(proto_redistribute));
		req_back.parse_ans();
		int ret = req_back.get_status();
		if( ret != STATUS_OK ){
			std::cout << "error in informing back controller lost slave info,error code:"<<ret << std::endl; 
		}
	}
	char* content = "redistribute info end";
	req_back.ask(CMD_LOST_SLAVES_END,content, strlen(content)+1);
}

/*
 * check all slaves' status, find the lost ones and inform back_controller
 */
void check_invalid_slaves(PandaStatus* panda_status, contex_t& ctx)
{
	std::vector<std::string> lost_slaves;
	panda_status->check_alive(lost_slaves);
	if(lost_slaves.size()>0){
		for(uint32_t i=0; i<lost_slaves.size(); ++i){
			std::cout << "Warning: Detected slave lost:" << lost_slaves[i] << std::endl;
		}
		std::vector<std::string> redistribute_info;
		bal.redistribute(lost_slaves, redistribute_info);
        socket_t s(ctx,ZMQ_REQ);
		std::string endpoint="tcp://"+back_ip+":"+ back_port;
        s.connect(endpoint.c_str());
        Requester req_back(s);
		send_redistribute(req_back, redistribute_info);
	}
}

void* keep_status_master(void* args)
{                                                                                         
	status_param_t *param = (status_param_t*)args;
	PandaStatus* panda_status = param->pstatus;
	context_t* ctx = param->pctx;
	uint32_t period = atoi(getenv("MASTER_PERIOD"));
    try{                                                                                  
        socket_t s(*ctx,ZMQ_REP);
		std::string master_ip = getenv("MASTER_IP");
		std::string status_master_port = getenv("STATUS_MASTER_PORT");
		std::string endpoint="tcp://"+master_ip+":"+status_master_port;
		int recv_timeout = 1000;
		s.setsockopt(ZMQ_RCVTIMEO, &recv_timeout, sizeof(int));
		s.bind(endpoint.c_str());
		time_t start_time = time(0);
		time_t past_time = 0;
		time_t click_time = time(0);
		while(1){
			Replier rep(s);
			rep.parse_ask();
			int cmd_id = rep.get_cmd();
			if( cmd_id >=0 )
				std::cout<< "operation " << cmd_id << ":" << cmd_name[cmd_id] << std::endl;
			time_t cur_time = time(0);
			panda_status->slave_time_lapse(cur_time-click_time);
			click_time = cur_time;
			switch(cmd_id){
				case CMD_ADD_SLAVE:
					handle_add_slave(rep, panda_status);
					break;
				case CMD_KEEP_SLAVE_STATUS:
					handle_keep_slave_status(rep, panda_status);
					break;
			}
			cur_time = time(0);
			past_time = cur_time - start_time;
			if( past_time > period){
				check_invalid_slaves(panda_status);
				start_time = cur_time;
			}
		}
		
    }catch(zmq::error_t& err){                                                            
        std::cout<<"thread " << pthread_self() << " error: "<<err.what()<<std::endl;         
    }                                                                                     
}                        

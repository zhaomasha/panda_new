/*************************************************************************
 * File Name: panda_backcontroller.cpp
 * Author: wangxin
 * Mail: wangxin4@iie.ac.cn
 * Created Time: Thu 10 Dec 2015 01:54:03 PM CST
 * Description:
 * 
************************************************************************/

#include<iostream>
using namespace std;
pthread_t thread_process;

void kill_func(int signum){	
	pthread_cancel(thread_process);
}

int inform_one_slave(string& slave_info, uint32_t back_worker_port)
{
	socket_t s(ctx,ZMQ_REQ);
	int send_timeout = 5000;
	s.setsockopt(ZMQ_SNDTIMEO, &send_timeout, sizeof(int));
	std::string endpoint="tcp://"+back_ip+":"+ back_port;
	s.connect(endpoint.c_str());
	Requester req_worker(s);
	bool success = req_worker.ask(CMD_LOST_SLAVES, &msg_redistribute, sizeof(proto_redistribute));
	if (!success)
		return -1;
	req_worker.parse_ans();
	s.disconnect(endpoint.c_str());
	s.close()
	return 0;	
}
/*
 *	 inform slaves checkpoint to back up data
 *		
 */
int inform_checkpoint()
{
	BackControllerMeta * back_meta = BackControllerMeta::get_instance();
	vector<string> slave_info = back_meta.get_slave_infos();
	int32_t back_worker_port = back_meta.get_worker_port();
	int ret = 0;
	int flag = 0;
	for(vector<string>::iterator it = slave_info.begin();
			it != slave_info.end(); ++it){
		ret = inform_one_slave(*it, back_worker_port);
		if( ret ! = 0){
			flag = -1;
			cout << cur_time_str() << " [WARNING] check point cannot connect to slave " << *it << endl;
		}
	}
	return ret;
}

void handle_lost_slaves(Replier& rep, vector<RedistributeTerm>& redistri_info)
{
	proto_redistribute *distri_msg = (proto_redistribute*)rep.get_arg();
	RedistributeTerm term;
	term.graph_name = distri_msg->graph_name;
	term.src_slave = distri_msg->src_slave;
	term.dst_slave = distri_msg->dst_slave;
	term.subgraph_id = distri_msg->subgraph_id;
	redistri_info.push_back(term);
	rep.ans(STATUS_OK,"ok",strlen("ok")+1);
}

void send_redistribute_info(map<string, vector<RedistributeTerm>>& redistri_map)
{
	for(map<string,vector<RedistributeTerm>>::iterator p_redistri_info = redistri_map.begin();
			p_redistri_info != redistri_map.end(); ++p_redistri_info){
		//for each alive slave
		std::string back_ip = p_redistri_info->first;		
		BackControllerMeta* back_meta = BackControllerMeta::get_instance();
		//config socket
		uint32_t back_port = back_meta->get_worker_port();
		std::string endpoint="tcp://"+back_ip+":"+ back_port;
		std::cout << "backip:" << back_ip << "back_port:" << back_port << std::endl;
		socket_t s(ctx,ZMQ_REQ);
		s.connect(endpoint.c_str());
		Requester req_back(s);
		vector<RedistributeTerm>& redistri_info = p_redistri_info->second;
		for(std::vector<RedistributeTerm>::iterator it = redistri_info.begin();
				it != redistri_info.end(); ++it){
			//send every RedistributeTerm info
			proto_redistribute msg_redistribute(it->graph_name,it->src_slave,it->dst_slave,it->subgraph_id);
			req_back.ask(CMD_LOST_SLAVES, &msg_redistribute, sizeof(proto_redistribute));
			req_back.parse_ans();
			int ret = req_back.get_status();
			if( ret != STATUS_OK ){
				std::cout << "error in informing back controller lost slave info,error code:"<<ret << std::endl; 
			}
		}
		const char* content = "redistribute info end";
		req_back.ask(CMD_LOST_SLAVES_END,(void*)content, strlen(content)+1);
		s.disconnect(endpoint.c_str());
		s.close()
	}
}

void handle_lost_slaves_end(Replier& rep, vector<RedistributeTerm>& redistri_info)
{
	rep.get_arg();
	map<string, vector<RedistributeTerm> > redistri_map;
	BackControllerMeta* back_meta = BackControllerMeta::get_instance();
	for(std::vector<RedistributeTerm>::iterator it = redistri_info.begin();
			it != redistri_info.end(); ++it){
		redistri_map[it->dst_slave].push_back(*it);
		back_meta->remove(it->src_slave);
	}
	send_redistribute_info(redistri_map);
	rep.ans(STATUS_OK,"ok",strlen("ok")+1);
}

void handle_recovery_end()
{
	string ip = (char*)rep.get_arg();
	static int cnt = 0;
	cout << cur_time_str() << " [INFO] slave "<<ip<< " recovery complete";
	rep.ans(STATUS_OK,"ok",strlen("ok")+1);
	BackControllerMeta * back_meta = BackControllerMeta::get_instance();
	if(cnt == back_meta->get_slave_cnt()){
		cnt = 0;
		cout << cur_time_str() << " [INFO] all recovery complete"
	}
}

void process(void* args)
{
	context_t* ctx = (context_t*)args;
	BackControllerMeta * back_meta = BackControllerMeta::get_instance();

	//config back controller
	string master_ip = back_meta.get_ip_master();
	int32_t back_ctl_port = back_meta.get_ctl_port();
	std::string endpoint="tcp://"+master_ip+":"+back_ctl_port;
	int32_t back_cycle = back_meta.get_back_cycle();

	vector<RedistributeTerm> redistri_info;
	try{
        socket_t s(*ctx,ZMQ_REP);
		int recv_timeout = 1000;
		s.setsockopt(ZMQ_RCVTIMEO, &recv_timeout, sizeof(int));
		s.bind(endpoint.c_str());
		time_t start_time = time(0);
		while(1){
			Replier rep(s);
			rep.parse_ask();
			int cmd_id = rep.get_cmd();
			if( cmd_id >=0 )
				std::cout<<cur_time_str()<< " [INFO] back_ctl thread, operation " << cmd_id << ":" << cmd_name[cmd_id] << std::endl;
			switch(cmd_id){
			case CMD_LOST_SLAVES:
				handle_lost_slaves(rep, redistri_info);
				break;
			case CMD_LOST_SLAVES_END:
				handle_lost_slaves_end(rep, redistri_info);
				break;
			case CMD_RECOVERY_END:
				handle_recovery_end();
				break;
			}
			time_t cur_time = time(0);
			if(cur_time - start_time > back_cycle){
				inform_checkpoint();
				start_time = cur_time;
			}

		}
    }catch(zmq::error_t& err){                                                            
        std::cout<<cur_time_str()<<" [ERROR] status thread error: "<<err.what()<<std::endl;
    }                                                                                     
}

int main()
{
	context_t ctx(16);
	signal(SIGTERM,kill_func);
	signal(SIGINT,kill_func);
	pthread_create(&thread_process,NULL,process,&ctx);
}

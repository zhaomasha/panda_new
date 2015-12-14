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

void process(void* args)
{
	context_t* ctx = (context_t*)args;
	BackControllerMeta * back_meta = BackControllerMeta::get_instance();

	//config back controller
	string master_ip = back_meta.get_ip_master();
	int32_t back_ctl_port = back_meta.get_ctl_port();
	std::string endpoint="tcp://"+master_ip+":"+back_ctl_port;
	int32_t back_cycle = back_meta.get_back_cycle();
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
			case :

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

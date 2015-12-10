/*************************************************************************
 * File Name: panda_backmeta.cpp
 * Author: wangxin
 * Mail: wangxin4@iie.ac.cn
 * Created Time: Thu 10 Dec 2015 03:31:09 PM CST
 * Description:
 * 
************************************************************************/

#include<iostream>

#include "panda_backmeta.h"
using namespace std;


static BackControllerMeta* BackControllerMeta::instance=NULL;

static BackControllerMeta* BackControllerMeta::get_instance()
{
	if(instance == NULL){
		return new BackControllerMeta();
	}
	return instance;
}


BackControllerMeta::BackControllerMeta()
{
	parse_env("SLAVE_IP", slave_infos, ":");
	back_cycle = atoi(getenv("BACK_CYCLE"));

}


uint32_t BackControllerMeta::get_back_cycle()
{
	return back_cycle;
}


int BackControllerMeta::valid_slave(string slave_info)
{
	for(vector<string>::iterator it = slave_infos.begin();
			it != slave_info.end(); ++it){
		if(*it == slave_info)
			return 1;
	}
	return 0;
}


void BackControllerMeta::remove_slave(string slave_info)
{
	for(vector<string>::iterator it = slave_infos.begin();
			it != slave_info.end(); ++it){
		if(*it == slave_info){
			slave_infos.erase(it);
			return ;
		}
	}
}


int BackControllerMeta::add_slave(string slave_info)
{
	if(!valid_slave(slave_info)){
		slave_infos.push_back(slave_info);
		return 0;
	}
	return -1;
}

int32_t BackControllerMeta::get_ctl_port()
{
	return controller_port;
}

int32_t BackControllerMeta::get_worker_port()
{
	return worker_port;
}

string BackControllerMeta::get_ip_master()
{
	return master_ip;
}

vector<string> BackControllerMeta::get_slave_infos()
{
	return slave_infos;
}



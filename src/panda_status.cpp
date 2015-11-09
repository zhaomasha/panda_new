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

typedef std::vector<std::string>::iterator string_it;
typedef std::vector<int>::iterator int_it;

PandaStatus::PandaStatus(std::vector<std::string> slave_hosts, int _alive_threshold ):alive_threshold(_)
{
	for(string_it host=slave_hosts.begin(); host!=slave_hosts.end(); ++host){
		init_slaves.insert(*host)
	}
}



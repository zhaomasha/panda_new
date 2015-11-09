/*************************************************************************
 * File Name: ../inc/panda_status.h
 * Author: wangxin
 * Mail: wangxin4@iie.ac.cn
 * Created Time: 2015年11月09日 星期一 16时36分07秒
 * Description:
 * 
************************************************************************/

#include<iostream>
using namespace std;

class PandaStatus{
public:
	/*
	 *  slave_hosts: slaves' host info(ip)'
	 *  _alive_threshold: the threshold time to jude if slave is alive, in second
	 */
	PandaStatus(std::vector<std::string> slave_hosts, unsigned int _alive_threshold ):alive_threshold(_){}

	/*
	 *  _alive_threshold: the threshold time to jude if slave is alive, in second
	 */
	PandaStatus(unsigned int _alive_threshold):alive_threshold(_){}
	~PandaStatus(unsigned int _alive_threshold):alive_threshold(_){}

	/*
	 *  add a slave to system
	 *  input:
	 *		host_ip: the slave's ip that added
	 *	output:
	 *		serial_num: the serial number allocated to the specified slave, using as ID of keeping alive.
	 *	return: SUCCESS(0)/FAILED(-1)
	 */
	int add_slave(std::string host_ip, unsigned int * serial_num);

	/*
	 * check if there are lost slaves.
	 * input:
	 *		None
	 * output:
	 *		lost_hosts, output the lost hosts' info(ip).
	 * return: SUCCESS(0)/FAILED(-1)
	 */
	int check_alive(std::vector<std::string>& lost_hosts);

	/*
	 * doing update state action according to the time 
	 * input:
	 *		t, specify the time, in second.
	 *	output:
	 *		None
	 */
	void slave_time_lapse(unsigned int t);
	
private:
	//alive info
	std::vector<int> slave_alive_info;
	//hosts info
	std::map<unsigned int, std::string> slave_hosts;

	unsigned int get_slave_serial();
};

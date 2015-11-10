/*************************************************************************
 * File Name: ../inc/panda_status.h
 * Author: wangxin
 * Mail: wangxin4@iie.ac.cn
 * Created Time: 2015年11月09日 星期一 16时36分07秒
 * Description:
 * 
************************************************************************/

#include <string>
#include <map>
#include <vector>
#include <set>

class PandaStatus{
public:
	/*
	 *  slave_hosts: slaves' host info(ip),keep the info to ckeck if all slaves is connected'
	 *  _alive_threshold: the threshold time to jude if slave is alive, in second
	 */
	PandaStatus(std::vector<std::string> slave_hosts, int _alive_threshold ):alive_threshold(_alive_threshold);

	/*
	 *  _alive_threshold: the threshold time to jude if slave is alive, in second
	 */
	PandaStatus(unsigned int _alive_threshold):alive_threshold(_alive_threshold){}
	~PandaStatus(){}

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
	/*
	 * update_slave's status
	 * input:
	 *		serial_num, indicate the slave
	 */
	void update_slave(unsigned int serial_num);
	
private:
	//alive info, indicate unused with negative
	std::vector<int> slave_alive_info;
	//slave hosts' info, that keep connected with master
	std::map<unsigned int, std::string> slave_hosts;
	//slave hosts' initial info
	std::set<std::string>  unconnected_slaves;
	//threshold to judge if a slave is alive.
	int alive_threshold;


	unsigned int get_slave_serial();
};

void* keep_status_slave(void *);
void* keep_status_master(void*);

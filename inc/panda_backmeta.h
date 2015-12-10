/*************************************************************************
 * File Name: panda_backmeta.h
 * Author: wangxin
 * Mail: wangxin4@iie.ac.cn
 * Created Time: Thu 10 Dec 2015 02:54:23 PM CST
 * Description:
 * 
************************************************************************/

#ifndef PANDA_BACKMETA_HEADER
#define PANDA_BACKMETA_HEADER

#include <vector>
#include <string>

using namespace std;

class BackControllerMeta{
public:
	/*
	 * get instance of meta class
	 */
	static BackControllerMeta* get_instance();
	/*
	 * get back cycle
	 *	return back cycle value
	 */
	uint32_t get_back_cycle();
	/*
	 *	to valid if a slave's info is in current slave list
	 *	input:
	 *		slave_info, the slave's info to be valid
	 *	return:
	 *		0 if not in , nonzero if in
	 */
	int valid_slave(string slave_info);
	/*
	 * remove a slave's info from current list
	 *	input:
	 *		slave_info, the slave's info to be removed
	 */
	void remove_slave(string slave_info);
	/*
	 *  add a slave to current slave list
	 *  input:
	 *		slave_info, the slave info to add
	 *	return:
	 *		0 if success, -1 if slave_info already exist
	 */
	int add_slave(string slave_info);
	/*
	 * get a copy of the slave info list
	 *	return the copy
	 */
	vector<string> get_slave_infos();
	/*
	 * get back controller's binding port
	 */
	int32_t get_ctl_port();
	/*
	 * get back worker's binding port
	 */
	int32_t get_worker_port();
	/*
	 * get master's ip
	 */
	string get_ip_master();
private:
	vector<string> slave_infos; //record current slave's host info
	static BackControllerMeta* instance;
	uint32_t back_cycle; // in second
	uint32_t worker_port;
	uint32_t controller_port;
	string master_ip;
	BackControllerMeta();
}


#endif

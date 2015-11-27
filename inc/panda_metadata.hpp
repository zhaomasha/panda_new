#ifndef PANDA_METADATA
#define PANDA_METADATA
#include "panda_metadata.hpp"
#include "panda_head.hpp"
#include "panda_util.hpp"
#include "panda_type.hpp"
//一个图的元数据，子图和节点的对应关系
class metadata{
public:
	//初始化一个元数据文件，一个元数据文件代表一个图的元数据
	void init(string name,string path);
	//添加子图和ip项
	void add_meta(uint32_t key,string ip);
	//返回子图对应的ip
	string find_meta(uint32_t key);
	void get_slave_sub(string slave_host, vector<uint32_t>& sub_ids);
	void remove_subs(const vector<uint32_t>& sub_ids);
	//把新的元数据重新写入文件中去，覆盖以前旧的
	void flush();
	//析构函数，把新的元数据重新写入文件中去，覆盖以前旧的
	~metadata();
	void print();//打印元数据，测试用
private:
	string graph_name;//该元数据所属的图的名字
	string path;//元数据文件名
	unordered_map<uint32_t,string> meta;//图的元数据，子图id和ip的hash表
};
class GraphMeta{
public:

	/*
	 * get the graph meta data manager's instance(singleinstance).
	 */
	static GraphMeta* get_instance();

	/*
	 * Check if the graph is already in system
	 * input:
	 *		graph_name, specify the graph's name
	 * return:
	 *		1 if the graph found, 0 not
	 */
	int find_graph(string graph_name);

	/*
	 *	Find the vertex' location in the system, which actually is 
	 *	location of the subgraph the vertex belong. If the subgra-
	 *	ph is not exist , it will be created.
	 *	input:
	 *		graph_name, the graph's name that the vertex belong
	 *		vertex_id, specify id of the vertex.
	 *	return:
	 *		Empty string if the graph is not exist, or ip of the
	 *		vertex belongs.
	 *
	 */
	string find_loc(string graph_name, v_type vertex_id);

	/*
	 *	Create a new graph in system
	 *	input:
	 *		graph_name, specify the graph's name.
	 *	return:
	 *		0 if success, -1 if graph already exist.
	 */
	int create_graph(string graph_name);

	/*
	 *	redistribute subgraph according to the lost slaves
	 */
	void redistribute(const vector<string> &lost_slaves, vector<RedistributeTerm>& redistribute_info);
	/*
	 *	Print all meta info
	 */
	void print();
	/*
	 *	Flush meta content to file
	 */
	void flush();
	~GraphMeta();
private:
	unordered_map<string,metadata*> metas;//graph和元数据对应表
	lock_t* meta_lock;
	static GraphMeta* meta_instance;
	string server_dir_name;
	metadata* __find_graph(string graph_name);
	GraphMeta();
};

//负载的类，每个节点所对应的子图数目当作负载
class Balancer{
public:
	/*
	 *	Get balancer's instance,in single instance mode
	 */
	static Balancer* get_instance();
	string get_min();//得到负载最小的节点
	void update(string ip,int num);//更新节点的负载，num代表增加的数目
	void remove_slave(string ip);
	//把最新的负载写入到文件中，覆盖旧的内容
	void flush();
	~Balancer();//把最新的负载写入到文件中，覆盖旧的内容
	void print();//打印元数据，测试用
private:
	string path;//负载文件的路径，在参数里面写好了
	unordered_map<string,uint32_t> bal;//ip和子图数目的hash表
	lock_t* bal_lock;
	static Balancer* bal_instance;
	Balancer();
	void init();//初始化负载均衡
	string __get_min();
};

#endif

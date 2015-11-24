#include "panda_metadata.hpp"
extern string server_dir_name;
//初始化一个子图的元数据
void metadata::init(string name,string path){
	graph_name=name;
	this->path=path;
	ifstream fin;
	fin.open(path.c_str());
	string tmp;
	while(getline(fin,tmp)){
		meta.insert(parse_sub_ip(tmp,":"));
	}
}
//添加子图和ip项
void metadata::add_meta(uint32_t key,string ip){
	meta[key]=ip;
}
//返回子图所在的节点，不存再则返回空串
string metadata::find_meta(uint32_t key){
	unordered_map<uint32_t,string>::iterator it=meta.find(key);
	if(it==meta.end()) return "";
	return it->second;
}
void metadata::flush()
{
	ofstream out;
	out.open(path.c_str(),ios::trunc);//如果文件已经存在，则先删除文件
	unordered_map<uint32_t,string>::iterator it=meta.begin();
	//把所有的元数据写到文件中去
	while(it!=meta.end()){
		out<<it->first<<":"<<it->second<<"\n";
		it++;
	}
	out.close();
}
//析构函数，把新的元数据重新写入文件中去，覆盖以前旧的
metadata::~metadata(){
	flush();
}
void metadata::print(){
	unordered_map<uint32_t,string>::iterator it=meta.begin();
	while(it!=meta.end()){
		cout<<it->first<<" "<<it->second<<endl;
		it++;
	}
}

GraphMeta::GraphMeta* get_instance()
{
	if(meta_instance == NULL){
		meta_instance = new GraphMeta();
	}
	return meta_instance;
}

int GraphMeta::find_graph(string graph_name)
{
	Lock(meta_lock);

	if( __find_graph(graph_name)){
		Unlock(meta_lock);
		return 1;
	}
	Unlock(meta_lock);
	return 0;
}

//获取元数据的函数，根据图和顶点获取所在的节点，如果顶点所在的子图还没有分配节点，则根据负载来分配，虽然这时候该子图还没有数据，但是已经分配好了节点，这是个异步过程。客户端会先确保访问的图存在，所以这个函数能调用，就肯定能保证该图已经存在。为了保险起见，如果图不存在，则返回空串
string GraphMeta::find_loc(string graph_name, v_type vertex_id)
{
	Lock(meta_lock);
	metadata* g_meta = __find_graph(graph_name);
	if( !g_meta ){
		//graph not exist
		Unlock(meta_lock);
		return "";
	}
	uint32_t subgraph_key=get_subgraph_key(id);//得到顶点所在的子图
	string sub_ip=g_meta->find_meta(subgraph_key);
	if(sub_ip==""){
		//如果图中该子图没有，则分配一个节点给该子图
		Balancer* bal = Balancer::get_instance();
		string ip=bal->get_min();
		bal->update(ip,1);//该节点的负载加1
		m->add_meta(key,ip);//更新该图的元数据
		sub_ip = ip;
	}
	Unlock(meta_lock);
	return sub_ip;	
}

int GraphMeta::creat_graph(string graph_name)
{
	Lock(meta_lock);
	if( __find_graph(graph_name)){
		Unlock(meta_lock);
		return -1;
	}
	string path=server_dir_name+"/"+graph_name+".meta";
	ofstream fout(path.c_str());
	fout.close();
	metadata* m=new metadata();
	m->init(graph_name,path);
	metas.insert(pair<string,metadata*>(graph_name,m));			  
	Unlock(meta_lock);
	return 0;
}

void GraphMeta::print()
{
	Lock(meta_lock);
	unordered_map<string,metadata*>::iterator it=metas.begin();
	while(it!=metas.end()){
		cout<<it->first<<":"<<endl;
		it->second->print();
		it++;
	}
	Unlock(meta_lock);
}

void GraphMeta::flush()
{
	Lock(meta_lock);
	for(unordered_map<string, metadata*>::iterator it = metas.begin();
			it != metas.end(); ++it){
		it->second->flush();
	}
	Unlock(meta_lock);
}

GraphMeta::GraphMeta()
{
	//initialize lock
	meta_lock = Getlock();
	Initlock(meta_lock, NULL);
	//load meta data in disk
    glob_t g;
	g.gl_offs=0;
	string meta_pattern=server_dir_name+"/*.meta";
	int res=glob(meta_pattern.c_str(),0,0,&g);
	if(res!=0&&res!=GLOB_NOMATCH){
		cout<<"master meta failed to invoking glob"<<endl;
	}else{
		if(g.gl_pathc==0){
			cout<<"master meta no match"<<endl;
		}else{
			for(uint32_t i=0;i<g.gl_pathc;i++){
				metadata* m=new metadata();
				string graph_name=metapath_key(g.gl_pathv[i]);
				string path(g.gl_pathv[i]);
				m->init(graph_name,path);
				metas.insert(pair<string,metadata*>(graph_name,m));	
			}
		}
	}
}
GraphMeta::~GraphMeta()
{
	flush();
	Destroylock(meta_lock);
}

metadata* GraphMeta::__find_graph(string graph_name)
{
	unordered_map<string, metadata*>::iterator rlt = metas.find(graph_name);
	if( rlt == metas.end() ){
		return NULL;
	}
	return rlt->second;
}
void GraphMeta::redistribute(vector<string> lost_slaves, vector<RedistributeTerm>& redistribute_info)
{
	
}

Balancer* Balancer::get_instance()
{
	if( bal_instance == NULL){
		bal_instance = new Balance();
	}
	return bal_instance;
}

Balancer::Balancer()
{
	//initialize balance data from config file
	init();
	//init lock
	bal_lock = Getlock();
	Initlock(bal_lock, NULL);

}

//初始化负载，参数中配置了slave集群，每个slave都有负载
void Banlancer::init(){
	path=string(getenv("BAL_DIR_NAME"))+"/banlance.cfg";
	if(access(path.c_str(),0)!=0){
		//如果不存在负载文件，则创建一个负载文件
 		ofstream fout(path.c_str());
		fout.close();
		//初始化内存中的负载,每个节点都为负载都为0
		vector<string> v;
		parse_env("SLAVE_IP",v,":");
		for(int i=0;i<v.size();i++){
			bal.insert(pair<string,uint32_t>(v[i],0));
		}
	}else{
		//否则读取负载文件，初始化内存中的负载
		ifstream fin;
		fin.open(path.c_str());
		string tmp;
		while(getline(fin,tmp)){
			bal.insert(parse_ip_num(tmp,":"));
		}
	}
}

void Balancer::flush()
{
	Lock(bal_lock);
	ofstream out;
	out.open(path.c_str(),ios::trunc);//如果文件已经存在，则先删除文件
	unordered_map<string,uint32_t>::iterator it=bal.begin();
	while(it!=bal.end()){
		out<<it->first<<":"<<it->second<<"\n";	
		it++;
	}
    out.close();
	Unlock(bal_lock);
}

Banlancer::~balance()
{
	flush();
	Destroylock(bal_lock);
	free(bal_lock);
}

//得到最小负载的节点，遍历负载集合，找出最小负载的ip
string Banlancer::get_min()
{
	Lock(bal_lock);
	string ip = __get_min();
	Unlock(bal_lock);
	return ip;
}

string Banlancer::__get_min()
{
	unordered_map<string,uint32_t>::iterator it=bal.begin();
	uint32_t min;
	int n=0;
	string ip;
	while(it!=bal.end()){
		if(n==0) {
			min=it->second;
			ip=it->first;
		}
		else{
			if(it->second<min) {
				min=it->second;
				ip=it->first;
			}
		}
		n++;
		it++;
	}
	return ip;
}

void Banlancer::update(string ip,int num){
	Lock(bal_lock);
	unordered_map<string,uint32_t>::iterator it=bal.find(ip);
	if(it==bal.end()) cout<<"master: no "<<ip<<" node"<<endl;
	else{
		it->second+=num;
	}
	Unlock(bal_lock);
}
void Banlancer::print(){
	Lock(bal_lock);
	unordered_map<string,uint32_t>::iterator it=bal.begin();
	while(it!=bal.end()){
		cout<<it->first<<" "<<it->second<<endl;	
		it++;
	}
	Unlock(bal_lock);
}




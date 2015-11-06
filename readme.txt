This is a ditributed system for graph storaging and querying,which is vertex centered model.


configuration:
    配置参数说明(panda/env/param.sh)：
    panda_home 没用
    INITSZ 每个磁盘文件初始化的大小，单位是MB
    BLOCKSZ 子图文件的block的大小，单位是B
    INDEX_BLOCKSZ 索引文件的block大小，单位是B
    INCREASZ 磁盘文件每次增加的大小，单位是MB
    CACHESZ 每个子图在内存中缓存的block数目
    MASTER_IP master进程所在的ip
    MASTER_PORT master进程的端口
    SLAVE_IP 所有slave的ip，每个ip之间用冒号隔开
    SLAVE_PORT slave进程的端口
    LOCAL_IP 该slave所在的ip，每个slave不一样
    DIR_NAME 数据库文件所在的根目录
    BAL_DIR_NAME master存放负载均衡文件的目录
    SERVER_DIR_NAME master存放每个图的元数据文件的目录
    HASH_NUM 子图的数目，也就是顶点被取模的数
    SLAVE_THREAD_NUM slave的工作线程数目
    VERTEX_INDEX_FILENAME 顶点索引的名字
    EDGE_INDEX_FILENAME 边属性索引的名字

部署说明：
    1.整个panda目录放在任意目录下，集群有几个节点，就得在每个节点的相同目录下放置一份代码。
    2.配置系统，按上面的说明配置
    3.sbin目录下面有启动脚本,和停止脚本,start_panda.sh stop_panda.sh 还有一个清空脚本，就是把数据全部删掉clean_panda.sh
    4.fuzhu目录下面有一些测试程序，和测试数据
    5.deploy目录下为自动化部署相关内容

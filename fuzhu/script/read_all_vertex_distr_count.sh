#########################################################################
# File Name: read_all_vertex_distr_count.sh
# Author: wangxin
# mail: wangxin4@iie.ac.cn
# Created Time: Fri 16 Oct 2015 10:21:12 AM CST
# Description:
#########################################################################
#!/bin/bash

base_dir=`dirname $BASH_SOURCE`

time_max=0
num_all=0
while read ip
do
    if [ `cat $base_dir/../log//$ip.readall.log|wc -l` == 0 ]
    then
        echo Not all client complete,please wait...
        exit 1
    fi
    time=`cat $base_dir/../log//$ip.readall.log|awk '{print $8}'`
    num=`cat $base_dir/../log//$ip.readall.log|awk '{print $11}'`
    if [ $time_max -lt $time ]
    then
        time_max=$time
    fi
    num_all=`expr $num_all + $num`
done < $base_dir/../config/client.ip

echo time= $time_max ms
echo read_num= $num_all vertexes


#########################################################################
# File Name: read_all_vertex_ditr.sh
# Author: wangxin
# mail: wangxin4@iie.ac.cn
# Created Time: Fri 16 Oct 2015 09:42:19 AM CST
# Description:
#########################################################################
#!/bin/bash

user_name="wangxin"


base_dir=`dirname $BASH_SOURCE`
read home_dir < $base_dir/../config/client.path

mkdir -p $base_dir/../log

while read ip
do 
    ssh -f  $user_name@$ip $home_dir/fuzhu/script/read_all_vertex.sh 0 > $base_dir/../log/$ip.readall.log
done < $base_dir/../config/client.ip

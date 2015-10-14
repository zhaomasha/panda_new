#########################################################################
# File Name: ex_install.sh
# Author: wangxin
# mail: wangxin4@iie.ac.cn
# Created Time: Mon 12 Oct 2015 11:52:45 AM CST
#########################################################################
#!/bin/bash

#usage: ex_install.sh dest_host user_name
#
if [ $# != 3 ]
then
    echo "invalid parameter numer $#"
    echo "usage: ex_install.sh dest_host user_name install_dir"
    exit 1
fi

dst_host=$1
usr_name=$2
install_dir=$3
script_dir=`dirname $BASH_SOURCE`

zmq_version=3.2.3
zmq_tar_name=zeromq-3.2.3.tar.gz
zmq_tar_path=$script_dir/depend_ware/$zmq_tar_name


ssh $usr_name@$dst_host "mkdir -p $install_dir"

#install zmq
zmq_tag=`ssh $usr_name@$dst_host 'ls -l /usr/local/lib*|grep zmq|wc -l'`
if [ $zmq_tag == 0 ]
then
    scp $zmq_tar_path  $usr_name@$dst_host:$install_dir
    ssh $usr_name@$dst_host "cd $install_dir && tar zxvf $install_dir/$zmq_tar_name&& cd zeromq-$zmq_version&& ./configure&& make&& make install && make clean && cd $install_dir && rm -fr $install_dir/zeromq-$zmq_version"
else
    echo "zmq alread installed on $host"
fi

echo "install complete"



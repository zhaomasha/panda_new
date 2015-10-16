#########################################################################
# File Name: deploy.sh
# Author: wangxin
# mail: wangxin4@iie.ac.cn
# Created Time: Thu 15 Oct 2015 05:30:48 PM CST
# Description:
#########################################################################
#!/bin/bash

base_dir=`dirname $BASH_SOURCE`

username="root"
passwd="111111"
home_dir="/home/wangxin/panda"
install_dir="$home_dir/install"

#create directory for install
$base_dir/../../deploy/automkdir.exp $username $passwd "127.0.0.1" $install_dir

#install tcl and expect
$base_dir/../../deploy/base_install.sh

while read ip
do
    echo deploy $ip
    #create directory for install
    $base_dir/../../deploy/automkdir.exp $username $passwd $ip $install_dir
    #install zmq
    $base_dir/../../deploy/install.sh $username $passwd $ip $install_dir

    echo deploy $ip complete
done<$base_dir/../config/client.ip



echo hi

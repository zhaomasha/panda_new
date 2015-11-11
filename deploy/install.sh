#########################################################################
# File Name: install.sh
# Author: wangxin
# mail: wangxin4@iie.ac.cn
# Created Time: Thu 15 Oct 2015 05:57:44 PM CST
# Description:
#########################################################################
#!/bin/bash


username=$1
passwd=$2
hostname=$3
install_dir=$4

base_dir=`dirname $BASH_SOURCE`

$base_dir/install.exp $username $passwd $hostname $install_dir $base_dir

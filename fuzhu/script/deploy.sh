#########################################################################
# File Name: deploy.sh
# Author: wangxin
# mail: wangxin4@iie.ac.cn
# Created Time: Thu 15 Oct 2015 05:30:48 PM CST
# Description:
#########################################################################
#!/bin/bash
set -x

base_dir=`dirname $BASH_SOURCE`

username="root"
passwd="111111"
read home_dir < $base_dir/../config/client.path
install_dir="$home_dir/install"
env_dir="$home_dir/env"
lib_dir="$home_dir/lib"
client_dir="$home_dir/fuzhu"

#create directory for install
#$base_dir/../../deploy/automkdir.exp $username $passwd "127.0.0.1" $install_dir

#install tcl and expect
$base_dir/../../deploy/base_install.sh

while read ip
do
    echo deploy $ip
    #create directory for install
    $base_dir/../../deploy/automkdir.exp $username $passwd $ip $install_dir
    #install zmq
    $base_dir/../../deploy/install.sh $username $passwd $ip $install_dir
    #copy files
    $base_dir/../../deploy/autocmd.exp $username $passwd $ip "rm -fr $env_dir"
    $base_dir/../../deploy/autoscp.exp $username $passwd $ip $base_dir/../../env $env_dir
    deploy_envfile_path=$base_dir/../../deploy/envfiles/${ip}.envfile
    if [ -e $deploy_envfile_path ]
    then
        $base_dir/../../deploy/autoscp.exp $username $passwd $ip $deploy_envfile_path $env_dir/param.sh
    fi
    $base_dir/../../deploy/autocmd.exp $username $passwd $ip "rm -fr $lib_dir"
    $base_dir/../../deploy/autoscp.exp $username $passwd $ip $base_dir/../../lib $lib_dir
    $base_dir/../../deploy/autocmd.exp $username $passwd $ip "rm -fr $client_dir"
    $base_dir/../../deploy/autoscp.exp $username $passwd $ip $base_dir/../../fuzhu $client_dir
    echo deploy $ip complete
done<$base_dir/../config/client.ip



echo client deploy all complete

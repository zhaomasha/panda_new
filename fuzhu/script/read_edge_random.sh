#########################################################################
# File Name: read_edge_random.sh
# Author: wangxin
# mail: wangxin4@iie.ac.cn
# Created Time: Wed 14 Oct 2015 11:53:51 AM CST
# Description:
#########################################################################
#!/bin/bash

if [ $# != 4 ]
then
    echo "usage: read_edge_random.sh start_id end_id repeat_num verbose"
    exit 1
fi

BASE_DIR=`dirname $BASH_SOURCE`

. $BASE_DIR/prepare.sh

start_id=$1
end_id=$2
repeat_num=$3
verbose=$4

$BASE_DIR/../bin/read_edge_random test $start_id $end_id $verbose $repeat_num


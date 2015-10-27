#########################################################################
# File Name: insert_vertex.sh
# Author: wangxin
# mail: wangxin4@iie.ac.cn
# Created Time: Wed 14 Oct 2015 09:52:56 AM CST
# Description:
#########################################################################
#!/bin/bash

if [ $# != 2 ]
then
    echo "usage: insert_vertex.sh start_id end_id"
    exit 1
fi

BASE_DIR=`dirname $BASH_SOURCE`
. $BASE_DIR/prepare.sh

min=$1
max=$2
../bin/input_random_vertex $min $max 1000 > insert.log 

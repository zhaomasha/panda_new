#########################################################################
# File Name: insert_edge.sh
# Author: wangxin
# mail: wangxin4@iie.ac.cn
# Created Time: Wed 14 Oct 2015 09:55:19 AM CST
# Description:
#########################################################################
#!/bin/bash

if [ $# != 2 ]
then
    echo "usage: insert_edge.sh start_id end_id"
    exit 1
fi

BASE_DIR=`dirname $BASH_SOURCE`
. $BASE_DIR/prepare.sh

../bin/input_random_edge $1 $2 1000 >> insert.log

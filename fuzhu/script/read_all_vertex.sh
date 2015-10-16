#########################################################################
# File Name: read_all_vertex.sh
# Author: wangxin
# mail: wangxin4@iie.ac.cn
# Created Time: Wed 14 Oct 2015 12:01:50 PM CST
# Description:
#########################################################################
#!/bin/bash

if [ $# != 1 ]
then
    echo "usage: read_all_vertex.sh verbose"
    exit 1
fi

BASE_DIR=`dirname $BASH_SOURCE`
. $BASE_DIR/prepare.sh

verbose=$1

$BASE_DIR/../bin/read_all_vertex test $verbose

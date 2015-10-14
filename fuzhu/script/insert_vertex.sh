#########################################################################
# File Name: insert_vertex.sh
# Author: wangxin
# mail: wangxin4@iie.ac.cn
# Created Time: Wed 14 Oct 2015 09:52:56 AM CST
# Description:
#########################################################################
#!/bin/bash


BASE_DIR=`dirname $BASH_SOURCE`
. $BASE_DIR/prepare.sh

../bin/input_random_vertex 1 10000000 1000 > insert.log 

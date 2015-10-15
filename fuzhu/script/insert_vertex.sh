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

min=1
max=10000
../bin/input_random_vertex $min $max 1000 > insert.log 

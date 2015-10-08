#########################################################################
# File Name: tmp.sh
# Author: wangxin
# mail: wangxin4@iie.ac.cn
# Created Time: 2015年09月29日 星期二 18时02分52秒
#########################################################################
#!/bin/bash

. ../env/param.sh
. ./prepare.sh
./input_random_vertex 1 1000000000 1000 > insert.log 
./input_random_edge 1 1000000000 1000 >> insert.log

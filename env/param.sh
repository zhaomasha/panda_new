#!/bin/bash
export panda_home='/home/wangxin/panda_new' 
export INITSZ='10'
export BLOCKSZ='2048'
export INDEX_BLOCKSZ='4096'
export INCREASZ='10'
export CACHESZ='8192'
#export MASTER_IP="127.0.0.1"
export MASTER_IP="192.168.11.52"
export MASTER_PORT="4344"
export SLAVE_IP="192.168.11.53:192.168.11.54:192.168.11.55:192.168.11.59"
#export SLAVE_IP="192.168.11.52:192.168.11.53:192.168.11.54:192.168.11.56:192.168.11.57:192.168.11.58:192.168.11.59"
#export SLAVE_IP="127.0.0.1"
export SLAVE_PORT="4345"
export LOCAL_IP="192.168.11.52"

export DIR_NAME=$panda_home"/data"
export BAL_DIR_NAME=$DIR_NAME"/panda_bal"
export SERVER_DIR_NAME=$DIR_NAME"/panda_server"

export HASH_NUM="100" 
export SLAVE_THREAD_NUM="10" 
export VERTEX_INDEX_FILENAME="vertex.index"
export EDGE_INDEX_FILENAME="edge.index"


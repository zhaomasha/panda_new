#!/usr/bin/env bash

set -x

BASE_DIR="`dirname $BASH_SOURCE`/..";
BASE_DIR="`cd $BASE_DIR;pwd`";
echo $BASE_DIR;

. $BASE_DIR/env/param.sh
echo $LOCAL_IP;

echo $SLAVE_IP | tr ':' '\n' | while read slave;do
	echo $slave;
	ssh -n $slave "rm -rf /home/zhaomasha/graphdata;rm -rf /home/zhaomasha/panda/logs"
done

echo "panda clean";

set +x

#!/usr/bin/env bash

set -x

BASE_DIR="`dirname $BASH_SOURCE`/..";
BASE_DIR="`cd $BASE_DIR;pwd`";
echo $BASE_DIR;

. $BASE_DIR/env/param.sh
echo "stopping panda...";
echo $SLAVE_IP | tr ':' '\n' | while read slave;do
	echo $slave;
	ssh -n $slave "bash $BASE_DIR/sbin/stop_panda_base.sh" > /dev/null 2>&1
done


SERVER_ID=`ps aux | grep panda_server | grep -v 'grep' | awk '{print $2}'`;
kill -INT $SERVER_ID

echo "panda stopped";

set +x

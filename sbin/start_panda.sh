#!/usr/bin/env bash
set -x

BASE_DIR="`dirname $BASH_SOURCE`/..";
BASE_DIR="`cd $BASE_DIR;pwd`";
echo $BASE_DIR;

mkdir -p $BASE_DIR/logs

. $BASE_DIR/env/param.sh
echo $LOCAL_IP;

echo "starting panda...";
nohup $BASE_DIR/bin/panda_server >> $BASE_DIR/logs/server.log 2>&1 &

echo $SLAVE_IP | tr ':' '\n' | while read slave;do
	echo $slave;
	ssh -n $slave "ulimit -c unlimited;mkdir -p $BASE_DIR/logs;. $BASE_DIR/env/param.sh;nohup $BASE_DIR/bin/panda_slave >> $BASE_DIR/logs/slave.log 2>&1 &"
done

echo "panda started";

set +x

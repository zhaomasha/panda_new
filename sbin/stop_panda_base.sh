#!/usr/bin/env bash

set -x

SLAVE_ID=`ps aux | grep panda_slave | grep -v 'grep' | awk '{print $2}'`;
kill -INT $SLAVE_ID
#kill -INT $SERVER_ID > /dev/null 2>&1;

set +x

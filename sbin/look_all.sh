#!/usr/bin/env bash


doforall.sh "ps -C bsp_master -C bsp_slave -C panda_server -C panda_slave -C bsp_jobmaster"
doforall.sh "ps aux | grep task[0-9]"

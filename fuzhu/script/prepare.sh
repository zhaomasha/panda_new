#! /bin/sh
fuzhu_dir=$panda_home
cp $panda_home"/lib/libpanda.so" ./
ls $fuzhu_dir/lib/libpanda.so


export LD_LIBRARY_PATH=../lib


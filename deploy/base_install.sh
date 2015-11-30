#########################################################################
# File Name: base_install.sh
# Author: wangxin
# mail: wangxin4@iie.ac.cn
# Created Time: Mon 12 Oct 2015 10:49:07 AM CST
#########################################################################
#!/bin/bash

script_dir=`dirname $0 |cd;pwd`
echo $script_dir

tcl_version=8.5.18
tcl_tar_name=tcl8.5.18-src.tar.gz
tcl_tar_path=$script_dir/depend_ware/$tcl_tar_name

expect_version=5.45
expect_tar_name=expect5.45.tar.gz
expect_tar_path=$script_dir/depend_ware/$expect_tar_name

set -x


#install tcl
tar zxf $tcl_tar_path -C $script_dir
tcl_tag=`ls -l /usr/local/bin/ /usr/bin/|grep " tcl"|wc -l`
if [ $tcl_tag == 0 ]
then
    cd $script_dir/tcl$tcl_version/unix
    ./configure
    make
    sudo make install
    cd -
else
    echo 'tcl alread installed'
fi

#install expect
expect_tag=`ls -l /usr/local/bin/ /usr/bin/|grep expect|wc -l`
if [ $expect_tag == 0 ]
then
    tar zxf $expect_tar_path -C $script_dir
    cd  $script_dir/expect$expect_version
    ./configure --with-tclinclude=$script_dir/tcl$tcl_version/generic/ --with-tclconfig=/usr/local/lib/
    make
    sudo make install
    cd -
else
    echo 'expect alread installed'
fi

rm -fr $script_dir/tcl$tcl_version
rm -fr $script_dir/expect$expect_version

